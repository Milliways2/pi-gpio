# Makefile:
#	pi-gpio 	library for the Raspberry Pi
#	pi-gpiod	Socket Server

#	Install tools
#	pi_gpio 	Python overlay
#	pi_sock 	Python Socket overlay
#	gpioStatus	Display status of all GPIO

# 2024-02-29

VERSION=$(shell cat ../VERSION)
DESTDIR?=/usr
PREFIX?=/local
PYLIB?=/usr/lib/python3/dist-packages

LDCONFIG?=ldconfig
V=1
ifneq ($V,1)
Q ?= @
endif

LIBNAME=pi-gpio

DYNAMIC=lib$(LIBNAME).so.$(VERSION)

#DEBUG	= -g -O0
DEBUG	= -O2
INCLUDE	= -I.
DEFS	= -D_GNU_SOURCE
CFLAGS	= $(DEBUG) $(DEFS) -Wformat=2 -Wall -Wextra -Winline $(INCLUDE) -Wno-pointer-to-int-cast -pipe -fPIC
# CFLAGS	= $(DEBUG) $(DEFS) -Wformat=2 -Wall -Wextra -Winline $(INCLUDE) -pipe -fPIC
LIBS    = -lm -lpthread -lrt -lcrypt

###############################################################################

SRC	=	c_gpio.c \
		cpuinfo.c \
		extras.c \
		soft_pwm.c \
		hard_pwm.c \
		kpwm.c \
		spi.c \
		i2c.c \
		pi-vers.h \
		MCP23017.c

# 		event_gpio.c \

HEADERS = $(LIBNAME).h pi-vers.h pi-spi.h pi-i2c.h MCP23017.h
MANPAGES = $(LIBNAME).3 MCP23017.3


OBJ	=	$(SRC:.c=.o)

all:	$(DYNAMIC) pi-gpiod libpi-sock.so

# Make pi-gpiod Socket Server code.
pi-gpiod: pi-gpiod.c process_data.c
	@ echo "[Make pi-gpiod Socket Server code]"
	$(CC) pi-gpiod.c process_data.c -o $@ -lpi-gpio -lpthread -lm -lcrypt -lrt
# 	$(CC) pi-gpiod.c process_data.c -o $@ -lpthread -lm -lcrypt -lrt

pi-vers.h:	../VERSION
	echo "#define VERSION \"`cat ../VERSION`\"" > pi-vers.h

libpi-sock.so: pi-sock.c pi-sock.h
	$(CC) -shared -Wl,-soname,libpi-sock.so  -fPIC -o libpi-sock.so.$(VERSION) -lm -lrt pi-sock.c 


$(DYNAMIC):	$(OBJ)
	$Q $(CC) -shared -Wl,-soname,lib$(LIBNAME).so -o lib$(LIBNAME).so.$(VERSION) $(LIBS) $(OBJ)

.c.o:
	@ echo [Compile] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@

.PHONY:	clean
clean:
	@ echo "[Clean]"
	$Q rm -f $(OBJ) $(OBJ_I2C) *~ core tags lib$(LIBNAME).*
	$Q rm libpi-sock.*
	$Q rm pi-gpiod

.PHONY:	install
install:	$(DYNAMIC)
	@ echo "[Install Headers]"
	$Q install -m 0755 -d $(DESTDIR)$(PREFIX)/include
	$Q install -m 0644 $(HEADERS) $(DESTDIR)$(PREFIX)/include

	@ echo "[Install Dynamic Lib]"
	$Q install -m 0755 -d $(DESTDIR)$(PREFIX)/lib
	$Q install -m 0755 lib$(LIBNAME).so.$(VERSION) $(DESTDIR)$(PREFIX)/lib/lib$(LIBNAME).so.$(VERSION)
	$Q ln -sf $(DESTDIR)$(PREFIX)/lib/lib$(LIBNAME).so.$(VERSION)	$(DESTDIR)/lib/lib$(LIBNAME).so
	$Q $(LDCONFIG)

	@ echo "[Install man pages]"
	$Q mkdir -p $(DESTDIR)$(PREFIX)/share/man/man3
	$Q install -m 0644 $(MANPAGES) 	$(DESTDIR)$(PREFIX)/share/man/man3

	@ echo "[Install python overlays]"
	mkdir -p $(DESTDIR)/usr/lib/python3/dist-packages/
	install -m 0644 pi_gpio.py 	$(PYLIB)/pi_gpio.py
	install -m 0644 pi_spi.py 	$(PYLIB)/pi_spi.py
	install -m 0644 pi_i2c.py 	$(PYLIB)/pi_i2c.py
	install -m 0644 pi_sock.py 	$(PYLIB)/pi_sock.py

	@ echo "[Install gpioStatus]"
	install -o root -m 0755 -p gpioStatus	$(DESTDIR)$(PREFIX)/bin/gpioStatus

.PHONY:	uninstall
uninstall:
	@ echo "[UnInstall]"
	$Q cd $(DESTDIR)$(PREFIX)/include/ && rm -f $(HEADERS)
	$Q cd $(DESTDIR)$(PREFIX)/lib/     && rm -f lib$(LIBNAME).*
	$Q $(LDCONFIG)

	$Q cd $(DESTDIR)$(PREFIX)/share/man/man3/     && rm -f $(MANPAGES)
