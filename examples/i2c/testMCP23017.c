/*
This program writes to port B of a MCP23017 16-Bit I/O Expander.
 */
#include <pi-gpio.h>
#include <MCP23017.h>
#include <stdio.h>
#include <stdlib.h>
// MCP23017
// 2022-09-05

const int i2cBus = 1;    // All Pi models with 40 pin header
unsigned i2cAddr = 0x20; // default for MCP23017
extern int ehand;        //  identify MCP23017 16-Bit I/O Expander on I2C bus

#define IODIRA	0x00
#define IPOLA	0x02
#define GPINTENA	0x04
#define DEFVALA	0x06
#define INTCONA	0x08
#define IOCON	0x0A
#define GPPUA	0x0C
#define INTFA	0x0E
#define INTCAPA	0x10
#define GPIOA	0x12
#define OLATA	0x14

#define IODIRB	0x01
#define IPOLB	0x03
#define GPINTENB	0x05
#define DEFVALB	0x07
#define INTCONB	0x09
// #define IOCON	0x0B
#define GPPUB	0x0D
#define INTFB	0x0F
#define INTCAPB	0x11
#define GPIOB	0x13
#define OLATB1	0x15


int main(){
  ehand = i2cOpen(i2cBus, i2cAddr);
  if (ehand < 0)
    exit(ehand);
  // Check if device exists
  if (i2cRead8(ehand, 0) < 0) {
    printf("No device at address %02x\n", i2cAddr);
    exit(-1);
  }

  unsigned value = 0xAA;
  printf("writes %02X to port B of a MCP23017 16-Bit I/O Expander\n", value);

// set all of port B to outputs
   i2cWrite8(ehand, IODIRB, 0x00);	 // IODIRB register
   i2cWrite8(ehand, GPIOB, value);	 // GPIOB register

   return 0;
}
