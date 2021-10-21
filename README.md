# pi-gpio
A dynamic C library to control Raspberry Pi GPIO channels

Supports all production models / all SoC

This library has code to:-

Read Raspberry Pi Information

Perform basic GPIO functions  
	• Set gpio as input or output  
	• Read/Write gpio  
	• Set pullup/down of gpio  
	• Read the current gpio mode  
	• Read gpio pull/up down (Pi4/BCM2711 only)

Software PWM on all pins

Hardware PWM support

Read/Set PAD drive, hysteresis & slew settings

Dependencies
============

All code uses direct hardware access to the SoC Peripheral, except Raspberry Pi Information, and are independent of the kernel.

If run without root permision uses character device /dev/gpiomem to map the GPIO register page.
This allows GPIO access to members of the gpio group but does not provide access the PWM or clock hardware registers.

If run with root permision uses character device /dev/mem which allows full access but requires determining the address of the GPIO peripheral which varies depending on SoC.

Raspberry Pi Information is determined from a register on the SoC, which is available from device-tree.
