#include "pi-i2c.h"
/* basic GPIO access routines for MCP23017 16-Bit I/O Expander
        GPIO & port 0 should work with MCP23008 8-Bit I/O Expander
        GPIO can be accessed individually,
        by port (8 bits)
        or device (16 bits)
        Access modes can be mixed.
*/
// 2022-09-02

// PORT B registers are incremented by 1
#define IODIRA 0x00   // I/O DIRECTION REGISTER
#define IPOLA 0x02    // INPUT POLARITY REGISTER
#define GPINTENA 0x04 // INTERRUPT-ON-CHANGE CONTROL REGISTER
#define DEFVALA 0x06  // DEFAULT COMPARE REGISTER FOR INTERRUPT-ON-CHANGE
#define INTCONA 0x08  // NTERRUPT CONTROL REGISTER
#define IOCON 0x0A    // CONFIGURATION REGISTER
#define GPPUA 0x0C    // PULL-UP RESISTOR CONFIGURATION REGISTER
#define INTFA 0x0E    // INTERRUPT FLAG REGISTER
#define INTCAPA 0x10  // INTERRUPT CAPTURE REGISTER
#define GPIOA 0x12    // PORT REGISTER
#define OLATA 0x14    // OUTPUT LATCH REGISTER

#ifdef __cplusplus
 "C" {
#endif

// ehand MUST be set by calling program e.g. i2cOpen()
// NOTE this is intended to allow this code to be used by multiple MCP23017 Expanders
int ehand; //  identify MCP23017 16-Bit I/O Expander on I2C bus

// ________  BIT operations

int setup_egpio(int gpio, int port, int direction, int pud) {
  /* Set single gpio channel as an input or an output
   *		gpio 0-7
   *		port 0 (port A) or 1 (port B)
   *		direction: 0=IN, 1=OUT
   *		pud: 0=None 1=Up */
  unsigned dirreg = i2cRead8(ehand, IODIRA + port);
  unsigned mask = 1 << gpio;
  if (direction) {
    // input
    dirreg |= mask;
    unsigned pubreg = i2cRead8(ehand, GPPUA + port);
    if (pud)
      pubreg |= mask;
    else
      pubreg &= ~mask;
    i2cWrite8(ehand, GPPUA + port, pubreg); // GPPU register
  } else {
    // output
    dirreg &= ~mask;
  }
  return i2cWrite8(ehand, IODIRA + port, dirreg); // IODIR register
}

int output_egpio(int gpio, int port, int value) {
  /* Output to a GPIO channel
   *		gpio 0-7
   *		port 0 (port A) or 1 (port B)
   *		value - 0/1 or False/True or LOW/HIGH */

  unsigned datreg = i2cRead8(ehand, OLATA + port);
  unsigned mask = 1 << gpio;

  if (value) {
    datreg |= mask;
  } else {
    datreg &= ~mask;
  }
  return i2cWrite8(ehand, OLATA + port, datreg); // OLAT register
}

int input_egpio(int gpio, int port) {
  /* Returns the GPIO level.
   *		gpio 0-7
   *		port 0 (port A) or 1 (port B)
   *		Returns HIGH=1=True or LOW=0=False */
  unsigned mask = 1 << gpio;
  unsigned datreg = i2cRead8(ehand, GPIOA + port);
  mask = (1 << gpio % 32);
  unsigned value = datreg & mask;
  return value ? 1 : 0;
}

// ________  PORT operations
int setup_egpio_port(unsigned port, unsigned dirreg) {
  /* Set all channel on specified port
   *		port 0 (port A) or 1 (port B)
   *		dirreg: 8 bit mask to set 0=IN, 1=OUT */
  return i2cWrite8(ehand, IODIRA + port, dirreg); // IODIR register
}

int setup_egpio_port_pud(unsigned port, unsigned pudreg) {
  /* Set pull on specified port
   *	port 0 (port A) or 1 (port B)
   *	pudreg: 8 bit mask to set pull 0=None 1=Up */
  return i2cWrite8(ehand, GPPUA + port, pudreg); // GPPU register
}

int output_egpio_port(unsigned port, unsigned datreg) {
  /* Output to all channel on specified port
   *	port 0 (port A) or 1 (port B)
   *	datreg -  8 bit mask to set 0/1 */
  return i2cWrite8(ehand, OLATA + port, datreg); // OLAT register
}

int input_egpio_port(unsigned port) {
  /* Returns the GPIO level.
   *	port 0 (port A) or 1 (port B)
   *	Returns 8 bit levels */
  return i2cRead8(ehand, GPIOA + port);
}

// ________  Device operations
int setup_egpio_dev(unsigned dirreg) {
  /* Set all channels
   *	dirreg: 16 bit mask to set 0=IN, 1=OUT */
  return i2cWrite16(ehand, IODIRA, dirreg); // IODIR register
}

int setup_egpio_dev_pud(unsigned pudreg) {
  /* Set pull on all channels
   *	pudreg: 16 bit mask to set pull 0=None 1=Up */
  return i2cWrite16(ehand, GPPUA, pudreg); // GPPU register
}

int output_egpio_dev(unsigned datreg) {
  /* Output to all channel
   *	datreg -  16 bit mask to set 0/1 */
  return i2cWrite16(ehand, OLATA, datreg); // OLAT register
}

int input_egpio_dev() {
  /* Returns the GPIO level.
   *	Returns 16  bit levels */
  return i2cRead16(ehand, GPIOA);
}

#ifdef __cplusplus
}
#endif
