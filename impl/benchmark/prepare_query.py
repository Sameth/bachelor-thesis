#!/usr/bin/env python3
import random, sys
qlen = int(sys.argv[1]);

chars = "ACGT"
outputs = 0
try:
    while(True):
        seq = input();
        seq = input();
        x = random.randint(0, len(seq) - qlen - 1)
        print(seq[x:x+qlen])
        outputs += 1


except EOFError:
    for x in range(outputs//19):
        randomseq = ""
        for i in range(qlen):
            randomseq += random.choice(chars)
        print(randomseq)
