/*
This program toggles a GPIO pin and measures rate.
Using pi-gpio

TO BUILD

gcc -Wall -o bench-pi-gpio bench-pi-gpio.c -lpi-gpio

sudo ./bench-pi-gpio

*/

// 2021-08-11

#define SigOUT 12
#define LOOPS 20000

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>

#include <pi-gpio.h>

int main(void) {
  unsigned i;
  double t0, t1;

  setup();
  setup_gpio(SigOUT, OUTPUT, 0);

  t0 = micros();

  for (i = 0; i < LOOPS + 1; i++) {
    output_gpio(SigOUT, 1);
    output_gpio(SigOUT, 0);
  }

  t1 = micros();
  printf("pi-gpio\t%10.0f toggles per second\n", (1.0e6 * LOOPS) / (t1 - t0));

  cleanup();
  return 0;
}
