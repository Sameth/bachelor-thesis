#!/usr/bin/env python3
import random

lines = []
try:
    while True:
        x = input()
        lines.append(x)
except EOFError:
    permutation = [i for i in range(len(lines))]
    random.shuffle(permutation)
    for i in permutation:
        print(lines [i])

