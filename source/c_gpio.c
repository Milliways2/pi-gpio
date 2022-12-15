/*
Copyright (c) 2012-2021 Ben Croston

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/*
Enhanced functionality by Ian Binnie (based on RPi.GPIO 0.7.0 by Ben Croston)
Includes code inspired by pigpio & wiringpi
2022-08-16
2022-11-19	Fix setup_gpio direction
*/

#include "c_gpio.h"
#include "cpuinfo.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define BCM2708_PERI_BASE_DEFAULT 0x20000000
#define BCM2709_PERI_BASE_DEFAULT 0x3f000000
#define BCM2710_PERI_BASE_DEFAULT 0x3f000000
#define BCM2711_PERI_BASE_DEFAULT 0xfe000000

#define PADS_BASE_OFFSET 0x100000
#define GPIO_BASE_OFFSET 0x200000
#define CLOCK_BASE_OFFSET 0x101000
#define TIMER_OFFSET 0x00B000
#define PWM_OFFSET 0x20C000

#define FSEL_OFFSET 0          // 0x0000
#define SET_OFFSET 7           // 0x001c / 4
#define CLR_OFFSET 10          // 0x0028 / 4
#define PINLEVEL_OFFSET 13     // 0x0034 / 4
#define EVENT_DETECT_OFFSET 16 // 0x0040 / 4
#define RISING_ED_OFFSET 19    // 0x004c / 4
#define FALLING_ED_OFFSET 22   // 0x0058 / 4
#define HIGH_DETECT_OFFSET 25  // 0x0064 / 4
#define LOW_DETECT_OFFSET 28   // 0x0070 / 4
#define PULLUPDN_OFFSET 37     // 0x0094 / 4
#define PULLUPDNCLK_OFFSET 38  // 0x0098 / 4

#define PULLUPDN_OFFSET_2711_0 57
#define PULLUPDN_OFFSET_2711_1 58
#define PULLUPDN_OFFSET_2711_2 59
#define PULLUPDN_OFFSET_2711_3 60

#define PAGE_SIZE (4 * 1024)
#define BLOCK_SIZE (4 * 1024)
#define PADS_LEN 0x38 // from pigpiod.c
#define PWM_LEN 0x28  // from pigpiod.c
#define CLK_LEN 0xA8  // from pigpiod.c

volatile uint32_t *gpio_map;
int piSetup = 0;
int piMemSetup = 0;

static volatile unsigned int pads_base;
static volatile unsigned int pwm_base;
static volatile unsigned int clk_base;

static volatile uint32_t *pads_map = 0;
volatile uint32_t *pwm_map = 0;
volatile uint32_t *clk_map = 0;

rpi_info rpiinfo;

#define BCM_PASSWORD 0x5A000000

void short_wait(void) {
  int i;

  for (i = 0; i < 150; i++) { // wait 150 cycles
    asm volatile("nop");
  }
}

/*
	Since kernel 4.1.7 (2015) character device /dev/gpiomem maps the GPIO register page.

	/dev/gpiomem allows GPIO access without root to members of the gpio group.
	This does not provide access the PWM or clock hardware registers.

	The alternative is to use /dev/mem which requires root and determining
	the address of the GPIO peripheral which varies depending on SoC.

	This function will use /dev/mem if invoked with root privileges and try
	/dev/gpiomem if not.
*/

int map_gpio_mem(int mem_fd, uint32_t gpio_base) {
  uint8_t *gpio_mem; //	The base address of the GPIO memory mapped hardware IO
  if ((gpio_mem = (uint8_t *)malloc(BLOCK_SIZE + (PAGE_SIZE - 1))) == NULL)
    return SETUP_MALLOC_FAIL;

  if ((uint32_t)gpio_mem % (uint32_t)PAGE_SIZE)
    gpio_mem += PAGE_SIZE - (uint32_t)gpio_mem % PAGE_SIZE;

  if ((gpio_map = (uint32_t *)mmap(
           (void *)gpio_mem, BLOCK_SIZE, PROT_READ | PROT_WRITE,
           MAP_SHARED | MAP_FIXED, mem_fd, gpio_base)) == MAP_FAILED)
    return SETUP_MMAP_FAIL;
  piSetup = 1;
  return 0;
}

