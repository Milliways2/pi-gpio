//
// Gertboard Demo
//
// SPI control code
//
// This code is part of the Gertboard test suite
// These routines access the DA chip
//
//
// Copyright (C) Gert Jan van Loo & Myra VanInwegen 2012
// No rights reserved
// You may treat this program as if it was in the public domain
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//
// Try to strike a balance between keep code simple for
// novice programmers but still have reasonable quality code
//

#include "gb_common.h"
#include "gb_spi.h"

// Set GPIO pins to the right mode
// DEMO GPIO mapping:
//         Function            Mode
// GPIO0=  unused
// GPIO1=  unused
// GPIO4=  unused
// GPIO7=  SPI chip select B   Alt. 0
// GPIO8=  unused
// GPIO9=  SPI MISO            Alt. 0
// GPIO10= SPI MOSI            Alt. 0
// GPIO11= SPI CLK             Alt. 0
// GPIO14= unused
// GPIO15= unused
// GPIO17= unused
// GPIO18= unused
// GPIO21= unused
// GPIO22= unused
// GPIO23= unused
// GPIO24= unused
// GPIO25= unused
//

// For D to A we only need the SPI bus and SPI chip select B
// void setup_gpio()
// {
//    INP_GPIO(7);  SET_GPIO_ALT(7,0);
//    INP_GPIO(9);  SET_GPIO_ALT(9,0);
//    INP_GPIO(10); SET_GPIO_ALT(10,0);
//    INP_GPIO(11); SET_GPIO_ALT(11,0);
// } // setup_gpio

CHANNEL = 0;

//
//  Read ADC input 0 and show as horizontal bar
//
void main(void)
{ int d, chan, dummy;

//   do {
//     printf ("Which channel do you want to test? Type 0 or 1.\n");
//     chan  = (int) getchar();
//     (void) getchar(); // eat carriage return
//   } while (chan != '0' && chan != '1');
//   chan = chan - '0';

chan = CHANNEL;
//   printf ("These are the connections for the digital to analogue test:\n");
//   printf ("jumper connecting GP11 to SCLK\n");
//   printf ("jumper connecting GP10 to MOSI\n");
//   printf ("jumper connecting GP9 to MISO\n");
//   printf ("jumper connecting GP7 to CSnB\n");
//   printf ("Multimeter connections (set your meter to read V DC):\n");
//   printf ("  connect black probe to GND\n");
//   printf ("  connect red probe to DA%d on J29\n", chan);
//   printf ("When ready hit enter.\n");
//   (void) getchar();

  // Map the I/O sections
  setup_io();

  // activate SPI bus pins
  setup_gpio();

  // Setup SPI bus
  setup_spi();

  // Most likely, the DAC you have installed is an 8 bit one, not 12 bit so
  // it will ignore that last nibble (4 bits) we send down the SPI interface.
  // So the number that we pass to write_dac will need to be the number
  // want to set (between 0 and 255) multiplied by 16. In hexidecimal,
  // we just put an extra 0 after the number we want to set.
  // So if we want to set the DAC to 64, this is 0x40, so we send 0x400
  // to write_dac.

  // To calculate the voltage we get out, we use this formula from the
  // datasheet: V_out = (d / 256) * 2.048

//   d = 0x000;
//   write_dac(chan, d);
//   // V_out = 0 / 256 * 2.048 (gives 0)
//   printf ("Your meter should read about 0V\n");
//   printf ("When ready hit enter.\n");
//   (void) getchar();
//
//   d = 0x400;
//   write_dac(chan, d);
//   // V_out = 64 / 256 * 2.048 (gives 0.512)
//   printf ("Your meter should read about 0.5V\n");
//   printf ("When ready hit enter.\n");
//   (void) getchar();

  d = 0x7F0;
  write_dac(chan, d);
  // V_out = 127 / 256 * 2.048 (gives 1.016)
//   printf ("Your meter should read about 1.02V\n");
//   printf ("When ready hit enter.\n");
//   (void) getchar();

//   d = 0xAA0;
//   write_dac(chan, d);
//   // V_out = 170 / 256 * 2.048 (gives 1.36)
//   printf ("Your meter should read about 1.36V\n");
//   printf ("When ready hit enter.\n");
//   (void) getchar();
//
//   d = 0xFF0;
//   write_dac(chan, d);
//   // V_out = 255 / 256 * 2.048 (gives 2.04)
//   printf ("Your meter should read about 2.04V\n");
//   printf ("When ready hit enter.\n");
//   (void) getchar();
//
//   restore_io();
} // main
