/*
This is a test program to set a subset of GPIO 0-27
Compile:	cc -Wall -o set28 set28.c -lpi-gpio -lpthread
2022-09-09
*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#include <pi-gpio.h>
#include <stdint.h>

#define TEST_GPIO_A 4
#define MASK_A 1 << TEST_GPIO_A
#define TEST_GPIO_B 17
#define MASK_B 1 << TEST_GPIO_B

int main() {
  unsigned bits, mask;
  setup();

  printf("This is a demonstration of  output_28\n");
  printf("It sets GPIO%i GPIO%i as OUTPUT\n", TEST_GPIO_A, TEST_GPIO_B);
  printf("then sets GPIO%d LOW\n", TEST_GPIO_A);

  //   bits = 0x00;
  bits = MASK_A;
  mask = MASK_A | MASK_B;
  printf("then sets bits %08x masked by  %08x\n", bits, mask);

  setup_gpio(TEST_GPIO_A, OUTPUT, 0); // Set TEST_GPIO_A OUTPUT
  setup_gpio(TEST_GPIO_B, OUTPUT, 0); // Set TEST_GPIO_B OUTPUT
  output_gpio(TEST_GPIO_A, 0);        // Set GPIO LOW

  printf("GPIO %2i Value=%i\n", TEST_GPIO_A, input_gpio(TEST_GPIO_A));
  printf("GPIO %2i Value=%i\n", TEST_GPIO_B, input_gpio(TEST_GPIO_B));

  output_28(bits, mask);
  printf("GPIO %04x\n", input_28());
  printf("GPIO %2i Value=%i\n", TEST_GPIO_A, input_gpio(TEST_GPIO_A));
  printf("GPIO %2i Value=%i\n", TEST_GPIO_B, input_gpio(TEST_GPIO_B));

  cleanup();
  return 0;
}
