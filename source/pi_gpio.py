#! /usr/bin/env python3
# 2022-12-22
# 2023-07-31
# 2023-09-07
"""
pi_gpio is a Python module for the RaspberryPi
which uses the pi-gpio library to control the GPIO

OVERVIEW
  *Essential*
    setup - Initialise pi-gpio

    cleanup - Clean up by releasing memory allocated by setup

  *Information*
    get_rpi_info - Raspberry Pi Information

    get_revision - Raspberry Pi Revision Code

  *GPIO*
    setup_gpio - Set gpio as an input or an output

    input_gpio - Returns the GPIO level__plibrary

    output_gpio - Output to a GPIO channel

    input_28 - Returns value of GPIO 0-27

    output_28 - Sets value of GPIO 0-27

    gpio_function - The current GPIO mode

    get_pullupdn - The current GPIO pull/up down (Pi4 only)

  *Software PWM*
    pwm_set_duty_cycle - Change the duty cycle

    pwm_set_frequency - Change the frequency

    pwm_start - Start software PWM

    pwm_stop - Stop software PWM

  *PAD*
    NOTE You need to be running as root to use these functions

    getPAD - Return the current PAD settings

    setPAD - Set the PAD parameters

  *Hardware PWM*
    NOTE You need to be running as root to use these functions

    pwmSetGpio - Put gpio pin into PWM mode

    pwmSetMode - Select the native “balanced” mode or standard mark:space mode

    pwmSetRange - Set the PWM range

    pwmWrite - Set the PWM mark value (duty cycle = mark/range) 

    pwmSetDutycycle - Set the duty cycle
    
    pwmGetRange - Get the PWM range

    pwmSetClock - Set/Change the PWM clock


  *I²C*
    I²C MUST be enabled BEFORE using this code (see i2cOpen for detail)

    i2cOpen - Open I²C device

    i2cRead - Read a single word (8 bits) from a device, without specifying a register

    i2cRead8 - Read a single word (8 bits) from register

    i2cRead16 - Read a single word (16 bits) from register

    i2cWrite - Write a single word (8 bits) to a device, without specifying a register

    i2cWrite8 - Write a single word (8 bits) to the specified register

    i2cWrite16 - Write a single word (16 bits) to the specified register

  *SPI*
    SPI MUST be enabled BEFORE using this code (see spiOpen for detail)

    spiOpen - Initialise a SPI channel

    spiDataRW2 - Write and Read a block of data over the SPI bus

    spiDataRW - Write and Read a block of data over the SPI bus
"""

VERSION='0.6'

import ctypes
__plibrary = ctypes.CDLL('libpi-gpio.so')

# Defining free function
# Free memory allocated by c
__free_mem = __plibrary.free_memory
__free_mem.argtypes = [ctypes.POINTER(ctypes.c_char)]

#  Essential
def setup():
  """
  Initialise pi-gpio
  Must be called before any other function except Information, I²C or SPI functions
  """
  return __plibrary.setup()

def cleanup():
  """
  Clean up by releasing memory allocated by setup
  NOTE Does not reset GPIO that have been used
  """
  __plibrary.cleanup()

# Information
def get_revision():
  """
  Get Raspberry Pi Revision Code
  """
  return __plibrary.get_revision()

def get_rpi_info():
  """
  Raspberry Pi Information
  Return rpi_info as a dictionary
  """
  info = ctypes.c_char_p(__plibrary.get_rpi_dict())
  cstring = info.value.decode()
  Dict = eval(cstring)
  __free_mem(info)
  return Dict

#  GPIO
#    NOTE all gpio use Broadcom BCM numbers

INPUT=0
OUTPUT=1

def setup_gpio(gpio, direction, pud):
# def setup_gpio(gpio, direction, pud=0):  # optional pud
  """
  Set gpio as an input or an output
  direction: 0=IN, 1=OUT
  pud: 0=None 1=Up 2=Down
  """
  __plibrary.setup_gpio(gpio, direction, pud)

def input_gpio(gpio):
  """
  Input from a GPIO channel.
  Returns HIGH=1=True or LOW=0=False
  """
  return __plibrary.input_gpio(gpio)

def output_gpio(gpio, value):
  """
  Output to a GPIO channel.
  value - 0/1 or False/True or LOW/HIGH
  """
  __plibrary.output_gpio(gpio, value)

def gpio_function(gpio):
  """
  Returns the current GPIO mode
  Returns 0-7 (IN, OUT, ALT5, ALT4, ALT0, ALT1, ALT2, ALT3)
  """
  return __plibrary.gpio_function(gpio)

