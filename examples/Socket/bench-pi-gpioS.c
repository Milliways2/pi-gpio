/*
This program toggles a GPIO pin and measures rate.
Using pi-gpio Socket Interface

TO BUILD

// If building on remote system gcc bench-pi-gpioS.c pi-sock.c  -o bench-pi-gpioS
// gcc bench-pi-gpioS.c -lpi-sock -o bench-pi-gpioS

*/
// 2023-09-14

#define SigOUT 12
#define LOOPS 2000

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include <stdint.h>
#include <stdlib.h>

// #include "pi-sock.h"	// If building on remote system 
#include <pi-sock.h>

static uint64_t epochMicro ;

void initialiseTimers(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  epochMicro = (uint64_t)ts.tv_sec * (uint64_t)1000000 + (uint64_t)(ts.tv_nsec / 1000L);
}

unsigned int micros(void) {
// Return a number of microseconds as an unsigned int.
// Wraps after 71 minutes.
  uint64_t now;
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  now = (uint64_t)ts.tv_sec * (uint64_t)1000000 + (uint64_t)(ts.tv_nsec / 1000);
  return (uint32_t)(now - epochMicro);
}

int main(int argc, char *argv[]) {
  unsigned i;
  double t0, t1;

  char* host = "127.0.1.1";
  if (argc > 1) { host = argv[1]; } // Optional host address
  if(socket_connect(host)<0)  return -1;

  initialiseTimers();
  
  setup_gpio(SigOUT, OUTPUT, 0);

  t0 = micros();
  
  for (i = 0; i < LOOPS + 1; i++) {
    output_gpio(SigOUT, 1);
    output_gpio(SigOUT, 0);
  }

  t1 = micros();
  printf("pi-gpio Socket\t%10.0f toggles per second\n", (1.0e6 * LOOPS) / (t1 - t0));

  socket_disconnect();
  return 0;
}
