#! /usr/bin/env python3
"""
Demonstration of setup_gpio, input_gpio, output_gpio
"""
# 2023-09-08

from pi_sock import *

host = "localhost";
if len(sys.argv) > 1:
  host = sys.argv[1];   # Optional host address

socket_connect(host)

PinA = 17
PinB = 18

setup_gpio(12, 0, 0)
setup_gpio(PinB, 1, 0)   # Set GPIO 18 OUTPUT
print("GPIO 12", input_gpio(12))
print("GPIO 21", input_gpio(21))
output_gpio(18, 0)
# print("input_28", input_28())
print(f"bank 0 {input_28():08x}")

setup_gpio(4, 1, 0)
print(f"bank 0 {input_28():08x}")
# output_28( 1<<4, 1<<4)	# Set GPIO 4 (mask 16) to 1
# print()
# print(f"GPIO 4 Value={input_gpio(4)} Function {gpio_function(4)}")
