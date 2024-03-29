#! /usr/bin/env python3
# 2023-10-09
"""
pi-sock is a client program to send socket commands to a Pi.
The target Pi must have a running pi-gpiod socket server to receive and act on messages.

OVERVIEW
  *Essential*

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

  *PAD*
    getPAD - Return the current PAD settings

    setPAD - Set the PAD parameters

  *Hardware PWM*
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

  *Software PWM*
    pwm_set_duty_cycle - Change the duty cycle

    pwm_set_frequency - Change the frequency

    pwm_start - Start software PWM

    pwm_stop - Stop software PWM
    
    pwm_exists - Check if there is a PWM for this gpio
"""
"""
  *SPI*
    SPI MUST be enabled BEFORE using this code (see spiOpen for detail)

    spiOpen - Initialise a SPI channel

    spiDataRW2 - Write and Read a block of data over the SPI bus

    spiDataRW - Write and Read a block of data over the SPI bus
"""

import sys, os, time
import socket, base64

HOST = "localhost"  
PORT = 31425  

class SOCKCLIENT:
  """
  Construct and return a new SOCKCLIENT object connected to host:port
    host: string representation of IP Address
    port: default 31425
  """
  def __init__(self, host=HOST, port=PORT):
    self.host = host
    self.port = port
    self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    self.sock.connect((self.host, self.port))
    
  def close(self):
    self.sock.close()

  def send2pi(self, cmd, *pargs):
    """
    Send cmd & parameters to socket server & receive result
    """
    # convert cmd & (optional) parameters (of any type) to bytes
    command = (cmd + ' ' + ' '.join(tuple((str(p)) for p in pargs))).encode('ascii')
    self.sock.sendall(command)
    res = self.sock.recv(127)   # return value is null terminated bitstring   
    return res[:-1].decode()


def socket_connect(host=HOST):
  """
  Connect to socket server on host
  host: default localhost
  """
  global s
  s = SOCKCLIENT(host)

def socket_disconnect():
  s.close()

#   Information
def get_revision():
  """
  Get Raspberry Pi Revision Code
  """
  return int(s.send2pi('get_revision'))

def get_rpi_info():
  """
  Raspberry Pi Information
  Return rpi_info as a dictionary
  """
  info = s.send2pi('get_rpi_info')
  Dict = eval(info)
  return Dict

#  GPIO
#    NOTE all gpio use Broadcom BCM numbers

INPUT=0
OUTPUT=1

def setup_gpio(gpio, direction, pud):
# def setup_gpio(gpio, direction, pud=0):    # optional pud
  """
  Set gpio as an input or an output
  direction: 0=IN, 1=OUT
  pud: 0=None 1=Up 2=Down
  """
  s.send2pi('setup_gpio', gpio, direction, pud)

def input_gpio(gpio):
  """
  Input from a GPIO channel.
  Returns HIGH=1=True or LOW=0=False
  """
  return s.send2pi('input_gpio', gpio)

def output_gpio(gpio, value):
  """
  Output to a GPIO channel.
  value - 0/1 or False/True or LOW/HIGH
  """
  s.send2pi('output_gpio', gpio, value)

def gpio_function(gpio):
  """
  Returns the current GPIO mode
    0-7 (IN, OUT, ALT5, ALT4, ALT0, ALT1, ALT2, ALT3)
  """
  return int(s.send2pi('gpio_function', gpio))

def get_pullupdn(gpio):
  """
  Return the current GPIO pull
    0:None/Unknown
    1:Up (Pi4 only)
    2:Down (Pi4 only)
  """
  return int(s.send2pi('get_pullupdn', gpio))
 
def input_28():
  """
  Returns value of GPIO 0-27
  """
  return int(s.send2pi('input_28'))

def output_28(bits, mask):
  """
  Sets value of GPIO 0-27
  bits: 28 bit values to set; each bit 0/1
  mask: 28 bit mask specifying GPIO to set
  """
  s.send2pi('output_28', bits, mask)

#    PAD
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
  return int(s.send2pi('getPAD'), group)

def setPAD(group, padstate):
  """
  Set the PAD (slew, hyst, drive)
  group: - 0-2
  padstate: - 0-0xF : padstate = slew << 4 | hyst << 3 | drive
  """
  s.send2pi('setPAD', group, padstate)

#   Hardware PWM

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
  s.send2pi('pwmSetGpio', gpio)

