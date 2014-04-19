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

for entry in z:
	print("%08x %s" % entry)

