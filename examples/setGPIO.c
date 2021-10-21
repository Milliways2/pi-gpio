#include <stdio.h>
#include <string.h>
#include <time.h>

#include <pi-gpio.h>

int main() {
  setup();

  setup_gpio(17, INPUT, 0);  // Set GPIO 17 INPUT no PU
  setup_gpio(18, OUTPUT, 0); // Set GPIO 18 OUTPUT
  output_gpio(18, 1);        // Set GPIO HIGH

  cleanup();

  return 0;
}
