// Server side code to respond to client GPIO messages.
// Handle multiple socket connections with select and fd_set on Linux
// https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/
// https://man.archlinux.org/man/core/man-pages/FD_ZERO.3.en
// Compile: gcc pi-gpiod.c process_data.c -o pi-gpiod -lpi-gpio -lpthread -lm -lcrypt -lrt

// 2023-09-12

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h> //close
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h> //close

#include <syslog.h>

#include "process_data.h"
#include "pi-gpio.h"

#define TRUE 1
#define FALSE 0
#define PORT 31425
#define BLEN 128
char buffer[BLEN]; // input data buffer

int main(int argc, char *argv[]) {
  int opt = TRUE;
  int master_socket, addrlen, new_socket, client_socket[30],
      max_clients = 30, activity, i, valread, sd;
  int max_sd;
  struct sockaddr_in address;

  fd_set readfds; // set of socket descriptors
  // initialise all client_socket[] to 0 so not checked
  for (i = 0; i < max_clients; i++) {
    client_socket[i] = 0;
  }
  // create a master socket
  if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  // set master socket to allow multiple connections
  if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  // type of socket created
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; // allow ANY server to connect
  address.sin_port = htons(PORT);
  // bind the socket to localhost PORT
  if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  // try to specify maximum of 3 pending connections for the master socket
  if (listen(master_socket, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  /* Open the log file */
  openlog("pi-gpiod", LOG_PID, LOG_DAEMON);
  syslog (LOG_NOTICE, "pi-gpiod started.");

  // accept the incoming connection
  addrlen = sizeof(address);

  setup(); // Initialise pi-gpio
  // change to the "/" directory
  int nochdir = 0;

  // redirect standard input, output and error to /dev/null
  // this is equivalent to "closing the file descriptors"
  int noclose = 0;

  // glibc call to daemonize this process without a double fork
  if(daemon(nochdir, noclose))
    perror("daemon");

  // our process is now a daemon!

//   **********************
  while (TRUE) {
    FD_ZERO(&readfds);               // clear the socket set
    FD_SET(master_socket, &readfds); // add master socket to set

    max_sd = master_socket;

    // add child sockets to set
    for (i = 0; i < max_clients; i++) {
      sd = client_socket[i]; // socket descriptor
      if (sd > 0)
        FD_SET(sd, &readfds); // if valid socket descriptor then add to read list
			if (sd > max_sd)
        max_sd = sd; // highest file descriptor number, for the select function
    }

    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL); // wait for an activity on one of the sockets
    if ((activity < 0) && (errno != EINTR)) {
      printf("select error");
    }

    // If something happened on the master socket, then its an incoming connection
    if (FD_ISSET(master_socket, &readfds)) {
      if ((new_socket = accept(master_socket, (struct sockaddr *)&address,
                               (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
      }

      // add new socket to array of sockets
      for (i = 0; i < max_clients; i++) {
        if (client_socket[i] == 0) {
          client_socket[i] = new_socket;
          break;
        }
      }
    }
    // else its some I/O operation on some other socket
    for (i = 0; i < max_clients; i++) {
      sd = client_socket[i];
      if (FD_ISSET(sd, &readfds)) {
        // Check if it was for closing , and also read the incoming message
        if ((valread = read(sd, buffer, BLEN)) == 0) {
          getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
          close(sd); // Close the socket and mark as 0
          client_socket[i] = 0;
        } else {
          buffer[valread] = '\0'; // add string terminator
          process_data(buffer);	// Process instruction received from socket
          send(sd, out_buffer, strlen(out_buffer) + 1, 0); // Send response
        }
      }
    }
  }

  return 0;
}
