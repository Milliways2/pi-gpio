#! /usr/bin/env python3
# 2022-11-25

import pi_gpio as GPIO

GPIO.setup()

padstate = GPIO.getPAD(0);
slew = (padstate >> 4) & 1
hyst = (padstate >> 3) & 1
drive = padstate & 7
print('{:x}'.format(padstate))
print("slew rate: {}, hysteresis: {}, drive: {:d}".format(slew, hyst, drive))

GPIO.cleanup()