int setup(void) {
  // Initialise pi-gpio
  // Must be called before any other function except get_rpi_info.
  int mem_fd;
  int map_result;
  uint32_t peri_base = 0;
  uint32_t gpio_base;
  char hardware[1024];

  if (piSetup)
    return SETUP_OK; // already initialised

  if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0) {
    if ((mem_fd = open("/dev/gpiomem", O_RDWR | O_SYNC | O_CLOEXEC)) > 0)
      return map_gpio_mem(mem_fd, 0);
  }

  // Use board revision already detected
  get_rpi_info(&rpiinfo);

  if (strcmp(rpiinfo.processor, "BCM2708") == 0 ||
      strcmp(hardware, "BCM2835") == 0) {
    peri_base = BCM2708_PERI_BASE_DEFAULT;
  } else if (strcmp(rpiinfo.processor, "BCM2709") == 0 ||
             strcmp(rpiinfo.processor, "BCM2836") == 0) {
    peri_base = BCM2709_PERI_BASE_DEFAULT;
  } else if (strcmp(rpiinfo.processor, "BCM2710") == 0 ||
             strcmp(rpiinfo.processor, "BCM2837") == 0) {
    peri_base = BCM2710_PERI_BASE_DEFAULT;
  } else if (strcmp(rpiinfo.processor, "BCM2711") == 0) {
    peri_base = BCM2711_PERI_BASE_DEFAULT;
  }

  if (!peri_base)
    return SETUP_NOT_RPI_FAIL;
  gpio_base = peri_base + GPIO_BASE_OFFSET;

  map_result = map_gpio_mem(mem_fd, gpio_base);
  if (map_result)
    return map_result;

  pads_base = peri_base + PADS_BASE_OFFSET;
  pads_map = (uint32_t *)mmap(0, PADS_LEN, PROT_READ | PROT_WRITE | PROT_EXEC,
                              MAP_SHARED | MAP_LOCKED, mem_fd, pads_base);
  if (pads_map == MAP_FAILED)
    printf("mmap (PADS) failed\n");

  pwm_base = peri_base + PWM_OFFSET;
  pwm_map = (uint32_t *)mmap(0, PWM_LEN, PROT_READ | PROT_WRITE | PROT_EXEC,
                             MAP_SHARED | MAP_LOCKED, mem_fd, pwm_base);
  if (pwm_map == MAP_FAILED)
    printf("mmap (PWM) failed\n");

  //	Clock control (needed for PWM)
  clk_base = peri_base + CLOCK_BASE_OFFSET;
  clk_map = (uint32_t *)mmap(0, CLK_LEN, PROT_READ | PROT_WRITE, MAP_SHARED,
                             mem_fd, clk_base);
  if (clk_map == MAP_FAILED)
    printf("mmap (CLK) failed\n");

  piMemSetup = 1;
  return SETUP_OK;
}

void cleanup(void) {
  if (piSetup)
    munmap((void *)gpio_map, BLOCK_SIZE);
  if (pads_map)
    munmap((void *)pads_map, PADS_LEN);
  if (pwm_map)
    munmap((void *)pwm_map, PWM_LEN);
//   if (pads_map) 2022-12-13 BUG!
  if (clk_map)
    munmap((void *)clk_map, CLK_LEN);
}
// -----------------------------

static void clear_event_detect(int gpio) {
  int offset = EVENT_DETECT_OFFSET + (gpio / 32);
  int shift = (gpio % 32);

  *(gpio_map + offset) |= (1 << shift);
  short_wait();
  *(gpio_map + offset) = 0;
}

int eventdetected(int gpio) {
  int offset, value, bit;

  offset = EVENT_DETECT_OFFSET + (gpio / 32);
  bit = (1 << (gpio % 32));
  value = *(gpio_map + offset) & bit;
  if (value)
    clear_event_detect(gpio);
  return value;
}

void set_rising_event(int gpio, int enable) {
  int offset = RISING_ED_OFFSET + (gpio / 32);
  int shift = (gpio % 32);

  if (enable)
    *(gpio_map + offset) |= 1 << shift;
  else
    *(gpio_map + offset) &= ~(1 << shift);
  clear_event_detect(gpio);
}

void set_falling_event(int gpio, int enable) {
  int offset = FALLING_ED_OFFSET + (gpio / 32);
  int shift = (gpio % 32);

  if (enable) {
    *(gpio_map + offset) |= (1 << shift);
    *(gpio_map + offset) = (1 << shift);
  } else {
    *(gpio_map + offset) &= ~(1 << shift);
  }
  clear_event_detect(gpio);
}

void set_high_event(int gpio, int enable) {
  int offset = HIGH_DETECT_OFFSET + (gpio / 32);
  int shift = (gpio % 32);

  if (enable)
    *(gpio_map + offset) |= (1 << shift);
  else
    *(gpio_map + offset) &= ~(1 << shift);
  clear_event_detect(gpio);
}

void set_low_event(int gpio, int enable) {
  int offset = LOW_DETECT_OFFSET + (gpio / 32);
  int shift = (gpio % 32);

  if (enable)
    *(gpio_map + offset) |= 1 << shift;
  else
    *(gpio_map + offset) &= ~(1 << shift);
  clear_event_detect(gpio);
}
// -----------------------------

void setupCheck(void) {
  if (piSetup)
    return;
  fprintf(stderr, "Pi Setup failure\n");
  exit(SETUP_NOT_RPI_FAIL);
}

void setupMemCheck(void) {
  if (piMemSetup)
    return;
  fprintf(stderr, "No access to /dev/mem.  Try running as root!\n");
  exit(SETUP_NOT_RPI_FAIL);
}

