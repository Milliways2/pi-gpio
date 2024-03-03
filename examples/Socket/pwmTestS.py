#! /usr/bin/env python3
"""
Software PWM test
Slowly (over 10 seconds) ramp up brightness of LED then ramp down

Using pi_gpio Socket Interface
"""

# 2023-10-03
import sys
from pi_sock import *
from time import sleep

host = "localhost";
if len(sys.argv) > 1:
  host = sys.argv[1];   # Optional host address

socket_connect(host)

LEDpin = 12

# GPIO.setup()
# print("GPIO.VERSION = {}".format(GPIO.VERSION))

print(f"Ramp up brightness of LED connected to GPIO {LEDpin:d} then ramp down")

setup_gpio(LEDpin, 1, 0)   # Set LEDpin OUTPUT

pwm_set_duty_cycle(LEDpin, 0.0)
pwm_set_frequency(LEDpin, 100.0)
pwm_start(LEDpin);

if (pwm_exists(LEDpin)):
    print(f"pwm enabled on {LEDpin:d}");

for g in range(0, 100, 10):
    sleep(1)
    pwm_set_duty_cycle(LEDpin, float(g))
for g in range(100, -1, -10):
    sleep(1)
    pwm_set_duty_cycle(LEDpin, float(g))

sleep(1)
pwm_stop(LEDpin)
socket_disconnect()
