#!/usr/bin/env python3
import random, sys

# Arguments:
# 1: path to fasta file containing only the assembled sequence
# 2: desired read_length
# 3: desired coverage
# 4: desired error chance numerator
# 5: desired error chance denominator

usage = sys.argv[0] + " <sequence fasta file> <read length> <coverage> <error numerator> <error denominator>"

bases = "ACGT"

if (len(sys.argv) < 6):
    sys.stderr.write(usage + '\n')
    exit(1)

infile = open(sys.argv[1], 'r')
rlen = int(sys.argv [2])
cov = int(sys.argv[3])
rnum = int(sys.argv[4])
rden = int(sys.argv[5])


name = infile.readline()
assembly = ""
for line in infile.readlines():
    if line[0] == '>':
        sys.stderr.write("Error, file containing multiple sequences!\n")
        exit(1)
    assembly += line[:-1]

togenerate = cov * len(assembly)

howmany = 0
while togenerate > 0:
    begin = random.randint(0, len(assembly) - rlen - 1)
    read = assembly[begin:begin+rlen]
    print(">Read_" + str(howmany))
    for x in read:
        if (random.randint(0, rden - 1) < rnum):
            print(random.choice(bases), end='')
        else:
            print(x, end='')
    
    print()
    howmany += 1
    togenerate -= rlen
