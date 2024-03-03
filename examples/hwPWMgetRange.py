#! /usr/bin/env python3
"""
This is a hardware PWM program
 2023-11-12
"""

import pi_gpio as GPIO
import ctypes
import time

# PWM0 = 12 # this is physical pin 32
# PWM1 = 13 # this is physical pin 33
PWM0 = 18                    # this is physical pin 12
PWM1 = 19                    # this is physical pin 35

GPIO.setup()

print(GPIO.pwmGetRange(PWM0))
print(GPIO.pwmGetRange(PWM1))

GPIO.cleanup()
