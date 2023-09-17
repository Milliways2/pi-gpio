#! /usr/bin/env python3
"""
Raspberry Pi Revision Code
"""
# 2023-09-08

from pi_sock import *

host = "localhost";
if len(sys.argv) > 1:
  host = sys.argv[1];   # Optional host address

socket_connect(host)

rev = get_revision()

print(f"revision {rev:08x}")

processor = rev>>12&0xF
print(f"processor {processor}")
