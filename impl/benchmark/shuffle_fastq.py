#!/usr/bin/env python3
import random

lines = []

try:
    while True:
        a=input()
        b = input()
        c=input()
        d=input()
        lines.append((a,b,c,d,))
except EOFError:
    permutation = [i for i in range(len(lines))]
    random.shuffle(permutation)
    for i in permutation:
        e,f,g,h = lines [i]
        print(e)
        print(f)
        print(g)
        print(h)
