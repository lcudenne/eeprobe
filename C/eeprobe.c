
    /* EEProbe: Energy Efficient Probe for MPI */
    /* Copyright (C) 2020 Loïc Cudennec */

    /* This program is free software: you can redistribute it and/or modify */
    /* it under the terms of the GNU General Public License as published by */
    /* the Free Software Foundation, either version 3 of the License, or */
    /* any later version. */

    /* This program is distributed in the hope that it will be useful, */
    /* but WITHOUT ANY WARRANTY; without even the implied warranty of */
    /* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
    /* GNU General Public License for more details. */

    /* You should have received a copy of the GNU General Public License */
    /* along with this program.  If not, see <https://www.gnu.org/licenses/>. */


/* ---------------------------------------------------------------------------------- */

/* gettimeofday */
#include <sys/time.h>

/* NULL */
#include <stddef.h>

/* clock_nanosleep */
#include <time.h>

/* gettimeofday */
#include <sys/time.h>

/* MPI */
#include "mpi.h"



/* ---------------------------------------------------------------------------------- */


#include "eeprobe.h"


/* ---------------------------------------------------------------------------------- */

static int _EEPROBE_LAST_YIELD_TIME = 0;

static int _EEPROBE_MAX_YIELD_TIME = 1000;

static int _EEPROBE_MIN_YIELD_TIME = 0;

static unsigned int _EEPROBE_INC_YIELD_TIME = 1;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME = 0;

/* ---------------------------------------------------------------------------------- */

void
EEPROBE_setMinYieldTime(int min_yield_time) {
  _EEPROBE_MIN_YIELD_TIME = min_yield_time;
}

void
EEPROBE_setMaxYieldTime(int max_yield_time) {
  _EEPROBE_MAX_YIELD_TIME = max_yield_time;
}

void
EEPROBE_setIncYieldTime(int inc_yield_time) {
  _EEPROBE_INC_YIELD_TIME = inc_yield_time;
}

int
EEPROBE_getMinYieldTime() {
  return _EEPROBE_MIN_YIELD_TIME;
}

int
EEPROBE_getMaxYieldTime() {
  return _EEPROBE_MAX_YIELD_TIME;
}

int
EEPROBE_getIncYieldTime() {
  return _EEPROBE_INC_YIELD_TIME;
}

int
EEPROBE_getLastYieldTime() {
  return _EEPROBE_LAST_YIELD_TIME;
}

unsigned long
EEPROBE_getTotalSleepTime() {
  return _EEPROBE_TOTAL_SLEEP_TIME;
}

/* ---------------------------------------------------------------------------------- */

unsigned long
EEPROBE_getTime() {

  struct timeval tv;

  gettimeofday(&tv, NULL);

  return ((unsigned long) 1000000 * tv.tv_sec + tv.tv_usec);
  
}

/* ---------------------------------------------------------------------------------- */


void
EEPROBE_Probe(int srce, int tag, MPI_Comm comm, MPI_Status * status, EEPROBE_Enable enable) {


#if EEPROBE_ENABLE_TOTAL_SLEEP_TIME
  unsigned long start = 0;
#endif
  
  int flag = 0;

  struct timespec current_yield_duration;

  if (enable == EEPROBE_ENABLE) {
    
    current_yield_duration.tv_sec = 0;
    current_yield_duration.tv_nsec = _EEPROBE_MIN_YIELD_TIME;

    while (flag == 0) {

      MPI_Iprobe(srce, tag, comm, &flag, status);

      if (flag == 0) {

#if EEPROBE_ENABLE_TOTAL_SLEEP_TIME
	start = EEPROBE_getTime();
#endif
      
	clock_nanosleep(CLOCK_MONOTONIC, 0, &current_yield_duration, NULL);

#if EEPROBE_ENABLE_TOTAL_SLEEP_TIME
	_EEPROBE_TOTAL_SLEEP_TIME += EEPROBE_getTime() - start;      
#endif
      
	if (current_yield_duration.tv_nsec < _EEPROBE_MAX_YIELD_TIME) {
	  current_yield_duration.tv_nsec += _EEPROBE_INC_YIELD_TIME;
	}

      } else {
      
	_EEPROBE_LAST_YIELD_TIME = current_yield_duration.tv_nsec;
	current_yield_duration.tv_nsec = _EEPROBE_MIN_YIELD_TIME;
      
      }

    }

  } else {

    MPI_Probe(srce, tag, comm, status);

  }

}


/* ---------------------------------------------------------------------------------- */




/* ---------------------------------------------------------------------------------- */
