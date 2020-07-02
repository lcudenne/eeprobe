
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

/* assert */
#include <assert.h>

/* gettimeofday */
#include <sys/time.h>

/* NULL */
#include <stddef.h>

/* clock_nanosleep */
#include <time.h>

/* gettimeofday */
#include <sys/time.h>

/* ---------------------------------------------------------------------------------- */


#include "eeprobe.h"


/* ---------------------------------------------------------------------------------- */

  /**
   * Enum type used to identify the MPI action.
   */
typedef enum {EEPROBE_PROBE, EEPROBE_WAIT, EEPROBE_REDUCE} EEPROBE_ACTION;


/* ---------------------------------------------------------------------------------- */

static long _EEPROBE_LAST_YIELD_TIME = 0;

static long _EEPROBE_MAX_YIELD_TIME = 1000;

static long _EEPROBE_MIN_YIELD_TIME = 0;

static long _EEPROBE_INC_YIELD_TIME = 1;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_PROBE = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_WAIT = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_REDUCE = 0;

/* ---------------------------------------------------------------------------------- */

void
EEPROBE_setMinYieldTime(long min_yield_time) {
  assert(min_yield_time >= 0);
  assert(min_yield_time < 1000000000);
  _EEPROBE_MIN_YIELD_TIME = min_yield_time;
}

void
EEPROBE_setMaxYieldTime(long max_yield_time) {
  assert(max_yield_time > 0);
  assert(max_yield_time < 1000000000);
  _EEPROBE_MAX_YIELD_TIME = max_yield_time;
}

void
EEPROBE_setIncYieldTime(long inc_yield_time) {
  assert(inc_yield_time > 0);
  assert(inc_yield_time < 1000000000);
  _EEPROBE_INC_YIELD_TIME = inc_yield_time;
}

long
EEPROBE_getMinYieldTime() {
  return _EEPROBE_MIN_YIELD_TIME;
}

long
EEPROBE_getMaxYieldTime() {
  return _EEPROBE_MAX_YIELD_TIME;
}

long
EEPROBE_getIncYieldTime() {
  return _EEPROBE_INC_YIELD_TIME;
}

long
EEPROBE_getLastYieldTime() {
  return _EEPROBE_LAST_YIELD_TIME;
}

unsigned long
EEPROBE_getTotalSleepTime() {
  return _EEPROBE_TOTAL_SLEEP_TIME_PROBE + _EEPROBE_TOTAL_SLEEP_TIME_WAIT + _EEPROBE_TOTAL_SLEEP_TIME_REDUCE;
}

unsigned long
EEPROBE_getTotalSleepTimeProbe() {
  return _EEPROBE_TOTAL_SLEEP_TIME_PROBE;
}

unsigned long
EEPROBE_getTotalSleepTimeWait() {
  return _EEPROBE_TOTAL_SLEEP_TIME_WAIT;
}

unsigned long
EEPROBE_getTotalSleepTimeReduce() {
  return _EEPROBE_TOTAL_SLEEP_TIME_REDUCE;
}

/* ---------------------------------------------------------------------------------- */

unsigned long
EEPROBE_getTime() {

  struct timeval tv;

  gettimeofday(&tv, NULL);

  return ((unsigned long) 1000000 * tv.tv_sec + tv.tv_usec);
  
}

/* ---------------------------------------------------------------------------------- */

static void
EEPROBE_updateTotalSleepTime(EEPROBE_ACTION action, unsigned long time) {

  switch(action) {
  case EEPROBE_PROBE:
    _EEPROBE_TOTAL_SLEEP_TIME_PROBE += time;
    break;
  case EEPROBE_WAIT:
    _EEPROBE_TOTAL_SLEEP_TIME_WAIT += time;
    break;
  case EEPROBE_REDUCE:
    _EEPROBE_TOTAL_SLEEP_TIME_REDUCE += time;
    break;
  default:
    break;
  }
  
}

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Probe(int source, int tag, MPI_Comm comm, MPI_Status * status) {
  return EEPROBE_Probe_Switch(source, tag, comm, status, EEPROBE_ENABLE);
}