def pwmSetMode(mode):
  """
  Select the native “balanced” mode or standard mark:space mode
  mode - 0 PWM_MODE_MS or 1 PWM_MODE_BAL
  """
  s.send2pi('pwmSetMode', mode)

def pwmSetRange(gpio, range):
  """
  Set the PWM range register
  range:
    In Mark:Space mode the output is HIGH for Mark time slots and LOW for Range-Mark
    The output is thus a fixed frequency; PWM frequency = PWM clock / range
    Set initial duty cycle to 50%
  Return 0 if successful
  """
  s.send2pi('pwmSetRange', gpio, range)#     return __plibrary.pwmSetRange(gpio, range)

def pwmWrite(gpio, value):
  """
  Set the duty cycle mark/range
  value: - 0-RANGE
  Return 0 if successful
  """
  s.send2pi('pwmWrite', gpio, value)
 
def pwmSetDutycycle(gpio, duty_cycle):
  """
  Set the duty cycle
  duty_cycle: - 0.0-1.0
  Return 0 if successful
  """
  s.send2pi('pwmSetDutycycle', gpio, duty_cycle)

def pwmGetRange(gpio):
  """
  Get the range
  """
  return s.send2pi('pwmGetRange', gpio)

def pwmSetClock(divisor):
  """
  Set/Change the PWM clock
  divisor - 1-4095
    Both channels share a common clock, which is Osc / divisor
    Osc is 19.2 MHz on most Pi models
    Osc is 54M MHz on most Pi4 (BCM2711)
  """
  s.send2pi('pwmSetClock', divisor)

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
  return int(s.send2pi('i2cOpen', i2cBus, i2cAddr))

def i2cRead(handle):
  """
  Read a single word from a device, without specifying a register
  handle: - handle to the I²C device

  Returns word read
  """
  return int(s.send2pi('i2cRead', handle))

def i2cRead8( handle, i2cReg):
  """
  Read a single word (8 bits) from register
  handle: - handle to the I²C device
  i2cReg: - I²C register

  Returns word read
  """
  return int(s.send2pi('i2cRead8', handle, i2cReg))

def i2cRead16(handle, i2cReg):
  """
  Read a single word (16 bits) from register
  handle: - handle to the I²C device
  i2cReg: - I²C register

  Returns Word read
  """
  return int(s.send2pi('i2cRead16', handle, i2cReg))

def i2cWrite(handle, data):
  """
  Write a single word (8 bits) to a device, without specifying a register
  handle: - handle to the I²C device
  data: - byte to write

  Returns result code
  """
  return int(s.send2pi('i2cWrite', handle, data))

def i2cWrite8(handle, i2cReg, value):
  """
  Write a single word (8 bits) to the specified register
  handle: - handle to the I²C device
  i2cReg: - I²C register
  data: - word to write

  Returns result code
  """
  return int(s.send2pi('i2cWrite8', handle,  i2cReg,  value))

def i2cWrite16(handle, i2cReg, value):
  """
  Write a single word (16 bits) to the specified register
  handle: - handle to the I²C device
  i2cReg: - I²C register
  data: - word to write

  Returns result code
  """
  return int(s.send2pi('i2cWrite16', handle, i2cReg, value))

#   Software PWM

def pwm_set_duty_cycle(gpio, dutycycle):
  """
  Change the duty cycle
  dutycycle: - between 0.0 and 100.0
  """
  s.send2pi('pwm_set_duty_cycle', gpio, dutycycle)

def pwm_set_frequency(gpio, freq):
  """
  Change the frequency
  frequency: - frequency in Hz (freq > 1.0)
  """
  s.send2pi('pwm_set_frequency', gpio, freq)

def pwm_start(gpio):
  """
  Start software PWM
  """
  s.send2pi('pwm_start', gpio)

def pwm_stop(gpio):
  """
  Stop software PWM
  MUST be called before exit
  Include delay BEFORE calling cleanup()
  """
  s.send2pi('pwm_stop', gpio)

def pwm_exists(gpio):
  """
  Returns 1 if there is a PWM for this gpio
  """
  return int(s.send2pi('pwm_exists', gpio))

#  SPI
def spiDataRW(channel, data, len):
  """
  Write and Read a block of data over the SPI bus
      Shared Tx/Rx buffer
  channel: - 0-3 (depending on available chip selects)
  data: pointer to buffer containing transmit data (will be overwritten by receive data)
  len: length of buffer

  Returns result code
  """
  b64data = base64.b64encode(data)
  s.send2pi('spiDataRW', channel, len, b64data)
  return b64data
