/*
Copyright (c) 2012-2018 Ben Croston

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// Function prototypes
//	c++ wrappers
#ifdef __cplusplus
extern "C" {
#endif

int setup(void);
void setup_gpio(int gpio, int direction, int pud);
int gpio_function(int gpio);
void output_gpio(int gpio, int value);
int input_gpio(int gpio);
int get_pullupdn(int gpio);
void set_rising_event(int gpio, int enable);
void set_falling_event(int gpio, int enable);
void set_high_event(int gpio, int enable);
void set_low_event(int gpio, int enable);
int eventdetected(int gpio);
void cleanup(void);

#define INPUT  1 // is really 0 for control register!
#define OUTPUT 0 // is really 1 for control register!
#define ALT0   4

#define HIGH 1
#define LOW  0

#define PUD_OFF  0
#define PUD_DOWN 1
#define PUD_UP   2

//	cpuinfo.h
#ifndef CPUINFO_H
#define CPUINFO_H
typedef struct
{
   int p1_revision;	// P1 Header 0:None, 1:Pi B, 2:Pi B V2, 3:40 pin
   char *ram;
   char *manufacturer;
   char *processor;
   char *type;
   char revision[1024];
} rpi_info;
#endif /* CPUINFO_H */

int get_rpi_info(rpi_info *info);

// extras.h
#ifndef EXTRAS_H
#define EXTRAS_H
void initialiseTimers (void);
unsigned int micros(void);
unsigned int millis (void);
#endif /* EXTRAS_H */

// event_gpio.h

// #define NO_EDGE      0
// #define RISING_EDGE  1
// #define FALLING_EDGE 2
// #define BOTH_EDGE    3
//
// int add_edge_detect(unsigned int gpio, unsigned int edge, int bouncetime);
// void remove_edge_detect(unsigned int gpio);
// int add_edge_callback(unsigned int gpio, void (*func)(unsigned int gpio));
// int event_detected(unsigned int gpio);
// int gpio_event_added(unsigned int gpio);
// int event_initialise(void);
// void event_cleanup(int gpio);
// void event_cleanup_all(void);
// int blocking_wait_for_edge(unsigned int gpio, unsigned int edge, int bouncetime, int timeout);

// soft_pwm.h
/* Software PWM using threads */

void pwm_set_duty_cycle(unsigned int gpio, float dutycycle);
void pwm_set_frequency(unsigned int gpio, float freq);
void pwm_start(unsigned int gpio);
void pwm_stop(unsigned int gpio);
int pwm_exists(unsigned int gpio);

// hard_pwm.h
// Hardware PWM

int pwmSetGpio(int gpio);
void pwmSetMode(int mode);
int pwmSetRange(int gpio, unsigned int range);
int pwmWrite(int gpio, int value);
void pwmSetClock(int divisor);

// PAD
int getPAD(unsigned group);
void setPAD(unsigned group, unsigned padstate);

#define	PWM_MODE_MS	0
#define	PWM_MODE_BAL	1

#ifdef __cplusplus
}
#endif
