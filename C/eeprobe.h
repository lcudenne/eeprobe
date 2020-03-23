
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
//extern "C" {
#endif

/* ---------------------------------------------------------------------------------- */

typedef enum {EEPROBE_ENABLE, EEPROBE_DISABLE} EEPROBE_Enable;

/* ---------------------------------------------------------------------------------- */

#define EEPROBE_ENABLE_TOTAL_SLEEP_TIME 1

/* ---------------------------------------------------------------------------------- */

void
EEPROBE_Probe(int srce, int tag, MPI_Comm comm, MPI_Status * status, EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

void EEPROBE_setMinYieldTime(int min_yield_time);

void EEPROBE_setMaxYieldTime(int max_yield_time);

void EEPROBE_setIncYieldTime(int inc_yield_time);

int EEPROBE_getMinYieldTime();

int EEPROBE_getMaxYieldTime();

int EEPROBE_getIncYieldTime();

int EEPROBE_getLastYieldTime();

unsigned long EEPROBE_getTotalSleepTime();

/* ---------------------------------------------------------------------------------- */
  
unsigned long EEPROBE_getTime();
  
/* ---------------------------------------------------------------------------------- */
  
#ifdef __cplusplus
}
#endif


#endif
