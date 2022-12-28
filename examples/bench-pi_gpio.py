#! /usr/bin/env python3
"""
This program toggles a GPIO pin and measures rate.
Using pi_gpio Python Interface
"""
# 2022-11-25

import pi_gpio as GPIO
import time
from signal import pause

GPIO.setup()

SigOUT = 12
LOOPS = 20000

GPIO.setup_gpio(SigOUT, 1, 0); # Set SigOUT OUTPUT

t0 = time.time()

for i in range(LOOPS):
    GPIO.output_gpio(SigOUT, 0)
    GPIO.output_gpio(SigOUT, 1)

t1 = time.time()

print("pi_gpio Python\t{:>10.0f} toggles per second".format((1.0 * LOOPS) / (t1 - t0)))

GPIO.cleanup()
