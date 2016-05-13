#!/usr/bin/env python3
import sys

genome_size = int(sys.argv[1])
target_coverage = int(sys.argv[2])

target = genome_size * target_coverage;

try:
    while target > 0:
        print(input())
        x = input()
        target -= len(x)
        print(x)
except EOFError:
    pass
