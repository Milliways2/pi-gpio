# Makefile:

ifneq ($V,1)
Q ?= @
endif

#DEBUG	= -g -O0
DEBUG	= -O3
CFLAGS	= $(DEBUG) -Wall  -Winline -pipe

LDLIBS    = -lpi-gpio

###############################################################################ˆ˙

SRC	=	DS3231.c \
		ADXL345.c \
		testMCP23017.c \
		egpio.c \
		bench-egpio.c
		
OBJ	=	$(SRC:.c=.o)

BINS	=	$(SRC:.c=)
# BINS	=	$(SRC:.c=.cpp=)

all:	$(BINS)

.c.o:
	$Q echo [CC] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@

# .cpp.o:
# 	$Q echo [CC] $<
# 	$Q $(CC) -c $(CFLAGS) $< -o $@

clean:
	$Q echo "[Clean]"
	$Q rm -f *.o *~ core tags $(BINS)
