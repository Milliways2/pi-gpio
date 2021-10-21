#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <pi-gpio.h>

/* Software PWM test
Slowly (over 10 seconds) ramp up brightness of LED then ramp down
*/
#define LEDpin 18

int main() {
  setup();

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
