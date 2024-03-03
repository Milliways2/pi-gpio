#define SOCK

#define PORT 31425

// extract from <pi-gpio.h>
// Function prototypes

//	c++ wrappers
#ifdef __cplusplus
extern "C" {
#endif

#ifndef SOCK
int setup(void);
void cleanup(void);
#endif

#ifdef SOCK
int socket_connect(char* host);
void socket_disconnect();
#endif

void setup_gpio(int gpio, int direction, int pud);
int gpio_function(int gpio);
void output_gpio(int gpio, int value);
int input_gpio(int gpio);
int get_pullupdn(int gpio);
int input_28(void);
void output_28(unsigned bits, unsigned mask);
void set_rising_event(int gpio, int enable);
void set_falling_event(int gpio, int enable);
void set_high_event(int gpio, int enable);
void set_low_event(int gpio, int enable);
int eventdetected(int gpio);

// 2022-11-19	Fix setup_gpio direction
#define INPUT  0
#define OUTPUT 1

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
   char revision[32];	// 2023-09-04
} rpi_info;

int get_rpi_info(rpi_info *info);
unsigned get_revision(void);
#endif /* CPUINFO_H */

// extras.h
#ifndef EXTRAS_H
#define EXTRAS_H
void initialiseTimers (void);
unsigned int micros(void);
unsigned int millis (void);
#endif /* EXTRAS_H */

// soft_pwm.h
/* Software PWM using threads */
#ifndef PiSOFT_PWM_PWM_H
#define PiSOFT_PWM_PWM_H
void pwm_set_duty_cycle(unsigned int gpio, float dutycycle);
void pwm_set_frequency(unsigned int gpio, float freq);
void pwm_start(unsigned int gpio);
void pwm_stop(unsigned int gpio);
int pwm_exists(unsigned int gpio);
#endif /* PiSOFT_PWM_PWM_H */

// hard_pwm.h
// Hardware PWM
#ifndef PiHARD_PWM_H
#define PiHARD_PWM_H
int pwmSetGpio(int gpio);
void pwmSetMode(int mode);
int pwmSetRange(int gpio, unsigned int range);
int pwmWrite(int gpio, int value);
int pwmSetDutycycle(int pin, float duty_cycle);
void pwmSetClock(int divisor);
unsigned int pwmGetRange(int gpio);

#define	PWM_MODE_MS	0
#define	PWM_MODE_BAL	1
#endif /* PiHARD_PWM_H */

// I2C
#ifndef PiI2C_H
#define PiI2C_H

int i2cOpen(unsigned i2cBus, unsigned i2cAddr);

int i2cRead(unsigned handle);
int i2cRead8(unsigned handle, unsigned i2cReg);
int i2cRead16(unsigned handle, unsigned i2cReg);

int i2cWrite(unsigned handle, int data);
int i2cWrite8(unsigned handle, unsigned i2cReg, int data);
int i2cWrite16(unsigned handle, unsigned i2cReg, int data);
#endif /* PiI2C_H */

// SPI
#ifndef PiSPI_H
#define PiSPI_H
int spiOpen(unsigned controller, unsigned channel, unsigned speed, unsigned mode);
int spiDataRW2(int channel, unsigned char *tx_data, unsigned char *rx_data, int len);
int spiDataRW (int channel, unsigned char *data, int len);
#endif /* PiSPI_H */

// PAD
#ifndef PiPAD_H
#define PiPAD_H
int getPAD(unsigned group);
void setPAD(unsigned group, unsigned padstate);
#endif /* PiPAD_H */

#ifdef __cplusplus
}
#endif
