#! /usr/bin/env python3
# 2022-11-25

import pi_gpio as GPIO

GPIO.setup()

PinA = 17
PinB = 18

print("Set GPIO: {} INPUT; GPIO: {} OUTPUT,HIGH".format(PinA, PinB))

GPIO.setup_gpio(PinA, 0, 0) # Set PinA INPUT no PU
GPIO.setup_gpio(PinB, 1, 0); # Set GPIO 18 OUTPUT
GPIO.output_gpio(PinB, 1) # Set PinB HIGH

GPIO.cleanup()
