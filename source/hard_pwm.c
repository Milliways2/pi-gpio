/*
This is a Hardware PWM module
Based on WiringPi code

2021-11-08
2023-07-27	pwmGetRange, pwmSetDutycycle
*/

/*
 *	Includes code originally from  wiringPi:
 *	Copyright (c) 2012-2017 Gordon Henderson
 *	Additional code for pwmSetClock by Chris Hall <chris@kchall.plus.com>
 *
 *	wiringPi is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as
 *	published by the Free Software Foundation, either version 3 of the
 *	License, or (at your option) any later version.
 *
 *	Includes code fragments from pigpio
 *
 *	See <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

//	Word offsets into the PWM control region

#define	PWM_CONTROL	0
#define	PWM_STATUS	1
#define	PWM0_RANGE	4
#define	PWM0_DATA	5
#define	PWM1_RANGE	8
#define	PWM1_DATA	9

//	Clock regsiter offsets

#define	PWMCLK_CNTL	40
#define	PWMCLK_DIV	41

#define	PWM0_MS_MODE    0x0080  // Run in MS mode
#define	PWM0_USEFIFO    0x0020  // Data from FIFO
#define	PWM0_REVPOLAR   0x0010  // Reverse polarity
#define	PWM0_OFFSTATE   0x0008  // Ouput Off state
#define	PWM0_REPEATFF   0x0004  // Repeat last value if FIFO empty
#define	PWM0_SERIAL     0x0002  // Run in serial mode
#define	PWM0_ENABLE     0x0001  // Channel Enable

#define	PWM1_MS_MODE    0x8000  // Run in MS mode
#define	PWM1_USEFIFO    0x2000  // Data from FIFO
#define	PWM1_REVPOLAR   0x1000  // Reverse polarity
#define	PWM1_OFFSTATE   0x0800  // Ouput Off state
#define	PWM1_REPEATFF   0x0400  // Repeat last value if FIFO empty
#define	PWM1_SERIAL     0x0200  // Run in serial mode
#define	PWM1_ENABLE     0x0100  // Channel Enable

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "c_gpio.h"

#include "hard_pwm.h"

#define BCM_PASSWORD 0x5A000000

#define ALT0   4
#define ALT1   5
#define ALT2   6
#define ALT3   7
#define ALT4   3
#define ALT5   2

// Locals to hold pointers to the hardware
extern volatile uint32_t *gpio_map;
extern volatile uint32_t *pads_map;
extern volatile uint32_t *pwm_map;
extern volatile uint32_t *clk_map;

extern void setupMemCheck();

void delayMicroseconds (unsigned int howLong)
{
  struct timeval tNow, tLong, tEnd ;

  gettimeofday (&tNow, NULL) ;
  tLong.tv_sec  = howLong / 1000000 ;
  tLong.tv_usec = howLong % 1000000 ;
  timeradd (&tNow, &tLong, &tEnd) ;

  while (timercmp (&tNow, &tEnd, <))
    gettimeofday (&tNow, NULL) ;
}

//	The port value to put a GPIO pin into PWM mode | ALT mode << 4
static uint8_t gpioToPwmPort [] = {
          0,             0,           0,             0,           0,             0,           0,         0,	//  0 ->  7
          0,             0,           0,             0, PWM0_DATA|ALT0<<4, PWM1_DATA|ALT0<<4, 0,         0,	//  8 -> 15
          0,             0, PWM0_DATA|ALT5<<4, PWM1_DATA|ALT5<<4, 0,             0,           0,         0,	// 16 -> 23
          0,             0,           0,             0,           0,             0,           0,         0,	// 24 -> 31
          0,             0,           0,             0,           0,             0,           0,         0,	// 32 -> 39
PWM0_DATA|ALT0<<4, PWM1_DATA|ALT0<<4, 0,             0,           0,       PWM1_DATA|ALT0<<4, 0,         0,	// 40 -> 47
          0,             0,           0,             0, PWM0_DATA|ALT1<<4, PWM1_DATA|ALT1<<4              	// 48 -> 53
};

// Select the native "balanced" mode PWM_MODE_BAL or standard mark:space mode PWM_MODE_MS
void pwmSetMode(int mode) {
  setupMemCheck();
  if (mode == PWM_MODE_MS)
    *(pwm_map + PWM_CONTROL) =
        PWM0_ENABLE | PWM1_ENABLE | PWM0_MS_MODE | PWM1_MS_MODE;
  else
    *(pwm_map + PWM_CONTROL) = PWM0_ENABLE | PWM1_ENABLE;
}

// Set the PWM range register.
int pwmSetRange(int gpio, unsigned int range) {
  setupMemCheck();
  switch (gpioToPwmPort[gpio] & 0xf) {
    case PWM0_DATA:
      *(pwm_map + PWM0_RANGE) = range;
      break;
    case PWM1_DATA:
      *(pwm_map + PWM1_RANGE) = range;
      break;
    default:
      return -1;
      delayMicroseconds(10);
  }
  return 0;
}

// Get the PWM range.
unsigned int pwmGetRange(int gpio) {
  setupMemCheck();
	unsigned int range;
	switch (gpioToPwmPort[gpio] & 0xf) {
    case PWM0_DATA:
      range = *(pwm_map + PWM0_RANGE);
      break;
    case PWM1_DATA:
      range = *(pwm_map + PWM1_RANGE);
      break;
    default:
      return 0;
  }
  return range;
}

// Set an output PWM value 
int pwmWrite(int pin, int value) {
  setupMemCheck();
  int channel = gpioToPwmPort[pin] & 0xf;
  if (channel) {
    *(pwm_map + channel) = value;
    return 0;
  }
  return -1;
}

// Set PWM duty_cycle 
int pwmSetDutycycle(int pin, float duty_cycle) {
	return pwmWrite(pin, pwmGetRange(pin) * duty_cycle + 0.5);
}

// Put gpio pin into PWM mode
int pwmSetGpio(int gpio) {
  int new_mode = (gpioToPwmPort[gpio] & 0xf0) >> 4;
  if (new_mode) {
    int offset = (gpio / 10);
    int shift = (gpio % 10) * 3;
    int value = *(gpio_map + offset);
    *(gpio_map + offset) = (value & ~(7 << shift)) | new_mode << shift;
    return 0;
  }
  return -1;
}

// Set/Change the PWM clock.
void pwmSetClock(int divisor) {
  uint32_t pwm_control;
  divisor &= 4095;
  setupMemCheck();

  pwm_control = *(pwm_map + PWM_CONTROL); // preserve PWM_CONTROL

  // We need to stop PWM prior to stopping PWM clock in MS mode otherwise BUSY stays high.

  *(pwm_map + PWM_CONTROL) = 0; // Stop PWM

  // Stop PWM clock before changing divisor. The delay after this does need to
  // this big (95uS occasionally fails, 100uS OK), it's almost as though the
  // BUSY flag is not working properly in balanced mode. Without the delay when
  // DIV is adjusted the clock sometimes switches to very slow, once slow
  // further DIV adjustments do nothing and it's difficult to get out of this
  // mode.

  *(clk_map + PWMCLK_CNTL) = BCM_PASSWORD | 0x01; // Stop PWM Clock
  delayMicroseconds(110); // prevents clock going sloooow

  while ((*(clk_map + PWMCLK_CNTL) & 0x80) != 0) // Wait for clock to be !BUSY
    delayMicroseconds(1);

  *(clk_map + PWMCLK_DIV) = BCM_PASSWORD | (divisor << 12);

  *(clk_map + PWMCLK_CNTL) = BCM_PASSWORD | 0x11; // Start PWM clock
  *(pwm_map + PWM_CONTROL) = pwm_control;         // restore PWM_CONTROL
}
