#! /usr/bin/env python3
# 2025-06-12
"""
pi_spi is a Python SPI module for the RaspberryPi using the spidev kernel driver.

OVERVIEW

  *SPI*
    SPI MUST be enabled BEFORE using this code (see spiOpen for detail)

    spiOpen - Initialise a SPI channel

    spiDataRW2 - Write and Read a block of data over the SPI bus

    spiDataRW - Write and Read a block of data over the SPI bus
"""

VERSION='1.0'

import ctypes
try:
    __plibrary = ctypes.CDLL('libpi-spi.so')
except:
    __plibrary = ctypes.CDLL('libpi-gpio.so')   // for old library

def spiOpen( controller,  channel,  speed,  mode):
  """
  Initialise a SPI channel
      Raspberry Pi Zero, 1, 2 and 3 have three SPI controllers
  SPI0, with 2 hardware chip selects, is available on all Raspberry Pis
  To enable SPI0 use raspi-config, or ensure the line dtparam=spi=on is not commented out in /boot/config.txt
  By default it uses 2 chip select lines, but this can be reduced to 1 using dtoverlay=spi0-1cs
  SPI1, with 3 hardware chip selects, is available on all Raspberry Pis with 40 pin header mode 1,3 do not work!
  To enable SPI1 with 1, 2 or 3 chip select lines add to the /boot/config.txt file
  dtoverlay=spi1-1cs #1 chip select
  dtoverlay=spi1-2cs #2 chip select
  dtoverlay=spi1-3cs #3 chip select
  SPI2, also with 3 hardware chip selects, is only available on Compute Module 1, 3 and 3+
  On the Raspberry Pi 4, 400 and Compute Module 4 there are 4 additional SPI buses:
  SPI3 to SPI6, each with 2 hardware chip selects

  controller: - 0-6 (depending on model)
  channel: - 0-3 (depending on available chip selects)
  speed: - 500,000 through 32,000,000 - SPI clock speed in bps
  mode: - 0-3

  Returns file-descriptor for the device, or -n on error
  """
  return __plibrary.spiOpen( controller,  channel,  speed,  mode)

def spiDataRW2( channel, tx_data, rx_data, len):
  """
  spiDataRW2( channel, tx_data, rx_data, len)
  Write and Read a block of data over the SPI bus
      Discrete Tx/Rx buffers
  channel: - 0-3 (depending on available chip selects)
  tx_data: pointer to buffer containing transmit data
  rx_data: pointer to buffer to receive data
  len: length of buffer

  Returns result code
  """
  return __plibrary.spiDataRW2(channel, ctypes.c_char_p(tx_data), ctypes.c_char_p(rx_data), len)

def spiDataRW ( channel, data, len):
  """
  Write and Read a block of data over the SPI bus
      Shared Tx/Rx buffer
  channel: - 0-3 (depending on available chip selects)
  data: pointer to buffer containing transmit data (will be overwritten by receive data)
  len: length of buffer

  Returns result code
  """
  return __plibrary.spiDataRW ( channel, ctypes.c_char_p(bytes(data)), len)

