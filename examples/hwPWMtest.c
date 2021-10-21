/*
This is a hardware PWM test program
It generates 2 PWM outputs @10kHz with differing duty cycles

 cc -Wall -o hwPWMtest -lpi-gpio -lpthread -lm -lcrypt -lrt hwPWMtest.c
 2021-10-16
*/

#include <stdio.h>
#include <string.h>

#include <pi-gpio.h>

#define PWM0 12 // this is physical pin 32
#define PWM1 13 // this is physical pin 33
// #define PWM0       18                    // this is physical pin 12
// #define PWM0       19                    // this is physical pin 35

int main(int argc, char *argv[]) {
  rpi_info info;
  // Pi3 & earlier have 19.2MHz clock
  // The following gives a precise 10kHz signal
  int DIVIDER = 15;
  int RANGE = 128;
  setup();

  get_rpi_info(&info);

  if (strcmp(info.processor, "BCM2711") == 0) {
    // Pi4 has 54MHz clock
    DIVIDER = 45;
    RANGE = 120;
  }

  pwmSetGpio(PWM0);
  pwmSetGpio(PWM1);

  pwmSetMode(PWM_MODE_MS); // use a fixed frequency
  pwmSetClock(DIVIDER);    // gives a precise 10kHz signal

  pwmSetRange(PWM0, RANGE);
  pwmWrite(PWM0, RANGE / 4); // duty cycle of 25%

  pwmSetRange(PWM1, RANGE);
  pwmWrite(PWM1, RANGE / 2); // duty cycle of 50%
	cleanup();
  return 0; // PWM output stays on after exit
}
