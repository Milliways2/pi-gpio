// This is a program to test socket equivalents of pi-gpio functions

// If building on remote system gcc testS.c pi-sock.c -o testC 
// gcc testS.c -lpi-sock -o testC
// 2023-09-14

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #include "pi-sock.h"	// If building on remote system 
#include <pi-sock.h>

int main(int argc, char *argv[]) {
	char* host = "127.0.1.1";
  if (argc > 1) { host = argv[1];	}	// Optional host address
  if(socket_connect(host)<0)	return -1;

	printf("This is a program to test pi-gpio socket server on host %s  port %d\n", host, PORT);

// **********************

	rpi_info info;
	get_rpi_info(&info);
	printf("\np1_revision\t%d\n", info.p1_revision);
	printf("type\t%s\n", info.type);
	printf("ram\t%s\n", info.ram);
	printf("processor\t%s\n", info.processor);
	printf("manufacturer\t%s\n", info.manufacturer);
	printf("revision\t%s\n", info.revision);
	
	printf("\n");
	printf("get_revision()\t%x\n", get_revision());
	printf("input_gpio(5)\t%d\n", input_gpio(5));
	printf("gpio_function(14)\t%d\n", gpio_function(14));
	printf("get_pullupdn(4)\t%d\n", get_pullupdn(4));
	setup_gpio(12, 1, 0);
	output_gpio(12, 1);
	printf("input_28()\t%x\n", input_28());
	output_28( 0, 1<<4);	// Set GPIO 4 (mask 16) to 0

	printf("\n");
	printf("getPAD()\t%x\n", getPAD(0));
	setPAD(0, 27);
	
	printf("\n");
	printf("pwmSetGpio\t%d\n", pwmSetGpio(12));
	pwmSetMode(0);		// void pwmSetMode(int mode);
	pwmSetClock(75);	// void pwmSetClock(int divisor);
	printf("pwmSetRange\t%d\n", pwmSetRange(12, 256));
	printf("pwmWrite\t%d\n", pwmWrite(12, 128));
	printf("pwmSetDutycycle\t%d\n", pwmSetDutycycle(12, 0.8));
	printf("pwmGetRange\t%d\n", pwmGetRange(12));

	socket_disconnect();
  return 0;
}
