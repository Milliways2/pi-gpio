#include <stdio.h>
#include <string.h>
#include <time.h>

#include <pi-gpio.h>
/*
List GPIO, Value, Function, Pullup/down (Pi4 only) for GPIO 0-27
*/

int main() {
  int n, f, p;
  setup();

  for (int i = 0; i < 28; i++) {
    n = input_gpio(i);
    f = gpio_function(i);
    p = (f == 0) ? get_pullupdn(i) : 0; // only input
    if (f == 0)
      printf("GPIO %2i Value=%i Function %i Pull %i\n", i, n, f, p);
    else
      printf("GPIO %2i Value=%i Function %i\n", i, n, f);
  }

  cleanup();
  return 0;
}