static void set_pullupdn(int gpio, int pud) {
  // Check GPIO register
  int is2711 = *(gpio_map + PULLUPDN_OFFSET_2711_3) != 0x6770696f;
  if (is2711) {
    // Pi 4 Pull-up/down method
    int pullreg = PULLUPDN_OFFSET_2711_0 + (gpio >> 4);
    int pullshift = (gpio & 0xf) << 1;
    unsigned int pullbits;
    unsigned int pull = 0;
    switch (pud) {
    case PUD_OFF:
      pull = 0;
      break;
    case PUD_UP:
      pull = 1;
      break;
    case PUD_DOWN:
      pull = 2;
      break;
    default:
      pull = 0; // switch PUD to OFF for other values
    }
    pullbits = *(gpio_map + pullreg);
    pullbits &= ~(3 << pullshift);
    pullbits |= (pull << pullshift);
    *(gpio_map + pullreg) = pullbits;
  } else {
    // Legacy Pull-up/down method
    int clk_offset = PULLUPDNCLK_OFFSET + (gpio / 32);
    int shift = (gpio % 32);

    if (pud == PUD_DOWN) {
      *(gpio_map + PULLUPDN_OFFSET) =
          (*(gpio_map + PULLUPDN_OFFSET) & ~3) | PUD_DOWN;
    } else if (pud == PUD_UP) {
      *(gpio_map + PULLUPDN_OFFSET) =
          (*(gpio_map + PULLUPDN_OFFSET) & ~3) | PUD_UP;
    } else { // pud == PUD_OFF
      *(gpio_map + PULLUPDN_OFFSET) &= ~3;
    }
    short_wait();
    *(gpio_map + clk_offset) = 1 << shift;
    short_wait();
    *(gpio_map + PULLUPDN_OFFSET) &= ~3;
    *(gpio_map + clk_offset) = 0;
  }
}

int get_pullupdn(int gpio) {
  //        Return the current GPIO pull
  //        0:None/Unknown
  //        1:Up Pi4 only
  //        2:Down Pi4 only
  setupCheck();
  // Check GPIO register
  int is2711 = *(gpio_map + PULLUPDN_OFFSET_2711_3) != 0x6770696f;
  if (is2711) {
    // Pi 4 Pull-up/down method
    int pullreg = PULLUPDN_OFFSET_2711_0 + (gpio >> 4);
    int pullshift = (gpio & 0xf) << 1;
    unsigned int pullbits;
    pullbits = *(gpio_map + pullreg);
    pullbits &= (3 << pullshift);
    pullbits >>= pullshift;
    return pullbits;
  } else {
    return 0;
  }
}

void setup_gpio(int gpio, int direction, int pud) {
  // Set gpio as an input or an output
  //        direction: 0=IN, 1=OUT
  //        pud: 0=None 1=Up 2=Down
  setupCheck();
  int offset = FSEL_OFFSET + (gpio / 10);
  int shift = (gpio % 10) * 3;
  set_pullupdn(gpio, pud);
  if (direction) { //  OUTPUT
    *(gpio_map + offset) =
        (*(gpio_map + offset) & ~(7 << shift)) | (1 << shift);
  } else { // INPUT
    *(gpio_map + offset) = (*(gpio_map + offset) & ~(7 << shift));
  }
}

int gpio_function(int gpio) {
  //        Returns the current GPIO mode
  //        Returns 0-7 (IN, OUT, ALT5, ALT4, ALT0, ALT1, ALT2, ALT3)
  setupCheck();
  int offset = FSEL_OFFSET + (gpio / 10);
  int shift = (gpio % 10) * 3;
  int function = *(gpio_map + offset);
  function >>= shift;
  function &= 7;
  return function;
}

void output_gpio(int gpio, int value) {
  //        Output to a GPIO channel
  //        value - 0/1 or False/True or LOW/HIGH
  int offset, shift;

  setupCheck();
  if (value) // value == HIGH
    offset = SET_OFFSET + (gpio / 32);
  else // value == LOW
    offset = CLR_OFFSET + (gpio / 32);

  shift = (gpio % 32);

  *(gpio_map + offset) = 1 << shift;
}

int input_gpio(int gpio) {
  //        Returns the GPIO level.
  //        Returns HIGH=1=True or LOW=0=False
  unsigned int offset, value, mask;

  setupCheck();
  offset = PINLEVEL_OFFSET + (gpio / 32);
  mask = (1 << gpio % 32);
  value = *(gpio_map + offset) & mask;
  return value ? 1 : 0;
}

int input_28(void) {
  //        Returns value of GPIO 0-27
  setupCheck();
  return *(gpio_map + PINLEVEL_OFFSET) & 0x0FFFFFFF;
}

void output_28(unsigned bits, unsigned mask) {
  //        Sets value of GPIO 0-27
  //        bits: 28 bit values to set; each bit 0/1
  //        mask: 28 bit mask specifying GPIO to set
  setupCheck();
  *(gpio_map + SET_OFFSET) = bits & mask;
  *(gpio_map + CLR_OFFSET) = ~bits & mask;
}

// -----------------------------

int getPAD(unsigned group) {
  setupMemCheck();
  return (*(pads_map + group + 11)) & 0x1f;
}

void setPAD(unsigned group, unsigned padstate) {
  setupMemCheck();
  *(pads_map + group + 11) = BCM_PASSWORD | (padstate & 0x1f);
}
