/*
Setup hardware PWM clock

 2022-12-14
*/

#include <stdio.h>

#include <pi-gpio.h>

int isBCM2711(void) {
	unsigned rev = get_revision();
	unsigned PROC = (rev&0x0000F000)>>12;
	return PROC==3;
}

int main(int argc, char *argv[]) {
  // Pi3 & earlier have 19.2MHz clock
  int DIVIDER = 15;

  if(isBCM2711()) {
    // Pi4 has 54MHz clock
    DIVIDER = 45;
    }

  setup();
  pwmSetClock(DIVIDER);    // gives a precise 10kHz signal
	cleanup();
  return 0;
}
