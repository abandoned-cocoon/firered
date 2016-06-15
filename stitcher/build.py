
import json
from ast import literal_eval as leval

class SectionBreaker:
	pass

class Region(SectionBreaker):
	def __init__(self, addr, size, gap=False):
		self.addr = addr
		self.size = size
		self.gap = gap

	def write(self):
		return "\n# {} at 0x{:08x}\n".format("gap" if self.gap else "region", self.addr)

class Function:
	def __init__(self, name, addr, size):
		self.name = name
		self.addr = addr
		self.size = size

class EntrypointThumb:
	def __init__(self, name, addr):
		self.name = name
		self.addr = addr
		self.size = 0

	def write(self):
		return '\n\t.global {0}\n{0}:'.format(self.name)


class Patch(SectionBreaker):
	def __init__(self, original, size):
		self.addr = original.addr
		self.size = size
		self.original = original

	def write(self):
		return "\n# patch for {}\n".format(self.original.name)

	def write_ld(self):
		return "\t\t*(.text.{}) /* {:08x} */".format(self.original.name, self.addr)
		#return "\t\t. = 0x{1:08x};\n\t\t*(.text.{0})".format(self.original.name, self.addr)

	def __repr__(self):
		return repr(self.original.name)

class Trampoline:
	def __init__(self, target):
		self.addr = target.addr
		self.target = target.name
		self.size = 18 if self.addr & 2 else 16

	def write(self):
		return """
	push {{r4, lr}}
	ldr r4, [pc, #{}]
	bl .+6
	pop {{r4, pc}}
	bx r4
	{}.word {}
""".format(8 - (self.addr & 2), "nop\n" if self.addr & 2 else "", self.target)

