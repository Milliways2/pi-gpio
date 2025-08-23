/*
This is a SPI module using the spidev kernel driver.
This MUST be enabled BEFORE using this code (see spiOpen for detail).

For documentation See https://www.kernel.org/doc/html/latest/spi/index.html
Pi documentation https://www.raspberrypi.com/documentation/computers/raspberry-pi.html#spi-overview

2022-09-07
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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

static const uint8_t     spiBPW   = 8;
static const uint16_t    spiDelay = 0;

static uint32_t    spiSpeeds [3];
static int         spiFds [3];

#define SPI_OPEN_FAIL  -9
#define SPI_MODE_FAIL  -10
#define SPI_BPW_FAIL  -11
#define SPI_SPEED_FAIL  -12

int spiOpen(unsigned controller, unsigned channel, unsigned speed, unsigned mode)
{
	/*
	Raspberry Pi Zero, 1, 2 and 3 have three SPI controllers:

	SPI0, with 2 hardware chip selects, is available on all Raspberry Pis;
	there is also an alternate mapping that is only available on Compute Modules.
	To enable SPI0 use raspi-config, or ensure the line dtparam=spi=on is not commented out in /boot/config.txt.
	By default it uses 2 chip select lines, but this can be reduced to 1 using dtoverlay=spi0-1cs.

	SPI1, with 3 hardware chip selects, is available on all Raspberry Pi models except the original with 26 pin header.
		mode 1,3 do not work!
	To enable SPI1 with 1, 2 or 3 chip select lines add to the /boot/config.txt file.
		dtoverlay=spi1-1cs  #1 chip select
		dtoverlay=spi1-2cs  #2 chip select
		dtoverlay=spi1-3cs  #3 chip select

	SPI2, also with 3 hardware chip selects, is only available on Compute Module 1, 3 and 3+.

	On the Raspberry Pi 4, 400 and Compute Module 4 there are 4 additional SPI buses:
	SPI3 to SPI6, each with 2 hardware chip selects.

	These overlays can be enabled on the fly with a command e.g.
		sudo dtoverlay spi1-1cs
	*/

	int fd;
	char spiDev [32];

	mode    &= 3;	// Mode is 0, 1, 2 or 3

	snprintf (spiDev, 31, "/dev/spidev%d.%d", controller, channel);

	if ((fd = open (spiDev, O_RDWR)) < 0)
	{
		return SPI_OPEN_FAIL;
	}

	spiSpeeds [channel] = speed;
	spiFds    [channel] = fd;

	// Set SPI parameters.
	if (ioctl (fd, SPI_IOC_WR_MODE, &mode)            < 0)
	{
		printf("SPI_IOC_WR_MODE: %d\n", fd);
		return SPI_MODE_FAIL;}
	if (ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0)
	{
		printf("SPI_BPW_FAIL: %d\n", fd);
		return SPI_BPW_FAIL;}
	if (ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed)   < 0)
	{
		printf("SPI_IOC_WR_MAX_SPEED_HZ: %d\n", fd);
		return SPI_SPEED_FAIL;}
	return fd;
}

int spiDataRW2(int channel, unsigned char *tx_data, unsigned char *rx_data, int len)
{
	struct spi_ioc_transfer spi;

	memset (&spi, 0, sizeof (spi));

	spi.tx_buf        = (unsigned long)tx_data;
	spi.rx_buf        = (unsigned long)rx_data;
	spi.len           = len;
	spi.delay_usecs   = spiDelay;
	spi.speed_hz      = spiSpeeds [channel];
	spi.bits_per_word = spiBPW;

	return ioctl (spiFds [channel], SPI_IOC_MESSAGE(1), &spi);
}

/*
 *	Write and Read a block of data over the SPI bus.
 *	Note the data ia being read into the transmit buffer, so will overwrite it!
 *	This is also a full-duplex operation.
 */

int spiDataRW(int channel, unsigned char *data, int len)
{
	return spiDataRW2(channel, data, data, len);
}

