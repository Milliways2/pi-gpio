# pi-sock
Includes a socket interface to the pi-gpio dynamic C library to control Raspberry Pi GPIO.  
This allows access to all functions without `sudo` and allows remote access.  
Supports all production models / all SOC

This library has code to:-

Read Raspberry Pi Information

Perform basic GPIO functions  
	• Set gpio as input or output  
	• Read/Write gpio  
	• Set pullup/down of gpio  
	• Read the current gpio mode  
	• Read gpio pull/up down (Pi4/BCM2711 only)  

Hardware PWM support

Read/Set PAD drive, hysteresis & slew settings

<!-- 
There is currently no access to

Software PWM

I²C

SPI
 -->

Overlay/Library
===============
Includes a python overlay with access to most functions  
This is available with `include pi_sock`

There is a Socket library included in pi-gpio to facilitate access to the Socket server in C programs.  
To use #include <pi-sock.h> in your program and Link with `-lpi-sock`

The overlay and library expose the same functions and allow existing GPIO programs to be adapted with minor changes to setup routines.

Documentation
=============
To read documentation:-  
	C:	`man pi-gpio` includes c & socket documentation  
	Python:	`python3 -m pydoc pi_sock`

Dependencies
============
Requires pi-gpio installed on target Pi with Pi Socket daemon `pi-gpiod` running.  
All functionality is available without root privileges.

External Computers
==================
It is possible to access Pi GPIO on most systems with a Socket interface.  

C programs should compile with `pi-sock.c`  
Requires Linux/Unix Libraries or equivalent  
	<arpa/inet.h>  
	<stdio.h>  
	<string.h>  
	<sys/socket.h>  
	<unistd.h>  
	<stdlib.h>

Python3 support on all platforms through `pi_sock.py` with Libraries  
	sys, os, time, socket

Other Languages
===============
It would be possible to access the socket server from any language with Socket support.

The socket server accepts/returns ASCII encoded strings with space or comma delimiters between fields.  
	Instructions are constructed as follows:-  
	  Command string  
	  Parameters 0 - 3 integers  
	  Final Parameter - may be int or float  
Any calling program is responsible for checking the validity of Instructions.
