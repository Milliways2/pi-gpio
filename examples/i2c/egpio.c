#include <MCP23017.h>
#include <pi-gpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Test basic GPIO access routines for MCP23017 16-Bit I/O Expander

 * set all GPIO input with pullup
 * set pins 0-5 port A to output and output HIGH
 * set GPIO0 port B to output
 * set GPIO7 port B to input

 * read GPIO7 port B & print
 * read all pins & print
 * read port A & print
 * read port B & print

 * toggle GPIO0 port B 3 times (assumes LED connected)
 */
// 2022-09-05

const int i2cBus = 1;    // All Pi models with 40 pin header
unsigned i2cAddr = 0x20; // default for MCP23017
extern int ehand;        //  identify MCP23017 16-Bit I/O Expander on I2C bus

int main() {
  ehand = i2cOpen(i2cBus, i2cAddr);
  if (ehand < 0)
    exit(ehand);
  // Check if device exists
  if (i2cRead8(ehand, 0) < 0) {
    printf("No device at address %02x\n", i2cAddr);
    exit(-1);
  }

  printf("MCP23017 16-Bit I/O Expander test\n");

  setup_egpio_dev(0xFF);     // set all GPIO input
  setup_egpio_dev_pud(0xFF); // set pullup on all GPIO
  setup_egpio_port(0, 0x5F);
  output_egpio_port(0, 0xFF);

  setup_egpio(0, 1, 0, 0);     // set GPIO0 port B to output
  setup_egpio(7, 1, 1, 0);     // set GPIO7 port B to input
  int in7 = input_egpio(7, 1); // read GPIO7 port B
  printf("GPIO7 = %d\n", in7);
  printf("GPIO = %04x\n", input_egpio_dev());
  printf("GPIOA = %02x\n", input_egpio_port(0));
  printf("GPIOB = %02x\n", input_egpio_port(1));

  int count = 3;
  while (count--) {
    output_egpio(0, 1, 1); // set GPIO0 port B to 1
    sleep(1);
    output_egpio(0, 1, 0); // set GPIO0 port B to 0
    sleep(1);
  }

  return 0;
}
