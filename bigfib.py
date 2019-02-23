#!/usr/bin/python3

from sys import argv

n = int(argv[1])

b1 = 1
b2 = 1
for i in range(1, n):
    b = b2 + b1
    b1 = b2
    b2 = b
print("{:x}".format(b2))
