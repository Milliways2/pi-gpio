# Makefile:

ifneq ($V,1)
Q ?= @
endif


CC = gcc

#DEBUG	= -g -O0
DEBUG	= -O3
CFLAGS	= $(DEBUG) -Wall  -Winline

# Define REMOTE to compile on remote system
REMOTE = 

ifdef REMOTE
objects :=  pi-sock.c 
LIBS =
else
objects :=  
LIBS = -lpi-sock
endif

###############################################################################

bench-pi-gpioS: bench-pi-gpioS.c  pi-sock.c pi-sock.h
	$(CC) $(CFLAGS) $< $(objects) $(LIBS) -o $@

testS: testS.c  pi-sock.c pi-sock.h
	$(CC) $(CFLAGS) $< $(objects) $(LIBS) -o $@

DS3231S: DS3231S.c  pi-sock.c pi-sock.h
	$(CC) $(CFLAGS) $< $(objects) $(LIBS) -o $@

pwmTestS: pwmTestS.c  pi-sock.c pi-sock.h
	$(CC) $(CFLAGS) $< $(objects) $(LIBS) -o $@

	
SRC	=	bench-pi-gpioS.c\
		testS.c\
		DS3231S.c\
		pwmTestS.c

# OBJ	=	$(SRC:.c=.o)

BINS	=	$(SRC:.c=)

all:	$(BINS)

clean:
	$Q echo "[Clean]"
	$Q rm -f $(OBJ) *~ core tags $(BINS)

