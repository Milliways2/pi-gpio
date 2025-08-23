/** Sample I2C ADXL345 Code that outputs the x,y and z accelerometer values
 *   on a single line for sixty seconds.
 * Original by Derek Molloy for the book "Exploring Raspberry Pi"
 * Adapted for pi-i2c */
// 2022-09-09
// 2025-05-09	Convert to g values

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pi-i2c.h>

const int i2cBus = 1;
const int i2cAddr = 0x53;

// The ADXL345 Resisters required for this example
#define DEVID 0x00
#define POWER_CTL 0x2D
#define DATA_FORMAT 0x31
#define DATAX0 0x32
#define DATAX1 0x33
#define DATAY0 0x34
#define DATAY1 0x35
#define DATAZ0 0x36
#define DATAZ1 0x37
#define BUFFER_SIZE 0x40
unsigned char dataBuffer[BUFFER_SIZE];

// Write a single value to a single register
int writeRegister(int file, unsigned char address, char value) {
  unsigned char buffer[2];
  buffer[0] = address;
  buffer[1] = value;
  if (write(file, buffer, 2) != 2) {
//     printf("Failed write to the device\n");
    return -1;
  }
  return 0;
}

// Read the entire 40 registers into the buffer (10 reserved)
int readRegistersAll(int file) {
  // Writing a 0x00 to the device sets the address back to 0 for the coming block read
  writeRegister(file, 0x00, 0x00);
  if (read(file, dataBuffer, BUFFER_SIZE) != BUFFER_SIZE) {
    printf("Failed to read in the full buffer\n");
    return -1;
  }
  if (dataBuffer[DEVID] != 0xE5) {
    printf("Problem detected! Device ID is wrong\n");
    return -1;
  }
  return 0;
}

short combineValues(unsigned char msb, unsigned char lsb) {
//   shift the msb right by 8 bits and OR with lsb
  return ((short)msb << 8) | (short)lsb;
}

float a[3];
const float ACC_CONV = 1/(powf(2, 10) / 4);    // acceleration conversion factor (10-bit resolution ±2g) 

int main() {
  int file;
  printf("Starting the ADXL345 sensor application\n");

  file = i2cOpen(i2cBus, i2cAddr);
  if (file < 0)
    exit(file);

  if(writeRegister(file, POWER_CTL, 0x08) <0)  {
    printf("No device at address %02x\n", i2cAddr);
    exit(-1);
  }
  // Setting mode to 00000000=0x00 for +/-2g 10-bit
  // Setting mode to 00001011=0x0B for +/-16g 13-bit
  writeRegister(file, DATA_FORMAT, 0x00);
  readRegistersAll(file);

  printf("The Device ID is: %02x\n", dataBuffer[DEVID]);
  printf("The POWER_CTL mode is: %02x\n", dataBuffer[POWER_CTL]);
  printf("The DATA_FORMAT is %02x\n", dataBuffer[DATA_FORMAT]);

  // Now loop a display the x, y, z accelerometer for 60 seconds
  int count = 0;
  while (count < 60) {
    short x = combineValues(dataBuffer[DATAX1], dataBuffer[DATAX0]);
    short y = combineValues(dataBuffer[DATAY1], dataBuffer[DATAY0]);
    short z = combineValues(dataBuffer[DATAZ1], dataBuffer[DATAZ0]);
		a[0] = ACC_CONV * x;
		a[1] = ACC_CONV * y;
		a[2] = ACC_CONV * z;
    // Use \r and flush to write the output on the same line
//     printf(" X= %d Y= %d Z= %d \r", x, y, z);
		printf("X=%0.2fg Y=%0.2fg Z=%0.2fg\r", a[0], a[1], a[2]);

    fflush(stdout);

    usleep(1000000);
    readRegistersAll(file); // read the sensor again
    count++;
  }
  printf("\n");
  close(file);
  return 0;
}
