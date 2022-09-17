#include "pi-i2c.h"

// ________  BIT operations
int setup_egpio(int gpio, int port, int direction, int pud);
int output_egpio(int gpio, int port, int value);
int input_egpio(int gpio, int port);
// ________  PORT operations
int setup_egpio_port(unsigned port, unsigned dirreg);
int setup_egpio_port_pud(unsigned port, unsigned pudreg);
int output_egpio_port(unsigned port, unsigned datreg);
int input_egpio_port(unsigned port);
// ________  Device operations
int setup_egpio_dev(unsigned dirreg);
int setup_egpio_dev_pud(unsigned pudreg);
int output_egpio_dev(unsigned datreg);
int input_egpio_dev();
