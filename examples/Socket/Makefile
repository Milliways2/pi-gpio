#
# Makefile:

ifneq ($V,1)
Q ?= @
endif

CC = gcc

#DEBUG	= -g -O0
DEBUG	= -O3
CFLAGS	= $(DEBUG) -Wall  -Winline

# change to use local vs system
# LDFLAGS	= -L/usr/local/lib
LDFLAGS	= -L/usr/lib
LIBS    = -lpi-gpio -lpthread -lm -lcrypt -lrt

###############################################################################

# bench-pi-gpioS: bench-pi-gpioS.c pi-sock.c pi-sock.h
bench-pi-gpioS: bench-pi-gpioS.c
	$(CC) $(CFLAGS) $<  pi-sock.c  -o $@

# testS: testS.c pi-sock.c pi-sock.h
testS: testS.c
	$(CC) $(CFLAGS) $<  pi-sock.c  -o $@

	
SRC	=	bench-pi-gpioS.c\
		testS.c

# OBJ	=	$(SRC:.c=.o)

BINS	=	$(SRC:.c=)

all:	$(BINS)
# 
# .c.o:
# 	$Q echo [CC] $<
# 	$Q $(CC) -c $(CFLAGS) $< -o $@

clean:
	$Q echo "[Clean]"
	$Q rm -f $(OBJ) *~ core tags $(BINS)

