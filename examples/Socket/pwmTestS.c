/* Software PWM test
Slowly (over 10 seconds) ramp up brightness of LED then ramp down

TO BUILD

// If building on remote system gcc pwmTestS.c pi-sock.c  -o pwmTestS
// gcc pwmTestS.c -lpi-sock -o pwmTestS

*/
// 2023-10-02

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// #include "pi-sock.h"	// If building on remote system 
#include <pi-sock.h>

// #define LEDpin 18
#define LEDpin 12

int main(int argc, char *argv[]) {
  char* host = "127.0.1.1";
  if (argc > 1) { host = argv[1]; } // Optional host address
  if(socket_connect(host)<0)  return -1;
	printf("Ramp up brightness of LED connected to GPIO %d then ramp down\n", LEDpin);

  // ensure channel set as output
  setup_gpio(LEDpin, OUTPUT, 0);
  pwm_set_duty_cycle(LEDpin, 0);
  pwm_set_frequency(LEDpin, 100);
  pwm_start(LEDpin);

  if (pwm_exists(LEDpin))
    printf("pwm enabled on %i\n", LEDpin);

  for (int bright = 0; bright < 100; bright++) {
    usleep(100000);
    pwm_set_duty_cycle(LEDpin, bright);
  }

  for (int bright = 100; bright; bright--) {
    usleep(100000);
    pwm_set_duty_cycle(LEDpin, bright);
  }

  pwm_stop(LEDpin);
  return 0;
}
