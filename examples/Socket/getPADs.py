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

padstate = getPAD(0)
slew = (padstate >> 4) & 1
hyst = (padstate >> 3) & 1
drive = padstate & 7
print(f"padstate: {padstate:X}")
print(f"slew rate: {slew}, hysteresis: {hyst}, drive: {drive:d}")

