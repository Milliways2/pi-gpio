#! /usr/bin/env python3
"""
Using pi_gpio Socket Interface
"""

# 2023-10-09

import sys
from pi_sock import *

host = "localhost";
if len(sys.argv) > 1:
  host = sys.argv[1];   # Optional host address

socket_connect(host)

print("spiDataRW test ", spiDataRW(1, bytes("Hello World", "ascii"), 9))
