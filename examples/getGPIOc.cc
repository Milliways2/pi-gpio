#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

#include <pi-gpio.h>
/*
List GPIO, Value, Function, Pullup/down (Pi4 only) for GPIO 0-27
*/

struct GPIOvalue {
	int value;
	int function;
	int pull;
};

struct GPIOvalue getGPIO(int g)
{
	GPIOvalue gp;
	gp.value = input_gpio(g);
	gp.function = gpio_function(g);
	gp.pull = (gp.function==0) ? get_pullupdn(g) : 0;	// only input
	return gp;
}

int main ()
{
	setup();
	struct GPIOvalue gp;

	for(int i=0; i<28; i++)
	{
		gp = getGPIO(i);
		printf("GPIO %2i Value=%i Function %i", i, gp.value, gp.function);
		if(gp.function==0)
			cout << " Pull " << gp.pull;
		cout << endl;
	}

	return 0 ;
}
