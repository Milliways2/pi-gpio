#include <stdio.h>
#include <string.h>
#include <time.h>

#include <pi-gpio.h>

#define PinA 17
#define PinB 18

int main() {
  setup();

    printf("Set GPIO%d INPUT GPIO%d OUTPUT,HIGH\n", PinA, PinB);

  setup_gpio(PinA, INPUT, 0);  // Set GPIO 17 INPUT no PU
  setup_gpio(PinB, OUTPUT, 0); // Set GPIO 18 OUTPUT
  output_gpio(PinB, 1);        // Set GPIO HIGH

  cleanup();

  return 0;
}
