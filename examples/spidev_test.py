#! /usr/bin/env python3
# 2023-10-05
"""
"""

# import pi_gpio as GPIO
import sys

def printval(ax):
    i = 0
    for val in ax:
        print("{:02X} ".format(val), end=' ')
        i += 1
        if i % 6 == 0:
            print()
    print()

# controller = 0  #SPI0
# channel = 0     #.0
# mode = 0
# bits = 8
# speed = 1000000
# 
# ret = GPIO.spiOpen( controller,  channel,  speed,  mode) #SPI controller, channel, speed, mode
# if (ret < 0) :
#     print("Unable to open /dev/spidev /dev/spidev{:d}.{:d}".format(controller, channel))
#     exit(-1)

# Block of data that is defined in the tx[] array
# tx = [0x37, 0xF0]
tx = [0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x40, 0x00, 0x00, 0x00, 0x00, 0x95,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD,
0xF0, 0x0D]

print("Connect the SPI_MOSI and SPI_MISO pins together.")
print("The output should be settings followed by:")
printval(tx)

print()

# print("Device: \t/dev/spidev{:d}.{:d}".format(controller, channel))
# print("spi mode:\t{:d}".format(mode))
# print("bits per word:\t{:d}".format(bits))
# print("max speed:\t{:d} Hz ({:d}KHz)".format(speed, speed // 1000))

Btx = bytes(tx)
# Brx = bytes([0]*len(Btx))   # create empty bytes to receive data
# 
# GPIO.spiDataRW2 ( channel, Btx, Brx, len(Btx))
printval(Btx)
