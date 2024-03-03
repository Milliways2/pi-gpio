#include <stdio.h>
//2024-02-25
// Compile: gcc kpwm.c -o kpwm

/*
This code provides user space access to the PWM hardware.
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

#define PWM_SYSFS "/sys/class/pwm/pwmchip0/"
#define PWM_PATH0 "/sys/class/pwm/pwmchip0/pwm0/"
#define PWM_PATH1 "/sys/class/pwm/pwmchip0/pwm1/"

int kpwm_export(int chan) { // export PWM channel
//     Export PWM channel
//     chan: 0 or 1
  FILE *fp;
  int res;
  fp = fopen(PWM_SYSFS "export", "w"); // open file for writing
  if (fp) {
    res = fprintf(fp, "%d", chan);
    fclose(fp);
    return res;
  }
  return -1;
}

int kpwm_unexport(unsigned chan) {
//     Unexport PWM channel
//     chan: 0 or 1
  FILE *fp;
  int res;
  fp = fopen(PWM_SYSFS "unexport", "w"); // open file for writing
  if (fp) {
    res = fprintf(fp, "%d", chan);
    fclose(fp);
    return res;
  }
  return -1;
}

unsigned kpwm_get_period(unsigned chan) {
// chan: 0 or 1
	FILE* fp=0;	// prevent maybe-uninitialized warning 
	unsigned period;
	if(chan == 0)
		fp = fopen(PWM_PATH0 "period", "r");
	if(chan == 1)
		fp = fopen(PWM_PATH1 "period", "r");
	if(fp) fscanf(fp, "%d", &period);           // get the period
	return period;
}

void kpwm_enable(unsigned chan, unsigned enable) {
// Enable/disable PWM on chan
// chan: 0 or 1
// enable: 0 or 1
	FILE* fp=0;	// prevent maybe-uninitialized warning 
  if (chan == 0)
    fp = fopen(PWM_PATH0 "enable", "w");
  if (chan == 1)
    fp = fopen(PWM_PATH1 "enable", "w");
	if (fp) {
		fprintf(fp, "%d", enable);      
		fclose(fp);
	}
}

void kpwm_start(unsigned chan, unsigned period, unsigned pulse_width) {
// Enable the PWM on chan, setting period and pulse_width
// chan: 0 or 1
// period: The period of the PWM signal in nanoseconds
// pulse_width: The period of the PWM signal in nanoseconds
	FILE* fp=0;	// prevent maybe-uninitialized warning 
  if (chan == 0) {
    fp = fopen(PWM_PATH0 "period", "w");
    if (fp) {
      fprintf(fp, "%d", period);      fclose(fp);
    }
    fp = fopen(PWM_PATH0 "duty_cycle", "w");
    if (fp) {
      fprintf(fp, "%d", pulse_width);      fclose(fp);
    }
  }
  if (chan == 1) {
    fp = fopen(PWM_PATH1 "period", "w");
    if (fp) {
      fprintf(fp, "%d", period);      fclose(fp);
    }
    fp = fopen(PWM_PATH1 "duty_cycle", "w");
    if (fp) {
      fprintf(fp, "%d", pulse_width);      fclose(fp);
    }
  }
  kpwm_enable(chan, 1);
}

void kpwm_start_f(unsigned chan, float frequency, float duty_cycle) {
// Enable the PWM on chan, setting frequency and duty_cycle
// chan: 0 or 1
// frequency: The frequency of the PWM signal in Hz
//	 (used to calculate period to nearest 10nS)
// duty_cycle: 0.0 - 1.0
  unsigned period, pulse_width;
  period = 1000000000/frequency;
	pulse_width = period * duty_cycle;
	return kpwm_start(chan, period, pulse_width);
}

void kpwm_set_pulse_width(unsigned chan, unsigned pulse_width) {
// chan: 0 or 1
// duty_cycle: 0.0 - 1.0
	FILE* fp=0;	// prevent maybe-uninitialized warning 
	if(kpwm_get_period(chan) < pulse_width) return;
  if (chan == 0)
    fp = fopen(PWM_PATH0 "duty_cycle", "w");
  if (chan == 1)
    fp = fopen(PWM_PATH1 "duty_cycle", "w");
	if (fp) {
		 fprintf(fp, "%d", pulse_width); // send the pulse_width to the file
		 fclose(fp);
	}
}

void kpwm_set_duty_cycle(unsigned chan, float duty_cycle) {
// chan: 0 or 1
// duty_cycle: 0.0 - 1.0
	FILE* fp=0;	// prevent maybe-uninitialized warning 
	unsigned period = kpwm_get_period(chan);
	unsigned pulse_width = period * duty_cycle;
  if (chan == 0)
    fp = fopen(PWM_PATH0 "duty_cycle", "w");
  if (chan == 1)
    fp = fopen(PWM_PATH1 "duty_cycle", "w");
	if (fp) {
		 fprintf(fp, "%d", pulse_width); // send the pulse_width to the file
		 fclose(fp);
	}
}
