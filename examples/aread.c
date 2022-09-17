// MCP3002 ADC test

/*	Simple test for the MCP3002 ADC on the Gertboard or similar
 *  See spiOpen for detail
 */

// 2022-09-10

#include <pi-gpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Assumes MCP3002 on SPI0.0 as on Gertboard
static unsigned controller = 0; // SPI0
static unsigned SPIchannel = 0; // SPIx.0
static unsigned DACchan = 0; // one of 2 analog SPI_channels on MCP3002

// NOTE speed is a compromise; too low allows charge loss; slower speeds allow higher impedance connections
// unsigned spiSpeed = 1000000;
unsigned spiSpeed = 100000;
unsigned spiMode = 0;

/* Input value on analog channel achan of MCP3002 DAC
   achan 0 or 1
 */
static int analogRead(int achan) {
  unsigned char spiData[2];
  unsigned char chanBits;
	// Microchip Technology Inc. DS21294E
	// SPI Communication with the MCP3002 using 8-bit segments (Mode 0,0: SCLK idles low)
	// Bit pattern	Low Start SGL Odd MSBF x x x
  chanBits = achan ? 0b01111000 : 0b01101000;
  spiData[0] = chanBits;
  spiData[1] = 0;

//   printf("Input spiData HEX %0x %0x\n", spiData[0], spiData[1]);
  spiDataRW(SPIchannel, spiData, 2);
//   printf("Output spiData HEX %0x %0x\n", spiData[0], spiData[1]);
  return ((spiData[0] << 8) | (spiData[1] >> 1)) & 0x3FF;
}

float voltage(unsigned sample) { return (3.3 * sample) / 1023; }

int main(int argc, char *argv[]) {
  int fd = 0;
  printf("Raspberry Pi MCP3002 test program\n");
  printf("SPIclock: %d kHz\n", spiSpeed/1000);
  printf("SPIchannel: %d\n", SPIchannel);

  fd = spiOpen(controller, SPIchannel, spiSpeed,
               spiMode); // Initialise SPI controller

  if (fd < 0)
    return -1;
  sleep(1);

  int inputValue = analogRead(0);
  printf("DACchan: %d\n", DACchan);
  printf("Value: %d Voltage: %1.2f\n", inputValue, voltage(inputValue));

// test other channel
	DACchan ^= 1;
  inputValue = analogRead(1);
  printf("DACchan: %d\n", DACchan);
  printf("Value: %d Voltage: %1.2f\n", inputValue, voltage(inputValue));

  close(fd);
}