def get_pullupdn(gpio):
  """
  Return the current GPIO pull
  Returns
  0:None/Unknown
  1:Up (Pi4 only)
  2:Down (Pi4 only)
  """
  return __plibrary.get_pullupdn(gpio)

def input_28():
  """
  Returns value of GPIO 0-27
  """
  return __plibrary.input_28()

def output_28(bits, mask):
  """
  Sets value of GPIO 0-27
  bits: 28 bit values to set; each bit 0/1
  mask: 28 bit mask specifying GPIO to set
  """
  __plibrary.output_28()

#  PAD
#    NOTE You need to be running as root to use these functions

def getPAD(group):
  """
  Return the current PAD settings (slew, hyst, drive)
  group: - 0-2
  Returns
  padstate: - 0-0x1F
  slew = (padstate >> 4) & 1
  hyst = (padstate >> 3) & 1
  drive = padstate & 7
  """
  return __plibrary.getPAD(group)

def setPAD(group, padstate):
  """
  Set the PAD (slew, hyst, drive)
  group: - 0-2
  padstate: - 0-0xF : padstate = slew << 4 | hyst << 3 | drive
  """
  return __plibrary.setPAD(group, padstate)

# Hardware PWM
#    NOTE You need to be running as root to use these functions

PWM_MODE_MS=0
PWM_MODE_BAL=1

def pwmSetGpio(gpio):
  """
  Put gpio pin into PWM mode
  The Pi has 2 independent hardware PWM channels, clocked at a fixed frequency
  The same PWM channel is available on multiple pins but the output is identical
  Channel 0 can be accessed on GPIO 12, 18, 40, 52
  Channel 1 can be accessed on GPIO 13, 19, 41, 45, 53
  Return 0 if successful
  """
  return __plibrary.pwmSetGpio(gpio)

def pwmSetMode(mode):
  """
  Select the native “balanced” mode or standard mark:space mode
  mode - 0 PWM_MODE_MS or 1 PWM_MODE_BAL
  """
  __plibrary.pwmSetMode(mode)

def pwmSetRange(gpio, range):
  """
  Set the PWM range register
  range:
  In Mark:Space mode the output is HIGH for Mark time slots and LOW for Range-Mark
  The output is thus a fixed frequency; PWM frequency = PWM clock / range
  Set initial duty cycle to 50%
  Return 0 if successful
  """
  return __plibrary.pwmSetRange(gpio, range)

def pwmWrite(gpio, value):
  """
  Set the duty cycle mark/range
  value: - 0-RANGE
  Return 0 if successful
  """
  return __plibrary.pwmWrite(gpio, value)
 
def pwmSetDutycycle(gpio, duty_cycle):
  """
  Set the duty cycle
  duty_cycle: - 0.0-1.0
  Return 0 if successful
  """
  return __plibrary.pwmSetDutycycle(gpio, ctypes.c_float(duty_cycle))

def pwmGetRange(gpio):
  """
  Get the range
  """
  return __plibrary.pwmGetRange(gpio)

def pwmSetClock(divisor):
  """
  Set/Change the PWM clock
  divisor - 1-4095
  Both channels share a common clock, which is Osc / divisor
  Osc is 19.2 MHz on most Pi models
  Osc is 54M MHz on most Pi4 (BCM2711)
  """
  __plibrary.pwmSetClock(divisor)

#  I²C
#   I²C MUST be enabled BEFORE using this code (see i2cOpen for detail)

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

#   SPI

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
  return __plibrary.spiDataRW2( channel, tx_data, rx_data, len)

def spiDataRW ( channel, data, len):
  """
  Write and Read a block of data over the SPI bus
      Shared Tx/Rx buffer
  channel: - 0-3 (depending on available chip selects)
  data: pointer to buffer containing transmit data (will be overwritten by receive data)
  len: length of buffer

  Returns result code
  """
  return __plibrary.spiDataRW ( channel, data, len)

#   Software PWM

def pwm_set_duty_cycle(gpio, dutycycle):
  """
  Change the duty cycle
  dutycycle: - between 0.0 and 100.0
  """
  __plibrary.pwm_set_duty_cycle(gpio, ctypes.c_float(dutycycle))

def pwm_set_frequency(gpio, freq):
  """
  Change the frequency
  frequency: - frequency in Hz (freq > 1.0)
  """
  __plibrary.pwm_set_frequency(gpio, ctypes.c_float(freq))

def pwm_start(gpio):
  """
  Start software PWM
  """
  __plibrary.pwm_start(gpio)

def pwm_stop(gpio):
  """
  Stop software PWM
  MUST be called before exit
  Include delay BEFORE calling cleanup()
  """
  __plibrary.pwm_stop(gpio)
