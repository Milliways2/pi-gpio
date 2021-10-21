
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static uint64_t epochMilli, epochMicro ;

void initialiseTimers (void)
{
  struct timespec ts ;
  clock_gettime (CLOCK_MONOTONIC_RAW, &ts) ;
  epochMilli = (uint64_t)ts.tv_sec * (uint64_t)1000    + (uint64_t)(ts.tv_nsec / 1000000L) ;
  epochMicro = (uint64_t)ts.tv_sec * (uint64_t)1000000 + (uint64_t)(ts.tv_nsec /    1000L) ;
}

/*
 * micros:
 *	Return a number of microseconds as an unsigned int.
 *	Wraps after 71 minutes.
 */
unsigned int micros(void) {
  uint64_t now;
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  now = (uint64_t)ts.tv_sec * (uint64_t)1000000 + (uint64_t)(ts.tv_nsec / 1000);
  return (uint32_t)(now - epochMicro);
}

/*
 * millis:
 *	Return a number of milliseconds as an unsigned int.
 *	Wraps at 49 days.
 */
unsigned int millis (void)
{
  uint64_t now ;
  struct  timespec ts ;
  clock_gettime (CLOCK_MONOTONIC_RAW, &ts) ;
  now  = (uint64_t)ts.tv_sec * (uint64_t)1000 + (uint64_t)(ts.tv_nsec / 1000000L) ;
  return (uint32_t)(now - epochMilli) ;
}