int
EEPROBE_Probe_Switch(int source, int tag, MPI_Comm comm, MPI_Status * status,
		     EEPROBE_Enable enable) {


#if EEPROBE_ENABLE_TOTAL_SLEEP_TIME
  unsigned long start = 0;
#endif
  
  int flag = 0;

  int errno = MPI_SUCCESS;
    
  struct timespec current_yield_duration;

  if (enable == EEPROBE_ENABLE) {
    
    current_yield_duration.tv_sec = 0;
    current_yield_duration.tv_nsec = _EEPROBE_MIN_YIELD_TIME;

    while ((flag == 0) && (errno == MPI_SUCCESS)) {

      errno = MPI_Iprobe(source, tag, comm, &flag, status);

      if (flag == 0) {

#if EEPROBE_ENABLE_TOTAL_SLEEP_TIME
	start = EEPROBE_getTime();
#endif
      
	clock_nanosleep(CLOCK_MONOTONIC, 0, &current_yield_duration, NULL);

#if EEPROBE_ENABLE_TOTAL_SLEEP_TIME
	EEPROBE_updateTotalSleepTime(EEPROBE_PROBE, EEPROBE_getTime() - start);
#endif

	current_yield_duration.tv_nsec += _EEPROBE_INC_YIELD_TIME;
	if (current_yield_duration.tv_nsec > _EEPROBE_MAX_YIELD_TIME) {
	  current_yield_duration.tv_nsec = _EEPROBE_MAX_YIELD_TIME;
	}

      }

    }

    _EEPROBE_LAST_YIELD_TIME = current_yield_duration.tv_nsec;

  } else {

    errno = MPI_Probe(source, tag, comm, status);

  }

  return errno;
  
}

/* ---------------------------------------------------------------------------------- */


static int
EEPROBE_Wait_Core(MPI_Request *request, MPI_Status *status,
		  EEPROBE_Enable enable, EEPROBE_ACTION action) {


#if EEPROBE_ENABLE_TOTAL_SLEEP_TIME
  unsigned long start = 0;
#endif
  
  int flag = 0;

  int errno = MPI_SUCCESS;
    
  struct timespec current_yield_duration;

  if (enable == EEPROBE_ENABLE) {
    
    current_yield_duration.tv_sec = 0;
    current_yield_duration.tv_nsec = _EEPROBE_MIN_YIELD_TIME;

    while ((flag == 0) && (errno == MPI_SUCCESS)) {

      errno = MPI_Test(request, &flag, status);

      if (flag == 0) {

#if EEPROBE_ENABLE_TOTAL_SLEEP_TIME
	start = EEPROBE_getTime();
#endif
      
	clock_nanosleep(CLOCK_MONOTONIC, 0, &current_yield_duration, NULL);

#if EEPROBE_ENABLE_TOTAL_SLEEP_TIME
	EEPROBE_updateTotalSleepTime(action, EEPROBE_getTime() - start);
#endif

	current_yield_duration.tv_nsec += _EEPROBE_INC_YIELD_TIME;
	if (current_yield_duration.tv_nsec > _EEPROBE_MAX_YIELD_TIME) {
	  current_yield_duration.tv_nsec = _EEPROBE_MAX_YIELD_TIME;
	}

      }

    }

    _EEPROBE_LAST_YIELD_TIME = current_yield_duration.tv_nsec;

  } else {

    errno = MPI_Wait(request, status);

  }

  return errno;
  
}

int
EEPROBE_Wait(MPI_Request *request, MPI_Status *status) {
  return EEPROBE_Wait_Core(request, status, EEPROBE_ENABLE, EEPROBE_WAIT);
}


int
EEPROBE_Wait_Switch(MPI_Request *request, MPI_Status *status, EEPROBE_Enable enable) {
  return EEPROBE_Wait_Core(request, status, enable, EEPROBE_WAIT);
}

/* ---------------------------------------------------------------------------------- */


int
EEPROBE_Reduce(const void *sendbuf, void *recvbuf, int count,
		      MPI_Datatype datatype, MPI_Op op, int root,
		      MPI_Comm comm) {
  return EEPROBE_Reduce_Switch(sendbuf, recvbuf, count, datatype, op, root, comm, EEPROBE_ENABLE);
}

int
EEPROBE_Reduce_Switch(const void *sendbuf, void *recvbuf, int count,
		      MPI_Datatype datatype, MPI_Op op, int root,
		      MPI_Comm comm, EEPROBE_Enable enable) {

  MPI_Request request;

  MPI_Status status;
  
  int errno = MPI_SUCCESS;

  if (enable == EEPROBE_ENABLE) {

    errno = MPI_Ireduce(sendbuf, recvbuf, count, datatype, op, root, comm, &request);

    errno = EEPROBE_Wait_Core(&request, &status, enable, EEPROBE_REDUCE);

  } else {

    errno = MPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm);

  }

  return errno;

}

/* ---------------------------------------------------------------------------------- */
