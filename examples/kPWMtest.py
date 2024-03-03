#! /usr/bin/env python3
# 2024-02-28

"""
This is a hardware PWM test program
	It generates 2 PWM outputs 
	PWM0 - 1kHz  with 75% duty cycle
	PWM1 - 2kHz with 1/3 duty cycle
	    after 10 seconds
	PWM1 - 2/3 duty cycle
	

--
The Pi has 2 independent hardware PWM channels.
	The same  PWM channel is available on multiple pins but the output is identical
	Channel 0 can be accessed on GPIO 12, 18, 40, 52
	Channel 1 can be accessed on GPIO 13, 19, 41, 45, 53
	
The kernel PWM service needs to be started before use.

To set up PWM on GPIO 18/19 
	dtoverlay=pwm-2chan in config.txt 
	sudo dtoverlay pwm-2chan on command line

To set up PWM on GPIO 12/13
	dtoverlay=pwm-2chan,pin=12,func=4,pin2=13,func2=4
	sudo dtoverlay pwm-2chan  pin=12 func=4 pin2=13 func2=4

There is a service pwm to set up a single channel.
"""

from kpwm import *
import sys
from time import sleep

kpwm_export(0)
kpwm_export(1)
time.sleep(0.1)

kpwm_start(0, 1000000, 750000)  
kpwm_start_f(1, 2000, 1/3)
time.sleep(10)
kpwm_set_duty_cycle(1, 2/3)
