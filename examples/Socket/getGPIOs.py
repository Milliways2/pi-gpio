#! /usr/bin/env python3
"""
Get values of BANK0 (GPIO0…27)
"""
# 2023-09-08

from pi_sock import *

host = "localhost";
if len(sys.argv) > 1:
  host = sys.argv[1];   # Optional host address

socket_connect(host)

print(f"bank 0 {input_28():08x}")

for g in range(0, 28):
    v = int(input_gpio(g))
    if(v):
        print(f"GPIO {g:2d} Value={v} Function {gpio_function(g)}")
    else:
        print(f"GPIO {g:2d} Value={v} Function {gpio_function(g)} Pull {get_pullupdn(g)}")

