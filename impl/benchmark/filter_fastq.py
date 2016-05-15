#!/usr/bin/env python3

try:
    while(True):
        a = input()
        b = input()
        c = input()
        d = input()
        if (b.count('N') == 0):
            print(a)
            print(b)
            print(c)
            print(d)

except EOFError:
    pass
