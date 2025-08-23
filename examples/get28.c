#include <stdio.h>
#include <string.h>
#include <time.h>

#include <pi-gpio.h>
/*
List GPIO 0-27
*/

int main() {
  int n;
  setup();

  printf("Print value of GPIO 0-27 as HEX \n");


  n = input_28();
  printf("GPIO %04x\n", n);

  cleanup();
  return 0;
}
