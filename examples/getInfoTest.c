/*
This program displays Raspberry Pi Information.
Type: RAM: Processor: Manufacturer:
*/

// 2022-08-06

#include <stdio.h>
#include <string.h>
#include <pi-gpio.h>
#include <pi-vers.h>

int main ()
{
	rpi_info  info;

	printf("pi-gpio version: %s\n", VERSION);

	get_rpi_info(&info);
	printf("Type: %s  RAM: %s Processor: %s Manufacturer: %s \n", info.type, info.ram, info.processor, info.manufacturer);

	return 0 ;
}

