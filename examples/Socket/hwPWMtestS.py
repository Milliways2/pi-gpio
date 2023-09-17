#! /usr/bin/env python3
"""
This is a hardware PWM test program
It generates 2 PWM outputs @10kHz with differing duty cycles
"""
# 2023-09-08

from pi_sock import *

host = "localhost";
if len(sys.argv) > 1:
  host = sys.argv[1];   # Optional host address

socket_connect(host)

# PWM0 = 12 # this is physical pin 32
# PWM1 = 13 # this is physical pin 33
PWM0 = 18                    # this is physical pin 12
PWM1 = 19                    # this is physical pin 35

# The following gives a precise 10kHz signal
rev = get_revision()
processor = rev>>12&0xF
if(processor == 3):
# Pi4 has 54MHz clock
    DIVIDER = 45;
    RANGE = 120;
else:
# Pi3 & earlier have 19.2MHz clock
    DIVIDER = 15;
    RANGE = 128;

pwmSetGpio(PWM0)
pwmSetGpio(PWM1)

pwmSetMode(PWM_MODE_MS) # use a fixed frequency
pwmSetClock(DIVIDER)  # gives a precise 10kHz signal

pwmSetRange(PWM0, RANGE)
# pwmWrite(PWM0, (RANGE // 4)) # duty cycle of 25%
pwmWrite(PWM0, (RANGE // 8)) # duty cycle of 12.5%

pwmSetRange(PWM1, RANGE)
pwmWrite(PWM1, (RANGE // 4)) # duty cycle of 25%

pwmSetDutycycle(PWM0, 0.6)

# GPIO.cleanup()
