#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
This is a hardware PWM test program
	It generates 2 PWM outputs 
	PWM0 - 1kHz  with 25% duty cycle
	PWM1 - 20kHz with 50% duty cycle

 cc -Wall -o kPWMtest -lpi-gpio -lpthread -lm -lcrypt -lrt kPWMtest.c

 2024-02-27

--
The Pi has 2 independent hardware PWM channels.
	The same  PWM channel is available on multiple pins but the output is identical
	Channel 0 can be accessed on GPIO 12, 18, 40, 52
	Channel 1 can be accessed on GPIO 13, 19, 41, 45, 53
	
The kernel PWM service needs to be started before use.

To set up PWM on GPIO 18/19 
	dtoverlay=pwm-2chan in config.txt 
	sudo dtoverlay pwm-2chan on command line

To set up PWM on GPIO 12/13
	dtoverlay=pwm-2chan,pin=12,func=4,pin2=13,func2=4
	sudo dtoverlay pwm-2chan  pin=12 func=4 pin2=13 func2=4

There is a service pwm to set up a single channel.
*/

#include <pi-gpio.h>

int main(int argc, char* argv[]) {
  if(kpwm_export(0) < 1) {
    printf("Overlay not loaded\n");
    exit(-1);
  }
  if(kpwm_export(1) < 1) {
    printf("Overlay not loaded\n");
    exit(-1);
  }
  usleep(100000);                  // sleep for 100ms
  kpwm_start(0, 1000000, 250000);
//   kpwm_start(1, 50000, 25000);  
  kpwm_start_f(1, 20000, 0.5);

}
