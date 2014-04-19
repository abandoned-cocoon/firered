import os, re

m_re = re.compile("// ([0-9A-Fa-f]{8})")

z = []

for fname in os.listdir():
	if not fname.endswith(".c"): continue
	for line in open(fname):
		m = m_re.match(line)
		if m:
			z.append((int(m.group(1), 16), fname))

z.sort()

codeonly = True

for addr, line in z:
	if codeonly and not ((0x08000000 <= addr < 0x0815F9B4) or (0x081DBD34 <= addr < 0x081E9F08)):
		continue
	print("%08x %s" % (addr, line))

