/*
This is a hardware PWM test program
It generates 2 PWM outputs @10kHz with differing duty cycles

 cc -Wall -o hwPWMtest -lpi-gpio -lpthread -lm -lcrypt -lrt hwPWMtest.c

 2021-10-16
 2022-12-12	isBCM2711()
*/

#include <stdio.h>
#include <string.h>

#include <pi-gpio.h>

#define PWM0 12 // this is physical pin 32
#define PWM1 13 // this is physical pin 33
// #define PWM0 18                    // this is physical pin 12
// #define PWM1 19                    // this is physical pin 35

int isBCM2711(void) {
	unsigned rev = get_revision();
	unsigned PROC = (rev&0x0000F000)>>12;
	return PROC==3;
}

int main(int argc, char *argv[]) {
  // The following gives a precise 10kHz PWM signal
  // CLOCK / (DIVIDER * RANGE)
  // Pi3 & earlier have 19.2MHz clock
  int DIVIDER = 15;
  int RANGE = 128;
  if(isBCM2711()) {
    // Pi4 has 54MHz clock
    DIVIDER = 45;
    RANGE = 120;
    }

  setup();

  pwmSetGpio(PWM0);
  pwmSetGpio(PWM1);

  pwmSetMode(PWM_MODE_MS); // use a fixed frequency
  pwmSetClock(DIVIDER);

  pwmSetRange(PWM0, RANGE);
  pwmWrite(PWM0, RANGE / 4); // duty cycle of 25%

  pwmSetRange(PWM1, RANGE);
  pwmWrite(PWM1, RANGE / 2); // duty cycle of 50%
	cleanup();
  return 0; // PWM output stays on after exit
}
