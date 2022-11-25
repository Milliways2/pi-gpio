# pi-gpio
A dynamic C library to control Raspberry Pi GPIO channels
Supports all production models / all SOC

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

I²C

SPI

Extras
======

micros & millis timers

Extensions
==========

GPIO access routines for MCP23017 16-Bit I/O Expander (requires MCP23017.h)

Dependencies
============

Most code uses direct hardware access to the SoC Peripheral and is independent of the kernel.

	• Raspberry Pi Information is determined from a register on the SoC, which is available from device-tree.  
	• I²C & SPI use kernel drivers.

If run without root permision pi-gpio uses character device /dev/gpiomem to map the GPIO register page.
This allows GPIO access to members of the gpio group but does not provide access the PWM or clock hardware registers.

NOTE Users of pi-gpio programs on *Raspberry Pi OS* should be members of group gpio (Ubuntu group dialout).
If error message "Pi Setup failure" the most likely cause is that user does not have access. 

If run with root permision uses character device /dev/mem which allows full access but requires determining the address of the GPIO peripheral which varies depending on SoC.
