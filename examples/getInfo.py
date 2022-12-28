#! /usr/bin/env python3
"""
This program displays Raspberry Pi Information.
"""
# 2022-11-25

import pi_gpio as GPIO

InfoDict = GPIO.get_rpi_info()
print(InfoDict)
for k in InfoDict:
    print(k, ':\t', InfoDict[k])

