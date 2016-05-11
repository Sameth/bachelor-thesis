#!/usr/bin/env python3
import random, sys
qlen = int(sys.argv[1]);

chars = "ACGT"
try:
    while(True):
        seq = input();
        seq = input();
        x = random.randint(0, len(seq) - qlen - 1)
        print(seq[x:x+qlen])
        randomseq = ""
        for i in range(qlen):
            randomseq += random.choice(chars)
#        print(randomseq)


except EOFError:
    pass
