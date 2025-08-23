/*
This program toggles a MCP23017 16-Bit I/O Expander pin and measures rate.
 */
// 2022-09-05

#include <MCP23017.h>
#include <pi-gpio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

const int i2cBus = 1;    // All Pi models with 40 pin header
unsigned i2cAddr = 0x20; // default for MCP23017
extern int ehand;        //  identify MCP23017 16-Bit I/O Expander on I2C bus

#define LOOPS 2000

static uint64_t epochMicro;

void initialiseTimers(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  epochMicro =
      (uint64_t)ts.tv_sec * (uint64_t)1000000 + (uint64_t)(ts.tv_nsec / 1000L);
}

/*
 * micros:
 *	Return a number of microseconds as an unsigned int.
 *	Wraps after 71 minutes.
 */
unsigned int micros(void) {
  uint64_t now;
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  now = (uint64_t)ts.tv_sec * (uint64_t)1000000 + (uint64_t)(ts.tv_nsec / 1000);
  return (uint32_t)(now - epochMicro);
}

int main(void) {
  unsigned i;
  double t0, t1;

  ehand = i2cOpen(i2cBus, i2cAddr);
  if (ehand < 0)
    exit(ehand);
  // Check if device exists
  if (i2cRead8(ehand, 0) < 0) {
    printf("No device at address %02x\n", i2cAddr);
    exit(-1);
  }

  printf("MCP23017 16-Bit I/O Expander rate test\n");

  initialiseTimers();
  t0 = micros();
  setup_egpio(0, 1, 0, 0); // set GPIO0 port B to output

  for (i = 0; i < LOOPS + 1; i++) {
    output_egpio(0, 1, 1); // set GPIO0 port B to 1
    output_egpio(0, 1, 0); // set GPIO0 port B to 0
  }
  t1 = micros();
  printf("MCP23017 bit\t\t%10.0f toggles per second\n",
         (1.0e6 * LOOPS) / (t1 - t0));

  t0 = micros();
  setup_egpio_port(1, 0xFF);
  for (i = 0; i < LOOPS + 1; i++) {
    output_egpio_port(1, 0xAA); // set GPIO0 port B to 0xAA
    output_egpio_port(1, 0x55);    // set GPIO0 port B to 0x55
  }
  t1 = micros();
  printf("MCP23017 port (8 bit)\t%10.0f toggles per second\n",
         (1.0e6 * LOOPS) / (t1 - t0));

  t0 = micros();
  setup_egpio_port(1, 0xFF);
  for (i = 0; i < LOOPS + 1; i++) {
    output_egpio_dev(0xFFFF); // set to 0xFFFF
    output_egpio_dev(0);    // set to 0
  }
  t1 = micros();
  printf("MCP23017 dev (16 bit)\t%10.0f toggles per second\n",
         (1.0e6 * LOOPS) / (t1 - t0));

  return 0;
}
