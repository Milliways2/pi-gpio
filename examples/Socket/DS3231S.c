/*
Display status of DS3231 RTC - assumes time in UTC 24 hour mode
Using pi-gpio Socket Interface

TO BUILD

// If building on remote system gcc DS3231S.c pi-sock.c  -o DS3231S
// gcc DS3231S.c -lpi-sock -o DS3231S

*/
// 2023-09-29

#include <stdio.h>
#include <stdlib.h>

// #include "pi-sock.h"	// If building on remote system 
#include <pi-sock.h>

const int i2cBus = 1;
const int i2cAddr = 0x68;

int bcdToDec(char b) { return (b / 16) * 10 + (b % 16); }

// The time and calendar registers are BCD
#define SECONDS 0x00
#define MINUTES 0x01
#define HOURS 0x02
#define DAY 0x03
#define DATE 0x04
#define MONTH 0x05
#define YEAR 0x06
#define A1M1 0x07
#define A1M2 0x08
#define A1M3 0x09
#define A1M4 0x0A
#define A2M2 0x0B
#define A2M3 0x0C
#define A2M4 0x0D
#define CONTROL 0x0E
#define TEMP 0x11
#define TEMPFRAC 0x12

int main(int argc, char *argv[]) {
  char* host = "127.0.1.1";
  if (argc > 1) { host = argv[1]; } // Optional host address
  if(socket_connect(host)<0)  return -1;

  int handle = i2cOpen(i2cBus, i2cAddr);
  if (handle < 0)
    exit(handle); // Note return DOES NOT guarantee device exists at address!
  int secs = i2cRead8(handle, SECONDS); // Detect device exists at address
  if (secs < 0) {
    printf("No device at address %02x\n", i2cAddr);
    exit(-1);
  }
  secs = bcdToDec(secs);
  if (secs < 0)
    exit(secs); // Detect device exists at address
  int mins = bcdToDec(i2cRead8(handle, MINUTES));
  int hours = bcdToDec(i2cRead8(handle, HOURS));
  int days = bcdToDec(i2cRead8(handle, DATE));
  int months = bcdToDec(i2cRead8(handle, MONTH));
  int years = bcdToDec(i2cRead8(handle, YEAR));

  printf("The RTC time is %2d:%02d:%02d\n", hours, mins, secs);
  printf("The RTC date is %2d-%02d-%02d\n", years, months, days);

  float temperature =
      i2cRead8(handle, TEMP) + ((i2cRead8(handle, TEMPFRAC) >> 6) * 0.25);
  printf("The temperature is %.2f°C\n", temperature);

  return 0;
}
