
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


#ifndef EEPROBE_H
#define EEPROBE_H


#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------------------------------- */

  /**
   * Enum type used to enable or disable the micro-sleep mechanism.
   */
typedef enum {EEPROBE_ENABLE, EEPROBE_DISABLE} EEPROBE_Enable;

/* ---------------------------------------------------------------------------------- */

  /**
   * Calculate the total amount of sleep duration since the beginning
   * Use EEPROBE_getTotalSleepTime() to read this value.
   */
#define EEPROBE_ENABLE_TOTAL_SLEEP_TIME 1

/* ---------------------------------------------------------------------------------- */

  /**
   * EEPROBE_Probe takes the same parameters
   * as the default MPI Probe function and a specific parameter to enable or disable
   * the micro-sleeping mechanism. This function is synchronous and returns when a
   * matching message is being delivered by the MPI runtime.
   *
   * @param source Source rank or MPI_ANY_SOURCE (integer).
   * @param tag Tag value or MPI_ANY_TAG (integer).
   * @param comm Communicator (handle).
   * @param status Status object (status).
   * @param enable Enable or disable the micro-sleep mechanism.
   * @return MPI routine error value.
   */
int
EEPROBE_Probe(int source, int tag, MPI_Comm comm, MPI_Status * status, EEPROBE_Enable enable);


/* ---------------------------------------------------------------------------------- */

  /**
   * EEPROBE_Wait takes the same parameters
   * as the default MPI Wait function and a specific parameter to enable or disable
   * the micro-sleeping mechanism. This function is synchronous and waits for an MPI
   * send or receive to complete.
   *
   * @param request The request handle.
   * @param status Status object (status).
   * @param enable Enable or disable the micro-sleep mechanism.
   * @return MPI routine error value.
   */
int
EEPROBE_Wait(MPI_Request *request, MPI_Status *status, EEPROBE_Enable enable);


/* ---------------------------------------------------------------------------------- */

  /**
   * Set the smallest yield time.
   * @param min_yield_time In nanoseconds, must be set within range [0;1000000000[
   */
void EEPROBE_setMinYieldTime(long min_yield_time);

  /**
   * Set the maximum yield time.
   * @param max_yield_time In nanoseconds, must be set within range ]0;1000000000[
   */
void EEPROBE_setMaxYieldTime(long max_yield_time);

  /**
   * Set the incremental step time.
   * @param inc_yield_time In nanoseconds, must be set within range ]0;1000000000[
   */
void EEPROBE_setIncYieldTime(long inc_yield_time);

  /**
   * Returns the current minimum yield time.
   * @return Current minimum yield time in nanoseconds.
   */
long EEPROBE_getMinYieldTime();

  /**
   * Returns the current maximum yield time.
   * @return Current maximum yield time in nanoseconds.
   */
long EEPROBE_getMaxYieldTime();

  /**
   * Returns the current incremental step yield time.
   * @return Current incremental step yield time in nanoseconds.
   */
long EEPROBE_getIncYieldTime();

  /**
   * Returns the last yield time applied before receiving a message.
   * @return Last yield time in nanoseconds.
   */
long EEPROBE_getLastYieldTime();

  /**
   * Returns the total sleep duration since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTime();

/* ---------------------------------------------------------------------------------- */
  
  /**
   * Returns the current system time as delivered by gettimeofday()
   * @return The current system time in microseconds.
   */
unsigned long EEPROBE_getTime();
  
/* ---------------------------------------------------------------------------------- */
  
#ifdef __cplusplus
}
#endif


#endif
