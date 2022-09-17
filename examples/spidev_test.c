/*
 * SPI testing utility (using spidev driver)
 *
 * Includes code Copyright (c) 2007  MontaVista Software, Inc.
 * Includes code Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 To test the SPI bus, you can use spidev_test.c that is a modified version from
 www.kernel.org.

 Connect the SPI_MOSI and SPI_MISO pins together. The output should be as
 follows: spi mode: 3 bits per word: 8 max speed: 32000000 Hz (32000 KHz)

        FF FF FF FF FF FF
        40 00 00 00 00 95
        FF FF FF FF FF FF
        FF FF FF FF FF FF
        FF FF FF FF FF FF
        DE AD BE EF BA AD
        F0 0D

        This is the block of data that is defined in the tx[] array inside the
 spidev_test.c code.

        Interestingly, you can determine a maximum SCLK by increasing the
 frequency within the spidev_test.c code until you get an until you get an
 inconsistent block of data. The maximum practical frequency is 32 MHz but later
 models may support higher speeds.

        The output displayed should be 0x00 when nothing is connected to the bus
 and the test program is executed.

 */
// 2022-09-06

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <pi-gpio.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static uint8_t controller = 0; // SPI0
// static uint8_t controller = 1;	// SPI1
static uint8_t channel = 0; // .0
static uint8_t mode = 0;
// static uint8_t mode = 3;
static uint8_t bits = 8;
// static uint32_t speed = 32000000;
static uint32_t speed = 1000000;

// uint8_t tx[] = {
// 	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
// 	0x40, 0x00, 0x00, 0x00, 0x00, 0x95,
// 	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
// 	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
// 	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
// 	0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD,
// 	0xF0, 0x0D,
// };
uint8_t tx[] = {0x37, 0xF0};
uint8_t rx[ARRAY_SIZE(tx)] = {
    0,
};

int main(int argc, char *argv[]) {
  int ret = 0;
  ret = spiOpen(controller, channel, speed,
                mode); // SPI controller, channel, speed, mode

  if (ret > 0) {
    ret = spiDataRW2(channel, tx, rx, ARRAY_SIZE(tx));
  } else {
    printf("Unable to open /dev/spidev%d.%d\n", controller, channel);
    exit(-1);
  }
  printf("Connect the SPI_MOSI and SPI_MISO pins together.\nThe output should "
         "be settings followed by:\n");
  for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
    if (!(ret % 6))
      puts("");
    printf("%.2X ", tx[ret]);
  }
  puts("\n");
//
  printf("Device: /dev/spidev%d.%d\n", controller, channel);

  printf("spi mode: %d\n", mode);
  printf("bits per word: %d\n", bits);
  printf("max speed: %d Hz (%d KHz)\n", speed, speed / 1000);

  if (ret > 0) {
    for (ret = 0; ret < ARRAY_SIZE(rx); ret++) {
      if (!(ret % 6))
        puts("");
      printf("%.2X ", rx[ret]);
    }
    puts("");
  } else
    printf("ret: %d\n", ret);
}
