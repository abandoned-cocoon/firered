#Patching games for the GBA
This document describes a workflow for patching GBA games with C code which allows debugging and has low per-function overhead. I'm assuming that the ROM being patched has empty space somewhere in which new functions can be placed. I still want to make use of the space of the functions replaced though. So I'll also talk about how to reduce code size enough to fit in those function gaps.
###About sections
ELF files consist of sections. Sections have permissions (read/write/executable) and byte contents (or are marked as zero initialized). C files are usually compiled to ELF files with three sections: one for code (*.text*), one for data (*.data*) and a writable one (*.bss*). During linking .text, .data and .bss sections of the individual  files are combined to one bigger .text, .data and .bss section respectively. This process is controlled by a linker script.
### The idea
By putting every single function in its own .text.functioname section, and providing an ELF file that contains the old ROM we can use a linker script to interleave the two.
###Makefile
I couldn't get clang to output .o files that declare they use EABI version 0.
```
TOOLCHAIN = arm-none-eabi
PATCH_CFLAGS =
```
By putting each funcion in its own section the linker script can place them precisely.

	PATCH_CFLAGS += -ffunction-sections

The original games don't use a frame pointer either (this reduces code size a bit)

	PATCH_CFLAGS += -fomit-frame-pointer

ARM has near and far braches. The compiler picks the appropriate one based on distance. But when using function sections, the final position is determined later in the linker. The compiler optimistically produces near branches and places the burden of inserting trampolines as neccessary to the linker.
However, these linker generated trampolines can not be controlled via the linker script and make byte-precise placement impossible. That's why we tell the compiler to be pessimistic and always use far branches.
That increases code size but avoids subsequent changes to the size in the linker.

	PATCH_CFLAGS += -mlong-calls

GBA games usually use the *thumb* instruction set, and so shall we.

	PATCH_CFLAGS += -mthumb -mcpu=arm7tdmi

The only "old" ARM calling convention/ABI supported by GCC is *apcs-gnu*. Only apcs doesn't require the stack pointer to be aligned at 8 bytes on function boundaries. Else you can end up with `push {r4, lr}` in the function prolog even though r4 doesn't have to be backed up by the function. So this is also for code size reduction. And because the stdlib uses a more recent ABI we can not use it.

	PATCH_CFLAGS += -mabi=apcs-gnu -nostdlib

All files linked together need to use the same ABI. To use the *apcs-gnu* ABI in assembly files we need the command line flag

	PATCH_ASFLAGS = -meabi=gnu

If you want to use VBA-M as your emulator, you have to force the debug info to format version 2. Alternatively you can use *mgba* and connect with gdb.

	PATCH_CFLAGS += -gdwarf-2 -gstrict-dwarf

Optimize for size. (Whether O2 or Os gives the smaller binary has to be determined by experiment.)

	PATCH_CFLAGS += -Os


Command for compiling C files :

	$(TOOLCHAIN)-gcc $(PATCH_CFLAGS) -c -o patch1.o patch1.c

Command for compiling assembly files:

	$(TOOLCHAIN)-as $(PATCH_ASFLAGS) -o patch2.o patch2.s

Command for linking:

	$(TOOLCHAIN)-ld -A arm7tdmi -o out.elf -T linkerscript patch1.o patch2.o

### Linker script
```
MEMORY { ROM (rx) : ORIGIN = 0x08000000, LENGTH = 0x1000000 }
SECTIONS {
	game_var1 = 0x02012345;
	game_var2 = 0x02011223;
	.text 0x08000000 : {
		/* old data interleaved with new data */
		*(.orig.08000000)
		*(.text.func_a)
		*(.orig.08000050)
		*(.text.func_b)
		*(.orig.08000130)
		*(.text.func_c)
		*(.orig.08000270)

		/* fill the following gap in the rom with remaining functions */

		/* I copied this straight from a gcc-generated linker script */
		*(.text .stub .text.* .gnu.linkonce.t.*)
		*(.data .data.* .gnu.linkonce.d.*)
		SORT(CONSTRUCTORS)
		*(.bss .bss.* .gnu.linkonce.b.*)
		*(.rodata*) /* this should be empty */
		*(COMMON)
		/* end copy */

		. = 0x4000; /* seek to end of gap */
		*(.orig.4000)
	}

	/* this is copied from a gcc-generated linker script as well */

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
}
```
###Assembly file to wrap original data in an ELF file
```asm
.section .orig.08000000 , "ax"

	# copy 32 bytes starting at 0 to here
	.incbin "original.gba",0,32

	# functions in the unmodified part can be given names like this
	.global game_func1
	.thumb_func
game_func1:

	# copy 32 bytes starting at 32 to here
	.incbin "original.gba",32,32

	.global game_func2
	.thumb_func
game_func2:

	# copy 8 bytes starting at 64 to here
	.incbin "original.gba",64,8

# This section ends after 0x48 bytes.
# 'func_a' will be inserted here by
# the linker script and original data
# resumes at offset 0x50.

.section .orig.08000050 , "ax"

	.incbin "original.gba",80,8

	# If the replacement for a function is too big for the gap
	# a trampoline can be written here instead.
	# In that case no new section is started

	push {r4, lr}
	ldr r4, [pc, #8]
	bl .+6
	pop {r4, pc}
	bx r4
	.word target_function

	.incbin …
```
### Automation
In this directory there is script called *build.py* which will generate a linker script and assembly file with incbins for you. It takes a descption file in the following format:
```
{
	"rom": ["0x08000000", "0x09000000"],
	"gap": ["0x0871B1C8", "0x08D00000"],
	"functions": [
		{"range": ["0x08000248", "0x00000154"], "name": "irq_handler"},
		{"range": ["0x080003A4", "0x00000108"], "name": "mainloop"},
		…
	],
	"symbols-thumb": [
		["0x08000111", "other_func"]
	],
	"symbols-ram": [
		["0x02036DFC", "current_map"]
	]
}
```

* The entries in *functions* specify the ranges of functions you can replace. Any function from this list for which you don't rpovide a replacement will be moved to *symbols-thumb*.
* The entries in *symbols-thumb* can be used for thumb functions you want to be able to call from your patch functions but don't want to replace or don't know the size of.
* The entries in *symbols-ram* can be used to specify the addresses of variables you want to use in your patch.

To determine the layout the script needs to know the sizes of the replacement functions. A tool called *rabin2* (which is part of the *radare2* toolchain) is used to dump these sizes in JSON format. This dependency might be dropped in the future.