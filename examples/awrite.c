// MCP4802 DAC test

/*	Simple test for the MCP4802 DAC on the Gertboard or similar
 *  See spiOpen for detail
 */

// 2022-09-09

#include <pi-gpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Assumes MCP4802 on SPI0.1 as on Gertboard
static unsigned controller = 0; // SPI0
static unsigned SPIchannel = 1; // SPIx.1
static unsigned DACchan = 0;    // one of 2 analog SPI_channels on MCP4802

unsigned spiSpeed = 1000000;
unsigned spiMode = 0;

/* Output value on analog channel achan of MCP4802 DAC
   achan 0 or 1
   value 0 - 255
 */
static void analogWrite(int achan, int value) {
  unsigned char spiData[2];
  unsigned char chanBits;
  //   printf("achan %d value %d\n", achan, value);

  chanBits = achan ? 0xB0 : 0x30;
  spiData[0] = chanBits | ((value >> 4) & 0x0F);
  spiData[1] = ((value << 4) & 0xF0);
  //   printf("spiData HEX %0x %0x\n", spiData[0], spiData[1]);
  spiDataRW(SPIchannel, spiData, 2);
}

float voltageOut(unsigned sample) { return (2.048 * sample) / 255; }

int main(int argc, char *argv[]) {
  int fd = 0;
  int i;
  printf("Raspberry Pi MCP4802 test program\n");
  printf("SPIchannel: %d\n", SPIchannel);
  printf("DACchan: %d\n", DACchan);
  fd = spiOpen(controller, SPIchannel, spiSpeed,
               spiMode); // Initialise SPI controller
  if (fd < 0) {
    printf("Unable to open /dev/spidev%d.%d\n", controller, SPIchannel);
    exit(-1);
  }

  // Output an incrementing analog value
  for (i = 0; i < 256; i += 16) {
    printf("\rValue: %d Voltage: %1.2f   ", i, voltageOut(i));
    fflush(stdout);
    analogWrite(DACchan, i);
    sleep(1);
  }
  printf("\n");
  close(fd);
}
