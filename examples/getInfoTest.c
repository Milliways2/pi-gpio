/*
This program displays Raspberry Pi Information.
Type: RAM: Processor: Manufacturer:
*/

// 2021-08-11

#include <stdio.h>
#include <string.h>
#include <pi-gpio.h>

int main ()
{
	rpi_info  info;

	get_rpi_info(&info);
	printf("Type: %s  RAM: %s Processor: %s Manufacturer: %s \n", info.type, info.ram, info.processor, info.manufacturer);

	return 0 ;
}

