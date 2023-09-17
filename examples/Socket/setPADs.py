#! /usr/bin/env python3
"""
Return the current PAD settings
"""

# 2023-09-08

from pi_sock import *

host = "localhost";
if len(sys.argv) > 1:
  host = sys.argv[1];   # Optional host address

socket_connect(host)

slew = 1
hyst = 1
drive = 7
setPAD(0, slew<<4 | hyst<<3 | drive)
print(f"padstate: {getPAD(0):X}")

