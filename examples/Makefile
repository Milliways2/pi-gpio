#
# Makefile:

ifneq ($V,1)
Q ?= @
endif

#DEBUG	= -g -O0
DEBUG	= -O3
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lpi-gpio -lpthread -lm -lcrypt -lrt

###############################################################################

SRC	=	getInfoTest.c \
		bench-pi-gpio.c  \
		getGPIO.c  \
		pwmTest.c  \
		hwPWMtest.c \
		setGPIO.c \
		getPAD.c

OBJ	=	$(SRC:.c=.o)

BINS	=	$(SRC:.c=)

all:	$(BINS)

.c.o:
	$Q echo [CC] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@

.cc.o:
	$Q echo [Compile] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@

clean:
	$Q echo "[Clean]"
	$Q rm -f $(OBJ) *~ core tags $(BINS)

