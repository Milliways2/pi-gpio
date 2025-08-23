#! /usr/bin/env python3
# 2025-06-12
"""
pi_i2c is an I2C Python module for the RaspberryPi using the I2C kernel driver.
I2C MUST be enabled BEFORE using this code (see i2cOpen for detail).

"""
VERSION='1.0'

import ctypes
try:
    __plibrary = ctypes.CDLL('libpi-i2c.so')
except:
    __plibrary = ctypes.CDLL('libpi-gpio.so')   // for old library

def i2cOpen(i2cBus,  i2cAddr):
  """
  Open I²C device
  To enable I²C use raspi-config
  or ensure the line dtparam=i2c_arm=on or dtparam=i2c=on is not commented out in /boot/config.txt
  I²C can be enabled on the fly with a command e.g. sudo dtparam i2c
  NOTE this will NOT return an error if there is no I²C device at i2cAddr
  Use i2cRead etc.  to check

  i2cBus: - 0-1
  i2cAddr: - 0-0x7F

  Returns handle to the I²C device, or -1 on error
  """
  return __plibrary.i2cOpen(i2cBus,  i2cAddr)

def i2cRead(handle):
  """
  Read a single word from a device, without specifying a register
  handle: - handle to the I²C device

  Returns word read

  """
  return __plibrary.i2cRead(handle)

def i2cRead8( handle,  i2cReg) :
  """
  Read a single word (8 bits) from register
  handle: - handle to the I²C device
  i2cReg: - I²C register

  Returns word read
  """
  return __plibrary.i2cRead8( handle,  i2cReg)

def i2cRead16( handle,  i2cReg):
  """
  Read a single word (16 bits) from register
  handle: - handle to the I²C device
  i2cReg: - I²C register

  Returns Word read
  """
  return __plibrary.i2cRead16( handle,  i2cReg)

def i2cWrite( handle,  data):
  """
  Write a single word (8 bits) to a device, without specifying a register
  handle: - handle to the I²C device
  data: - byte to write

  Returns result code
  """
  return __plibrary.i2cWrite( handle,  data)

def i2cWrite8( handle,  i2cReg,  value):
  """
  Write a single word (8 bits) to the specified register
  handle: - handle to the I²C device
  i2cReg: - I²C register
  data: - word to write

  Returns result code
  """
  return __plibrary.i2cWrite8( handle,  i2cReg,  value)

def i2cWrite16( handle,  reg,  value):
  """
  Write a single word (16 bits) to the specified register
  handle: - handle to the I²C device
  i2cReg: - I²C register
  data: - word to write

  Returns result code
  """
  return __plibrary.i2cWrite16( handle,  reg,  value)

