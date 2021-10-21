/* Hardware PWM */

int pwmSetGpio(int gpio);
void pwmSetMode(int mode);
int pwmSetRange(int gpio, unsigned int range);
int pwmWrite(int gpio, int value);
void pwmSetClock(int divisor);


#define	PWM_MODE_MS	0
#define	PWM_MODE_BAL	1
