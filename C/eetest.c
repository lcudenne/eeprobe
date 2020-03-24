
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

/* NULL */
#include <stddef.h>

/* malloc */
#include <stdlib.h>

/* gethostname */
#include <unistd.h>

/* fprintf */
#include <stdio.h>

/* clock_nanosleep */
#include <time.h>

/* strcmp */
#include <string.h>


/* MPI */
#include "mpi.h"

/* ---------------------------------------------------------------------------------- */

#include "eeprobe.h"

/* ---------------------------------------------------------------------------------- */

#define EEPROBE_TAG 0

#define EEPROBE_RANK_SEND 0

#define EEPROBE_RANK_RECV 1

#define EEPROBE_HOSTNAME_BUFFER_SIZE 2048

/* ---------------------------------------------------------------------------------- */

#define EEPROBE_NB_ITER 24

#define EEPROBE_INTER_MSG_SLEEP_S 1
#define EEPROBE_INTER_MSG_SLEEP_NS 0

#define EEPROBE_COUNT 1024

/* ---------------------------------------------------------------------------------- */


static int
EEPROBE_getTaskNr(MPI_Comm comm) {
  int numtasks = 0;

  MPI_Comm_size(comm, &numtasks);

  return numtasks;
}


static int
EEPROBE_getTaskId(MPI_Comm comm) {
  int taskid = 0;

  MPI_Comm_rank(comm, &taskid);

  return taskid;
}

static char *
EEPROBE_getHostName() {

  char * hostname = NULL;

  hostname = malloc(sizeof(unsigned char) * EEPROBE_HOSTNAME_BUFFER_SIZE);
  assert(hostname);

  gethostname(hostname, EEPROBE_HOSTNAME_BUFFER_SIZE);

  return hostname;
}


/* ---------------------------------------------------------------------------------- */

static void
EEPROBE_sendRecv(EEPROBE_Enable enable) {

  int rank = 0;

  unsigned int i = 0;

  char * buffer = NULL;

  int errno = MPI_SUCCESS;

  MPI_Status status;

  struct timespec sender_sleep;

  unsigned long start_time = 0;
  
  rank = EEPROBE_getTaskId(MPI_COMM_WORLD); 

  buffer = malloc(sizeof(char) * EEPROBE_COUNT);
  assert(buffer);

  start_time = EEPROBE_getTime();
  
  fprintf(stdout, "%lu rank %d start\n", EEPROBE_getTime() - start_time, rank);
  
  if (rank == EEPROBE_RANK_SEND) {
    
    sender_sleep.tv_sec = EEPROBE_INTER_MSG_SLEEP_S;
    sender_sleep.tv_nsec = EEPROBE_INTER_MSG_SLEEP_NS;
    
    for (i = 0; i < EEPROBE_NB_ITER; i++) {

      errno = MPI_Send(buffer, EEPROBE_COUNT, MPI_CHAR, EEPROBE_RANK_RECV,
		       EEPROBE_TAG, MPI_COMM_WORLD);

      assert(errno == MPI_SUCCESS);

      fprintf(stdout, "%lu rank %d send %d\n", EEPROBE_getTime() - start_time, rank, i);
      
      clock_nanosleep(CLOCK_MONOTONIC, 0, &sender_sleep, NULL);

    }

  } else {

    if (enable == EEPROBE_ENABLE) {
      fprintf(stdout, "%lu rank %d min_yield_time %ld max_yield_time %ld inc_yield_time %ld\n",
	      EEPROBE_getTime() - start_time, rank,
	      EEPROBE_getMinYieldTime(), EEPROBE_getMaxYieldTime(), EEPROBE_getIncYieldTime());
    }
      
    for (i = 0; i < EEPROBE_NB_ITER; i++) {

      EEPROBE_Probe(EEPROBE_RANK_SEND, EEPROBE_TAG, MPI_COMM_WORLD, &status, enable);

      errno = MPI_Recv(buffer, EEPROBE_COUNT, MPI_CHAR, EEPROBE_RANK_SEND,
		       EEPROBE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      assert(errno == MPI_SUCCESS);

      if (enable == EEPROBE_ENABLE) {
	fprintf(stdout, "%lu rank %d recv %d last_yield_time %ld total_sleep_time %lu\n",
		EEPROBE_getTime() - start_time, rank, i,
		EEPROBE_getLastYieldTime(), EEPROBE_getTotalSleepTime());
      } else {
	fprintf(stdout, "%lu rank %d recv %d\n", EEPROBE_getTime() - start_time, rank, i);
      }
      
    }

  }

  fprintf(stdout, "%lu rank %d end\n", EEPROBE_getTime() - start_time, rank);

  free(buffer);
  buffer = NULL;
  
}

/* ---------------------------------------------------------------------------------- */


int
main(int argc, char *argv[]) {

  char * hostname = NULL;
  
  int nr = 0;

  EEPROBE_Enable enable = EEPROBE_ENABLE;

  if ((argc > 1) && (strcmp(argv[1], "disable") == 0)) {
    enable = EEPROBE_DISABLE;
  }
  
  MPI_Init(&argc, &argv);

  hostname = EEPROBE_getHostName();

  fprintf(stdout, "rank %d on %s\n",
	  EEPROBE_getTaskId(MPI_COMM_WORLD), hostname);
  free(hostname);
  
  nr = EEPROBE_getTaskNr(MPI_COMM_WORLD);

  if (nr == 2) {

    EEPROBE_sendRecv(enable);

  } else {
    fprintf(stdout, "Warning: MPI task nr is %d. Expected 2. Usage:\nmpirun -np 2 %s\nmpirun -np 2 %s disable\n",
	    nr, argv[0], argv[0]);
  }

  
  MPI_Finalize();
  
  return 0;
}



/* ---------------------------------------------------------------------------------- */


