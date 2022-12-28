#! /usr/bin/env python3
"""
This is a hardware PWM test program
It generates 2 PWM outputs @10kHz with differing duty cycles

 2022-12-01
"""

import pi_gpio as GPIO
import ctypes
import time

PWM0 = 12 # this is physical pin 32
PWM1 = 13 # this is physical pin 33
# PWM0 = 18                    # this is physical pin 12
# PWM1 = 19                    # this is physical pin 35

GPIO.setup()

# The following gives a precise 10kHz signal
rev = GPIO.get_revision()
processor = rev>>12&0xF
if(processor == 3):
# Pi4 has 54MHz clock
    DIVIDER = 45;
    RANGE = 120;
else:
# Pi3 & earlier have 19.2MHz clock
    DIVIDER = 15;
    RANGE = 128;

GPIO.pwmSetGpio(PWM0)
GPIO.pwmSetGpio(PWM1)

GPIO.pwmSetMode(GPIO.PWM_MODE_MS) # use a fixed frequency
GPIO.pwmSetClock(DIVIDER)  # gives a precise 10kHz signal

GPIO.pwmSetRange(PWM0, RANGE)
# GPIO.pwmWrite(PWM0, (RANGE // 4)) # duty cycle of 25%
GPIO.pwmWrite(PWM0, (RANGE // 8)) # duty cycle of 12.5%

GPIO.cleanup()