def write(info, patches, symbols_thumb, symbols_ram, rom_path, out_s, out_ld):

	rombegin = leval(info["rom"][0])
	romend   = leval(info["rom"][1])

	events = []
	free_placement = []

	for patch in patches:
		original = patch.original

		if original.size is not None and patch.size > original.size:
			trampoline = Trampoline(original)

			if trampoline.size > original.size:
				print("Patch {} and trampoline are too big for gap at {:08x} ({} > {} > {}).".format(
					patch, original.addr, patch.size, trampoline.size, original.size))
				raise Exception("No space for trampoline")

			events.append(trampoline)
			original.addr = None

		if original.addr is None:
			free_placement.append(patch)

		else:
			events.append(patch)

	for addr, name in symbols_thumb:
		events.append(EntrypointThumb(name, addr))

	gapstart = leval(info["gap"][0])
	gapend   = leval(info["gap"][1])

	regions = [
		Region(addr=rombegin, size=gapstart-rombegin),
		Region(addr=gapstart, size=gapend  -gapstart, gap=True),
		Region(addr=gapend,   size=romend  -gapend)
	]

	print("MEMORY {", file=out_ld)
	print("\tROM (rx) : ORIGIN = 0x{:08x}, LENGTH = 0x{:x}".format(rombegin, romend-rombegin), file=out_ld)
	print("}", file=out_ld)

	events.extend(regions)

	events.sort(key=lambda thing: (thing.addr, 1-isinstance(thing, Region)))

	section = object()
	in_region = None

	print('.thumb', file=out_s)

	def incbin(begin, end):
		nonlocal section
		assert begin <= end, "[{:08x}:{:08x}]".format(begin, end)

		if section is None:
			section = "s{:08x}".format(begin)
			print('.section', section, ', "ax"', file=out_s)
			# print("\t\t. = 0x{:08x};".format(begin), file=out_ld)
			print("\t\t*({})".format(section), file=out_ld)

		if end-begin > 0:
			print('\t.incbin "{}",{},{}'.format(rom_path, begin-rombegin, end-begin), file=out_s)

	def close_region():
		nonlocal in_region
		print("\t}", file=out_ld)
		in_region = None

	def do_gap(event):
		nonlocal addr
		addr = event.addr
		for p in free_placement:
			p.addr = addr
			addr += p.size
			print(p.write_ld(), file=out_ld)

		print("""
		*(.text .stub .text.* .gnu.linkonce.t.*)
		*(.data .data.* .gnu.linkonce.d.*)
		SORT(CONSTRUCTORS)
		*(.bss .bss.* .gnu.linkonce.b.*)
		*(.rodata*) /* this should be empty */
		*(COMMON)""", file=out_ld)


	#	. = 0x08000000;
	#	* (s08000000)
	#	. = 0x08000111;
	#	* (.text.npc_coords_shift)
	#	. = 0x08000121;
	#	* (s08000121)

	rnum = 0
	print("SECTIONS {", file=out_ld)

	for addr, name in symbols_ram:
		print("\t{} = 0x{:08x};".format(name, addr), file=out_ld)
	print("", file=out_ld)

	addr = rombegin
	in_gap = False
	for event in events:
		evaddr = event.addr
		#print("{:08x}/{:08x}/{}/{}".format(addr, evaddr, type(event), vars(event)))

		if in_gap:
			print("\t\t/* remaining functions here */", file=out_ld)
			do_gap(event)
			in_gap = False
			pass
		else:
			incbin(addr, evaddr)

			print(event.write(), file=out_s)
			if hasattr(event, "write_ld"):
				print(event.write_ld(), file=out_ld)

			if isinstance(event, SectionBreaker):
				section = None

		if isinstance(event, Region):
			if "new_region":
				if not in_region:
					in_region = event
					print("\t.text.{} 0x{:08x} : {{".format(rnum, evaddr), file=out_ld)
					rnum += 1

				print("\t\t. = 0x{:08x};".format(evaddr-in_region.addr), file=out_ld)

				if event.gap:
					in_gap = True

			else:
				if in_region and not in_region.gap:
					close_region()

				in_region = event

				if not in_region.gap:
					print("\t.text.{} 0x{:08x} : {{".format(rnum, evaddr), file=out_ld)
					rnum += 1

		if isinstance(event, Region) and event.gap is False:
			addr = evaddr
		else:
			addr = evaddr + event.size

	incbin(addr, romend)
	close_region()


	print("""
	/* DWARF debug sections.
	 Symbols in the DWARF debugging sections are relative to the beginning
	 of the section so we begin them at 0.  */
	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	""", file=out_ld)

	print("}", file=out_ld)

	#memcpy, memmove, memset and memcmp are the four functions required by gcc even with in freestanding mode

def main():
	import sys
	if len(sys.argv) == 5:
		_, p_rom, p_patch, p_asmfile, p_ldscript = sys.argv
	else:
		print("Usage: <this> rom.gba patch.json out.s out.ldscript")
		return

	with open("info.json") as infofile:
		info = json.load(infofile)

	unknowns = []
	funcs = {}
	for fj in info["functions"]:
		addr = leval(fj["range"][0])
		size = leval(fj["range"][1])
		name = fj.get("name", "sub_{:08x}".format(addr))
		funcs[name] = Function(name, addr, size)

	del addr, size, name

	with open(p_patch) as patchfile:
		patch = json.load(patchfile)

	patches = []
	for section in patch["sections"]:
		if section["name"].startswith(".text."):
			funcname = section["name"][6:]
			if funcname not in funcs:
				# allow all unknowns
				func = Function(funcname, None, None)
				unknowns.append(func)
			else:
				func = funcs[funcname]
			patches.append(Patch(func, section["size"]))

	leval_sym_addr = lambda ts: [(leval(addr_expr), name) for addr_expr, name in ts]
	symbols_thumb = leval_sym_addr(info["symbols-thumb"])
	symbols_ram = leval_sym_addr(info["symbols-ram"])

	with open(p_asmfile, "w") as asmfile:
		with open(p_ldscript, "w") as ldscript:
			write(info, patches, symbols_thumb, symbols_ram, p_rom, asmfile, ldscript)

	if not unknowns: return
	print("Unknowns (put into pool)")
	for f in unknowns:
		print(" ", f.name)

if __name__ == '__main__':
	main()
