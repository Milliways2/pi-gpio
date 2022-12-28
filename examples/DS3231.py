#! /usr/bin/env python3
# 2022-12-17

import pi_gpio as GPIO
import sys

"""
Display status of DS3231 RTC - assumes time in UTC 24 hour mode
"""

i2cBus = 1
i2cAddr = 0x68

def bcdToDec(b):
    return (b // 16) * 10 + (b % 16)

# The time and calendar registers are BCD
SECONDS = 0x00
MINUTES = 0x01
HOURS = 0x02
DAY = 0x03
DATE = 0x04
MONTH = 0x05
YEAR = 0x06
A1M1 = 0x07
A1M2 = 0x08
A1M3 = 0x09
A1M4 = 0x0A
A2M2 = 0x0B
A2M3 = 0x0C
A2M4 = 0x0D
CONTROL = 0x0E
TEMP = 0x11
TEMPFRAC = 0x12

handle = GPIO.i2cOpen(i2cBus, i2cAddr)
if (handle < 0):
    sys.exit(handle) # Note return DOES NOT guarantee device exists at address!

secs = GPIO.i2cRead8(handle, SECONDS)    # Detect device exists at address
if (secs < 0):
    print("No device at address {:02x}".format(secs))
mins = bcdToDec(GPIO.i2cRead8(handle, MINUTES))
hours = bcdToDec(GPIO.i2cRead8(handle, HOURS))
days = bcdToDec(GPIO.i2cRead8(handle, DATE))
months = bcdToDec(GPIO.i2cRead8(handle, MONTH))
years = bcdToDec(GPIO.i2cRead8(handle, YEAR))

print("The RTC time is  {:2d}:{:02d}:{:02d}".format(hours, mins, secs))
print("The RTC date is  {:2d}-{:02d}-{:02d}".format(years, months, days))

temperature = GPIO.i2cRead8(handle, TEMP) + ((GPIO.i2cRead8(handle, TEMPFRAC) >> 6) * 0.25)
print("The temperature is  {}°C".format(temperature))
