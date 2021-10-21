/*
This is a test program to read the Pi PAD settings
Compile:	cc -Wall -o getPAD getPAD.c -lpi-gpio
2021-10-17
*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#include <pi-gpio.h>

int main(int argc, char *argv[]) {
  int group = 0;
  int padstate, slew, hyst, drive;
  setup();

  rpi_info info;
  get_rpi_info(&info);
  printf("%s\n", info.type);

  padstate = getPAD(group);
  slew = (padstate >> 4) & 1;
  hyst = (padstate >> 3) & 1;
  drive = padstate & 7;
  printf("slew = %d \nhyst = %d \ndrive = %d \n", slew, hyst, drive);

  cleanup();

  return 0;
}
