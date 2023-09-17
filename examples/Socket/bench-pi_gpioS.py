#! /usr/bin/env python3
"""
This program toggles a GPIO pin and measures rate.
Using pi_gpio Socket Interface
"""
# 2023-09-08

from pi_sock import *

host = "localhost";
if len(sys.argv) > 1:
  host = sys.argv[1];   # Optional host address

socket_connect(host)

import time

SigOUT = 12
LOOPS = 2000

setup_gpio(SigOUT, 1, 0)   # Set SigOUT OUTPUT

t0 = time.time()

for i in range(LOOPS):
    output_gpio(SigOUT, 0)
    output_gpio(SigOUT, 1)

t1 = time.time()

print(f"pi_gpio Socket\t{float(LOOPS)/(t1-t0):>10.0f} toggles per second")
