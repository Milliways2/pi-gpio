#! /usr/bin/env python3
# 2022-12-21

import pi_gpio as GPIO
import sys
from time import sleep

"""
Software PWM test
Slowly (over 10 seconds) ramp up brightness of LED then ramp down
"""

LEDpin = 12

GPIO.setup()
print("GPIO.VERSION = {}".format(GPIO.VERSION))

print("Ramp up brightness of LED connected to GPIO {:d} then ramp down".format(LEDpin))

GPIO.setup_gpio(LEDpin, GPIO.OUTPUT, 0)

GPIO.pwm_set_duty_cycle(LEDpin, 0.0)
GPIO.pwm_set_frequency(LEDpin, 100.0)
GPIO.pwm_start(LEDpin);

for g in range(0, 100, 10):
    sleep(1)
    GPIO.pwm_set_duty_cycle(LEDpin, float(g))
for g in range(100, 0, -10):
    sleep(1)
    GPIO.pwm_set_duty_cycle(LEDpin, float(g))


#   if (pwm_exists(LEDpin))
#     printf("pwm enabled on %i\n", LEDpin);

GPIO.pwm_stop(LEDpin)
# sleep(1)
# print("cleanup()")
sleep(3)
GPIO.cleanup()
sleep(1)
print("Finished")
