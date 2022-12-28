#! /usr/bin/env python3
# 2022-11-25

import pi_gpio as GPIO

GPIO.setup()

print('GPIO {:08x}'.format(GPIO.input_28()))
for g in range(0, 28):
    v = GPIO.input_gpio(g)
    if(v):
        print("GPIO {:2d} Value={} Function {}".format(g, v, GPIO.gpio_function(g),))
    else:
        print("GPIO {:2d} Value={} Function {} Pull {}".format(g, v, GPIO.gpio_function(g), GPIO.get_pullupdn(g)))

GPIO.cleanup()
