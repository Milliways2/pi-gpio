#! /usr/bin/env python3
# 2025-06-26
"""
Sample I²C  Example Program to read the Device ID & registers of the ADXL345.
"""

try:
    from pi_i2c import i2cOpen, i2cRead8, i2cWrite8
except:
    from pi_gpio import i2cOpen, i2cRead8, i2cWrite8   # compatibility with older pi_gpio

import sys

i2cBus = 1
i2cAddr = 0x53

# The ADXL345 Resisters required for this example
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
dataBuffer = []

def printval(ax):
    i = 0
    LINELEN=16
    for val in ax:
        print("{:02X}".format(val), end=' ')
        i += 1
        if i % LINELEN == 0:
            print()
    print()

def writeRegister(handle, reg, value) :
    """
    Write a single value to a register
    """
    i2cWrite8(handle, reg, value)

def readRegister(handle, reg) :
    """
    Read a single register
    """
    return i2cRead8(handle, reg)

# Read sequential registers into the buffer
def readRegisters(handle,  reg,  no_bytes):
    dataBuffer = [0 for x in range(no_bytes)]
    for i in range(0, no_bytes):
        dataBuffer[i] = i2cRead8(handle, reg+i)
    return dataBuffer

def convAccel(val) :
    return int.from_bytes(val, byteorder='little', signed=True)

ACC_CONV = float(2**10 // 4)    # acceleration conversion factor (10-bit resolution ±2g)

def readAccel(handle) :
    """
    Read data for each axis, convert to acceleration in g units.
    Returns tuple ax, ay, az
    """
    accel = readRegisters(handle, DATAX0, 6)    # read DATAXYZ registers
    datx = convAccel(accel[0:2])
    daty = convAccel(accel[2:4])
    datz = convAccel(accel[4:6])
    ax = datx / ACC_CONV
    ay = daty / ACC_CONV
    az = datz / ACC_CONV
   
    return (ax, ay, az)

handle = i2cOpen(i2cBus, i2cAddr)
if (handle < 0):
    sys.exit(handle) # Note return DOES NOT guarantee device exists at address!

writeRegister(handle, POWER_CTL, 0x08);	# take the ADXL345 out of power-saving mode

print("The DEVID is {:02X}".format(readRegister(handle, DEVID)))    # Detect device exists at address
print("The full set of registers are:")
printval(readRegisters(handle, DEVID, BUFFER_SIZE) )    # read all registers

# printval(readRegisters(handle, DATAX0, 6) )    # read DATAXYZ registers
ax, ay, az =readAccel(handle)    # read acceleration
print(f"X={ax:.2f}g Y={ay:.2f}g Z={az:.2f}g")
