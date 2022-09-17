/*
Simplified I2C access routines

This is a I2C module using the I2C kernel driver.
I2C MUST be enabled BEFORE using this code (see i2cOpen for detail).

For documentation See https://www.kernel.org/doc/html/latest/i2c/index.html

SMBus (System Management Bus) is based on the I2C protocol, and is mostly a subset of I2C protocols and signaling.
Many I2C devices will work on an SMBus,
but some SMBus protocols add semantics beyond what is required to achieve I2C branding.

Because the SMBus is mostly a subset of the generalized I2C bus, we can use its protocols on many I2C systems.

2022-08-06
*/

/*
 *	Includes code originally from  wiringPi:
 *	Copyright (c) 2012-2017 Gordon Henderson
 *
 *	wiringPi is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as
 *	published by the Free Software Foundation, either version 3 of the
 *	License, or (at your option) any later version.
 *
 *	See <http://www.gnu.org/licenses/>.
 */

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <asm/ioctl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

static inline int i2c_smbus_access(unsigned handle, char rw, uint8_t command,
                                   int size, union i2c_smbus_data *data) {
  struct i2c_smbus_ioctl_data args;

  args.read_write = rw;
  args.command = command;
  args.size = size;
  args.data = data;
  return ioctl(handle, I2C_SMBUS, &args);
}

//   This returns a handle to the I2C device at the address on the I2C bus.
int i2cOpen(unsigned i2cBus, unsigned i2cAddr) {
  /*	i2cBus: 0-1
  		i2cAddr: 0-0x7F

	To enable I2C use raspi-config, or ensure the line dtparam=i2c_arm=on or dtparam=i2c=on is not commented out in /boot/config.txt.

	I2C can be enabled on the fly with a command e.g.
		sudo dtparam i2c
	*/

  int fd;
  char i2cDev[32];
  snprintf(i2cDev, 31, "/dev/i2c-%d", i2cBus);

  if ((fd = open(i2cDev, O_RDWR)) < 0) {
		perror("Unable to open I2C device");
    fprintf(stderr, "Unable to open I2C device: %s\n", strerror(errno));
    return -1;
  }

  if (ioctl(fd, I2C_SLAVE, i2cAddr) < 0) {
    printf("Unable to select I2C device: %s\n", strerror(errno));
    return -1;
  }
  //   printf("errret = %i\n", erret);
  return fd;
}

// Read a single byte from a device, without specifying a register
int i2cRead(unsigned handle) {
  union i2c_smbus_data data;
// Some devices are so simple that this interface is enough;
// for others it is a shorthand if you want to read the same register as the revious
  if (i2c_smbus_access(handle, I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data))
    return -1;
  else
    return data.byte & 0xFF;
}

// Read a single byte from the specified register
int i2cRead8(unsigned handle, unsigned i2cReg) {
  union i2c_smbus_data data;

  if (i2c_smbus_access(handle, I2C_SMBUS_READ, i2cReg, I2C_SMBUS_BYTE_DATA,
                       &data))
    return -1;
  else
    return data.byte & 0xFF;
}

// Read a single word (16 bits) from the specified register
int i2cRead16(unsigned handle, unsigned i2cReg) {
  union i2c_smbus_data data;

  if (i2c_smbus_access(handle, I2C_SMBUS_READ, i2cReg, I2C_SMBUS_WORD_DATA, &data))
    return -1;
  else
    return data.word & 0xFFFF;
}

// Write a single byte to a device, without specifying a register
int i2cWrite(unsigned handle, int data) {
  return i2c_smbus_access(handle, I2C_SMBUS_WRITE, data, I2C_SMBUS_BYTE, NULL);
}

// Write a single byte to a device, to the specified register
int i2cWrite8(unsigned handle, unsigned i2cReg, int value) {
  union i2c_smbus_data data;

  data.byte = value;
  return i2c_smbus_access(handle, I2C_SMBUS_WRITE, i2cReg, I2C_SMBUS_BYTE_DATA,
                          &data);
}

// Write a single word (16 bits) to the specified register
int i2cWrite16(unsigned handle, unsigned i2cReg, int value) {
  union i2c_smbus_data data;

  data.word = value;
  return i2c_smbus_access(handle, I2C_SMBUS_WRITE, i2cReg, I2C_SMBUS_WORD_DATA,
                          &data);
}
