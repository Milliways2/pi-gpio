// This is a program to test socket equivalents of pi-gpio functions
// It translates function calls into messages, sends to socket server and receives responses.

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include "pi-sock.h"

// 2023-09-10
int obj_socket = 0;
#define BLEN 127
char message[BLEN] = {0};	// Message to send to server
char buffer[BLEN] = {0};	// Message from server

char *test_server(char *message) {
// Send message to server & return result
  printf("Test of \t%s\n", message);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
	return buffer;
}

// **********************

//    Information

unsigned get_revision(void) {
	char *message = "get_revision";
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}

int get_rpi_info(rpi_info *info) {
	sprintf(message, "get_rpi_info");
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
	char delim[] = ":,";
	char delim2[] = ":,'";
	char *str1 = strtok(buffer, "{}");

	strtok(str1, delim);	// skip
	info->p1_revision = atoi(strtok(NULL, delim));
	strtok(NULL, delim);	// skip
	info->type = strtok(NULL, delim2);	// remove python '' string delimiters
	strtok(NULL, delim);	// skip
	info->ram= strtok(NULL, delim2);
	strtok(NULL, delim);	// skip
	info->processor = strtok(NULL, delim2);
	strtok(NULL, delim);	// skip
	info->manufacturer = strtok(NULL, delim2);
	strtok(NULL, delim);	// skip
	strcpy(info->revision, strtok(NULL, delim2));
	return 0;
}

//    GPIO
int input_gpio(int gpio) {
	sprintf(message, "input_gpio %d", gpio);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}
int gpio_function(int gpio) {
	sprintf(message, "gpio_function %d", gpio);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}
int get_pullupdn(int gpio) {
	sprintf(message, "get_pullupdn %d", gpio);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}
void setup_gpio(int gpio, int direction, int pud) {
	sprintf(message, "setup_gpio %d %d %d", gpio, direction, pud);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
}
void output_gpio(int gpio, int value) {
	sprintf(message, "output_gpio %d %d", gpio, value);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
}
int input_28(void) {
	char *message = "input_28";
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}
void output_28(unsigned bits, unsigned mask) {
	sprintf(message, "output_28 %d %d", bits, mask);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
}

// PAD
int getPAD(unsigned group) {
	sprintf(message, "getPAD %d", group);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
	}
void setPAD(unsigned group, unsigned padstate) {
	sprintf(message, "setPAD %d %d", group, padstate);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
}

// Hardware PWM
int pwmSetGpio(int gpio) {
	sprintf(message, "pwmSetGpio %d", gpio);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}
void pwmSetMode(int mode) {
	sprintf(message, "pwmSetMode %d", mode);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
}
void pwmSetClock(int divisor) {
	sprintf(message, "pwmSetClock %d", divisor);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
}
int pwmSetRange(int gpio, unsigned int range) {
	sprintf(message, "pwmSetRange %d %d", gpio, range);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}
int pwmWrite(int gpio, int value) {
	sprintf(message, "pwmWrite %d %d", gpio, value);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}
int pwmSetDutycycle(int pin, float duty_cycle) {
	sprintf(message, "pwmSetDutycycle %d %f", pin, duty_cycle);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}
unsigned int pwmGetRange(int gpio) {
	sprintf(message, "pwmGetRange %d", gpio);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}


// I2C
int i2cOpen(unsigned i2cBus, unsigned i2cAddr) {
	sprintf(message, "i2cOpen %d %d", i2cBus, i2cAddr);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}
int i2cRead(unsigned handle) {
	sprintf(message, "i2cRead %d", handle);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}
int i2cRead8(unsigned handle, unsigned i2cReg) {
	sprintf(message, "i2cRead8 %d %d", handle, i2cReg);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}
int i2cRead16(unsigned handle, unsigned i2cReg) {
	sprintf(message, "i2cRead16 %d %d", handle, i2cReg);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}
int i2cWrite(unsigned handle, int data) {
	sprintf(message, "i2cWrite %d %d", handle, data);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}
int i2cWrite8(unsigned handle, unsigned i2cReg, int data) {
	sprintf(message, "i2cWrite8 %d %d %d", handle, i2cReg, data);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}
int i2cWrite16(unsigned handle, unsigned i2cReg, int data) {
	sprintf(message, "i2cWrite16 %d %d %d", handle, i2cReg, data);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}

// Software PWM
void pwm_set_duty_cycle(unsigned int gpio, float dutycycle) {
	sprintf(message, "pwm_set_duty_cycle %d %f", gpio, dutycycle);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
}
void pwm_set_frequency(unsigned int gpio, float freq) {
	sprintf(message, "pwm_set_frequency %d %f", gpio, freq);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
}
void pwm_start(unsigned int gpio) {
	sprintf(message, "pwm_start %d", gpio);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
}
void pwm_stop(unsigned int gpio) {
	sprintf(message, "pwm_stop %d", gpio);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
}
int pwm_exists(unsigned int gpio) {
	sprintf(message, "pwm_exists %d", gpio);
  send(obj_socket, message, strlen(message), 0);	// Send message
  read(obj_socket, buffer, BLEN);
  return(atoi(buffer));
}

// **********************
int socket_connect(char* host) {
  // Connect to pi-gpio socket interface
  struct sockaddr_in serv_addr;
  if ((obj_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Socket creation error !");
    return -1;
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  // Converting IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, host, &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address ! This IP Address is not supported !\n");
    return -1;
  }
  if (connect(obj_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("Connection Failed : Can't establish a connection over this socket !\n");
    return -1;
  }
	return 0;
}

void socket_disconnect(void) {
	close(obj_socket);
}
