import os

with open("perftsuite.epd", 'r') as f:
	lines = f.readlines();
	data = [x.split(';') for x in lines]
	data = [[x.strip() for x in d] for d in data]

fen = [d[0] for d in data]
res = [d[1:] for d in data]

res = [[r.split()[1] for r in d] for d in res]
for d in res:
	d = d[:-1]
	print(*d, sep=' ')


