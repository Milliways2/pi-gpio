/* Sample SPI Example Program to read the Device ID & registers of the ADXL345.
*  Adapted for pi-gpio */

// 2025-05-07
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pi-gpio.h>
#include <string.h>

unsigned spiSpeed = 1000000;
unsigned spiMode = 3;
static unsigned controller = 0; // SPI0
static unsigned SPIchannel = 0; // SPIx.0

// The ADXL345 Resisters required for this example
// To read or write multiple bytes in a single transmission, the multiple-byte bit,
// located after the R/W bit in the first byte transfer must be set
#define READBIT 0x80	// 1 to READ
#define MULTIBYTE 0x40

#define DEVID 0x00 (Read/Write)
#define POWER_CTL 0x2D
#define DATA_FORMAT 0x31
#define DATAX0 0x32
#define DATAX1 0x33
#define DATAY0 0x34
#define DATAY1 0x35
#define DATAZ0 0x36
#define DATAZ1 0x37

#define BUFFER_SIZE 0x40
unsigned char dataBuffer[BUFFER_SIZE+1];	// R/W buffer
unsigned char *receivedBuffer = dataBuffer+1;	// 1st byte read is ignored

// Write a single value to a register
int writeRegister(int SPIchannel, int reg, int value) {
  dataBuffer[0] = reg;
  dataBuffer[1] = value;
	return spiDataRW(SPIchannel, dataBuffer, 2);
}

// Read a single register
unsigned readRegister(int SPIchannel, int reg) {
	memset(dataBuffer, 0x00, BUFFER_SIZE+1);    // clear the full memory buffer
  dataBuffer[0] = reg | READBIT;
  dataBuffer[1] = 0;
	spiDataRW(SPIchannel, dataBuffer, 2);
	return receivedBuffer[0];
}

// Read sequential registers into the buffer
int readRegisters(int SPIchannel, int reg, int no_bytes) {
	dataBuffer[0] = reg | READBIT | MULTIBYTE;		// sequential read
	return spiDataRW(SPIchannel, dataBuffer, no_bytes+1);     // read no_bytes registers
}

float a[3];
const float ACC_CONV = 1/(powf(2, 10) / 4);    // acceleration conversion factor (10-bit resolution ±2g) 

float * readAccel(int channel) {
//     Read data for each axis, convert to acceleration in g units.
//     Returns array containing ax, ay, az
	int i;
	readRegisters(channel, DATAX0, 6);     // read DATAXYZ registers

 	for(i=0; i<6; i++){	// display  registers
		printf("%02X ", receivedBuffer[i]);	// display value in hexadecmial
	}
	printf("\n");

//  	for(i=0; i<6; i=i+2){
// 		a[i/2] = ACC_CONV * ((short)((short)receivedBuffer[i] + ((short)receivedBuffer[i+1]<<8)));
// 	}
 	for(i=0; i<3; i++){
		a[i] = ACC_CONV * ((short)((short)receivedBuffer[i+i] + ((short)receivedBuffer[i+i+1]<<8)));
	}
	return a;
}

int main() {
	int fd = 0;
	int i;
	memset(dataBuffer, 0x00, BUFFER_SIZE+1);    // clear the full memory buffer

  fd = spiOpen(controller, SPIchannel, spiSpeed, spiMode); // Initialise SPI controller
  if (fd < 0)	return -1;
  sleep(1);
	writeRegister(SPIchannel, POWER_CTL, 0x08);	//take the ADXL345 out of power-saving mode
  sleep(1);
	readRegisters(SPIchannel, 0, BUFFER_SIZE);     // read all registers

	printf("The DEVID is %X (%d)\n", receivedBuffer[0x00], receivedBuffer[0x00]); // display register 0x00
	printf("The full set of registers are:\n");
	for(i=0; i<BUFFER_SIZE; i++){	// display all 0x40 registers
		printf("%02X ", receivedBuffer[i]);	// display value in hexadecmial
		if(i%16==15) printf("\n");	// place \n after each 15th value
   }

	printf("\n");
	readAccel(SPIchannel);
	printf("X=%0.2fg Y=%0.2fg Z=%0.2fg\n", a[0], a[1], a[2]);

}
