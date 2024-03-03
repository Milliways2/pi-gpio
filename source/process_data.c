#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pi-gpio.h"
#include "process_data.h"
char out_buffer[BLEN]; // data buffer

// 2023-10-09

// Process instruction received from socket and execute pi-gpio function
// Instructions are constructed as follows:-
//   Command string
//   Parameters 0 - 3 integers
//   Final Parameter - may be int, float or string
void process_data(char *instr) {
  char *cmd;
  char *param;
	char *finalparam;	// Contains last valid parameter
  const char *seps = " ,";
  cmd = strtok(instr, seps);
// 	printf("cmd = %s\n", instr);
	unsigned np=0, p[4] = {0};
	param = strtok(NULL, seps);
	while( param != NULL ) {
		finalparam = param;
		p[np++] = atoi(param);
		param = strtok(NULL, seps);
	 }

//    GPIO
	if (strcmp(cmd, "setup_gpio") == 0) {	// void setup_gpio(int gpio, int direction, int pud);
			setup_gpio(p[0], p[1], p[2]);
			strcpy(out_buffer, "0");
			return;
	}

	if (strcmp(cmd, "output_gpio") == 0) {	// void output_gpio(int gpio, int value);
		output_gpio(p[0], p[1]);
		strcpy(out_buffer, "0");
		return;
	}

	if (strcmp(cmd, "input_gpio") == 0) {	// int input_gpio(int gpio);
		strcpy(out_buffer, input_gpio(p[0]) ? "1" : "0" ); // copy  "1" or "0" to out_buffer depending on GPIO
		return;
	}

	if (strcmp(cmd, "gpio_function") == 0) {	// int gpio_function(int gpio);
		sprintf(out_buffer, "%d", gpio_function(p[0]));
		return;
	}
	
	if (strcmp(cmd, "get_pullupdn") == 0) {	// int get_pullupdn(int gpio);
		sprintf(out_buffer, "%d", get_pullupdn(p[0]));
		return;
	}
	
	if (strcmp(cmd, "input_28") == 0) {	// int input_28(void);
		sprintf(out_buffer, "%d", input_28());
		return;
	}

	if (strcmp(cmd, "output_28") == 0) {	/// void output_28(unsigned bits, unsigned mask);
		output_28(p[0], p[1]);
		strcpy(out_buffer, "0");
		return;
	}

//    Information
char *get_rpi_dict();
	if (strcmp(cmd, "get_rpi_info") == 0) {	// int get_rpi_info(rpi_info *info);
		strcpy(out_buffer, get_rpi_dict());
		return;
	}

	if (strcmp(cmd, "get_revision") == 0) {	// unsigned get_revision(void);
		sprintf(out_buffer, "%d", get_revision());
		return;
	}

// PAD
	if (strcmp(cmd, "getPAD") == 0) {	// int getPAD(unsigned group);
		sprintf(out_buffer, "%d", getPAD(p[0]));
		return;
	}

	if (strcmp(cmd, "setPAD") == 0) {	// void setPAD(unsigned group, unsigned padstate);
		setPAD(p[0], p[1]);
		strcpy(out_buffer, "0");
		return;
	}

// Hardware PWM
	if (strcmp(cmd, "pwmSetGpio") == 0) {	// int pwmSetGpio(int gpio);
		sprintf(out_buffer, "%d", pwmSetGpio(p[0]));
		return;
	}

	if (strcmp(cmd, "pwmSetMode") == 0) {	// void pwmSetMode(int mode);
		pwmSetMode(p[0]);
		strcpy(out_buffer, "0");
		return;
	}
	
	if (strcmp(cmd, "pwmSetRange") == 0) {	// int pwmSetRange(int gpio, unsigned int range);
		sprintf(out_buffer, "%d", pwmSetRange(p[0], p[1]));
		return;
	}

	if (strcmp(cmd, "pwmWrite") == 0) {	// int pwmWrite(int gpio, int value);
		sprintf(out_buffer, "%d", pwmWrite(p[0], p[1]));
		return;
	}

	if (strcmp(cmd, "pwmSetDutycycle") == 0) {	// int pwmSetDutycycle(int pin, float duty_cycle);
		sprintf(out_buffer, "%d", pwmSetDutycycle(p[0], atof(finalparam)));
		return;
	}

	if (strcmp(cmd, "pwmSetClock") == 0) {	// void pwmSetClock(int divisor);
		pwmSetClock(p[0]);
		strcpy(out_buffer, "0");
		return;
	}

	if (strcmp(cmd, "pwmGetRange") == 0) {	// unsigned int pwmGetRange(int gpio);
		sprintf(out_buffer, "%d", pwmGetRange(p[0]));
		return;
	}

// I2C
	if (strcmp(cmd, "i2cOpen") == 0) {	int i2cOpen(unsigned i2cBus, unsigned i2cAddr);
		sprintf(out_buffer, "%d", i2cOpen(p[0], p[1]));
		return;
	}
	
	if (strcmp(cmd, "i2cRead") == 0) {	// int i2cRead			(unsigned handle);
		sprintf(out_buffer, "%d", i2cRead(p[0]));
		return;
	}
	
	if (strcmp(cmd, "i2cRead8") == 0) {	// int i2cRead8		(unsigned handle, unsigned i2cReg);
		sprintf(out_buffer, "%d", i2cRead8(p[0], p[1]));
		return;
	}
	
	if (strcmp(cmd, "i2cRead16") == 0) {	//	int i2cRead16     (unsigned handle, unsigned i2cReg);
		sprintf(out_buffer, "%d", i2cRead16(p[0], p[1]));
		return;
	}
	
	if (strcmp(cmd, "i2cWrite") == 0) {	//	int i2cWrite		(unsigned handle, int data);
		sprintf(out_buffer, "%d", i2cWrite(p[0], p[1]));
		return;
	}
	
	if (strcmp(cmd, "i2cWrite8") == 0) {	//	int i2cWrite8    (unsigned handle, unsigned i2cReg, int data);
		sprintf(out_buffer, "%d", i2cWrite8(p[0], p[1], p[2]));
		return;
	}

	if (strcmp(cmd, "i2cWrite16") == 0) {	//	int i2cWrite16    (unsigned handle, unsigned i2cReg, int data);
		sprintf(out_buffer, "%d", i2cWrite16(p[0], p[1], p[2]));
		return;
	}

// Software PWM
	if (strcmp(cmd, "pwm_set_duty_cycle") == 0) {	// void pwm_set_duty_cycle(unsigned int gpio, float dutycycle);
		pwm_set_duty_cycle(p[0], atof(finalparam));
		return;
	}

	if (strcmp(cmd, "pwm_set_frequency") == 0) {	// void pwm_set_frequency(unsigned int gpio, float freq);
		pwm_set_frequency(p[0], atof(finalparam));
		return;
	}

	if (strcmp(cmd, "pwm_start") == 0) {	// void pwm_start(unsigned int gpio);
		pwm_start(p[0]);
		strcpy(out_buffer, "0");
		return;
	}

	if (strcmp(cmd, "pwm_stop") == 0) {	// void pwm_stop(unsigned int gpio);
		pwm_stop(p[0]);
		strcpy(out_buffer, "0");
		return;
	}

	if (strcmp(cmd, "pwm_exists") == 0) { // int pwm_exists(unsigned int gpio);
		strcpy(out_buffer, pwm_exists(p[0]) ? "1" : "0" ); // copy  "1" or "0" to out_buffer depending on result
		return;
	}
	
// SPI
// int spiOpen(unsigned controller, unsigned channel, unsigned speed, unsigned mode);
// int spiDataRW2(int channel, unsigned char *tx_data, unsigned char *rx_data, int len);
// int spiDataRW (int channel, unsigned char *data, int len);
// 	if (strcmp(cmd, "pwmSetDutycycle") == 0) {	// int pwmSetDutycycle(int pin, float duty_cycle);
// 		sprintf(out_buffer, "%d", pwmSetDutycycle(p[0], atof(finalparam)));
// 		return;

	if (strcmp(cmd, "spiDataRW") == 0) {	// int spiDataRW (int channel, unsigned char *data, int len);
	// 	NOTE channel  received as p[0], len received as p[1], data as base64 encoded string
	// 	This is test code - it DOES NOT execute anything
	// 		sprintf(out_buffer, "%d", spiDataRW(p[0], finalparam));
		strcpy(out_buffer, finalparam);
		return;
	}
	
	strcpy(out_buffer, "Unknown command");
	return;
}
