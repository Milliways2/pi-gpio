/* Hardware PWM */

int pwmSetGpio(int gpio);
void pwmSetMode(int mode);
int pwmSetRange(int gpio, unsigned int range);
int pwmWrite(int gpio, int value);
int pwmSetDutycycle(int pin, float duty_cycle);
void pwmSetClock(int divisor);
unsigned int pwmGetRange(int gpio);


#define	PWM_MODE_MS	0
#define	PWM_MODE_BAL	1
