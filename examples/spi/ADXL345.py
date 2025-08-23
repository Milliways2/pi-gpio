#! /usr/bin/env python3
# 2025-05-06
"""
Sample SPI Example Program to read the Device ID & registers of the ADXL345.
"""

from pi_gpio import *
import sys


controller = 0  #SPI0
channel = 0     #.0
mode = 3
bits = 8
speed = 1000000

# The ADXL345 Resisters required for this example
# To read or write multiple bytes in a single transmission, the multiple-byte bit,
# located after the R/W bit in the first byte transfer must be set
READBIT = 0x80	# 1 to READ
MULTIBYTE = 0x40

DEVID = 0x00 # (Read/Write)
POWER_CTL = 0x2D
DATA_FORMAT = 0x31
DATAX0 = 0x32
DATAX1 = 0x33
DATAY0 = 0x34
DATAY1 = 0x35
DATAZ0 = 0x36
DATAZ1 = 0x37

BUFFER_SIZE = 0x40

def printval(ax):
    i = 0
    LINELEN=16
    for val in ax:
        print("{:02X}".format(val), end=' ')
        i += 1
        if i % LINELEN == 0:
            print()
#     if(i % LINELEN == 0): print()
    print()

def writeRegister(channel, reg, value) :
    """
    Write a single value to a register
    """
    Btx = bytes([reg]+[0]) # transmit buffer
    return spiDataRW(channel, Btx, len(Btx))

def readRegister(SPIchannel, reg) :
    """
    Read a single register
    """
    Btx = bytes([reg | READBIT]+[0]) # transmit buffer
    Brx = bytes([0]*2)   # create empty bytes to receive data
    spiDataRW2(channel, Btx, Brx, len(Btx))
    return Brx[1]

def readRegisters(SPIchannel, reg, no_bytes) :
    """
    Read sequential registers
    """
    Btx = bytes([reg | READBIT | MULTIBYTE]+[0]*no_bytes) # transmit buffer
    Brx = bytes([0]*len(Btx))   # create empty bytes to receive data
    spiDataRW2(channel, Btx, Brx, len(Btx))
    return Brx[1:]

def convAccel(val) :
    return int.from_bytes(val, byteorder='little', signed=True)

ACC_CONV = float(2**10 // 4)    # acceleration conversion factor (10-bit resolution ±2g) 

def readAccel(channel) :
    """
    Read data for each axis, convert to acceleration in g units.
    Returns tuple ax, ay, az
    """
    accel = readRegisters(channel, DATAX0, 6)
    datx = convAccel(accel[0:2])
    daty = convAccel(accel[2:4])
    datz = convAccel(accel[4:6])
    ax = datx / ACC_CONV
    ay = daty / ACC_CONV
    az = datz / ACC_CONV
   
    return (ax, ay, az)

ret = spiOpen(controller,  channel,  speed,  mode) #SPI controller, channel, speed, mode
if (ret < 0) :
    print("Unable to open /dev/spidev /dev/spidev{:d}.{:d}".format(controller, channel))
    exit(-1)

print("Device: \t/dev/spidev{:d}.{:d}".format(controller, channel))
print("spi mode:\t{:d}".format(mode))
print("bits per word:\t{:d}".format(bits))
print("max speed:\t{:d} Hz ({:d}KHz)".format(speed, speed//1000))

writeRegister(channel, POWER_CTL, 0x08);	# take the ADXL345 out of power-saving mode
print("The DEVID is {:02X}".format(readRegister(channel, DEVID)))

print("The full set of registers are:")
printval(readRegisters(channel, DEVID, BUFFER_SIZE) )    # read all registers

printval(readRegisters(channel, DATAX0, 6) )    # read DATAXYZ registers
ax, ay, az =readAccel(channel)
print(f"X={ax:.2f}g Y={ay:.2f}g Z={az:.2f}g")

# printval(writeRegister(channel, DATA_FORMAT, 0x00))

