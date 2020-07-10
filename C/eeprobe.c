
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
typedef enum {
	      EEPROBE_PROBE,
	      EEPROBE_WAIT,
	      EEPROBE_RECV,
	      EEPROBE_REDUCE,
	      EEPROBE_ALLREDUCE,
	      EEPROBE_ALLTOALL,
	      EEPROBE_ALLTOALLV,
	      EEPROBE_ALLTOALLW,
	      EEPROBE_BCAST,
	      EEPROBE_SCATTER,
	      EEPROBE_SCATTERV,
	      EEPROBE_GATHER,
	      EEPROBE_GATHERV,
	      EEPROBE_ALLGATHER,
	      EEPROBE_ALLGATHERV,
	      EEPROBE_BARRIER
} EEPROBE_ACTION;


/* ---------------------------------------------------------------------------------- */

static long _EEPROBE_LAST_YIELD_TIME = 0;

static long _EEPROBE_MAX_YIELD_TIME = 1000;

static long _EEPROBE_MIN_YIELD_TIME = 0;

static long _EEPROBE_INC_YIELD_TIME = 1;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_PROBE = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_WAIT = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_RECV = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_REDUCE = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_ALLREDUCE = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_ALLTOALL = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_ALLTOALLV = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_ALLTOALLW = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_BCAST = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_SCATTER = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_SCATTERV = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_GATHER = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_GATHERV = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_ALLGATHER = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_ALLGATHERV = 0;

static unsigned long _EEPROBE_TOTAL_SLEEP_TIME_BARRIER = 0;


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
  return _EEPROBE_TOTAL_SLEEP_TIME_PROBE +
    _EEPROBE_TOTAL_SLEEP_TIME_WAIT +
    _EEPROBE_TOTAL_SLEEP_TIME_RECV +
    _EEPROBE_TOTAL_SLEEP_TIME_REDUCE +
    _EEPROBE_TOTAL_SLEEP_TIME_ALLREDUCE +
    _EEPROBE_TOTAL_SLEEP_TIME_ALLTOALL +
    _EEPROBE_TOTAL_SLEEP_TIME_ALLTOALLV +
    _EEPROBE_TOTAL_SLEEP_TIME_ALLTOALLW +
    _EEPROBE_TOTAL_SLEEP_TIME_BCAST +
    _EEPROBE_TOTAL_SLEEP_TIME_SCATTER +
    _EEPROBE_TOTAL_SLEEP_TIME_SCATTERV +
    _EEPROBE_TOTAL_SLEEP_TIME_GATHER +
    _EEPROBE_TOTAL_SLEEP_TIME_GATHERV +
    _EEPROBE_TOTAL_SLEEP_TIME_ALLGATHER +
    _EEPROBE_TOTAL_SLEEP_TIME_ALLGATHERV +
    _EEPROBE_TOTAL_SLEEP_TIME_BARRIER;
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
EEPROBE_getTotalSleepTimeRecv() {
  return _EEPROBE_TOTAL_SLEEP_TIME_RECV;
}

unsigned long
EEPROBE_getTotalSleepTimeReduce() {
  return _EEPROBE_TOTAL_SLEEP_TIME_REDUCE;
}

unsigned long
EEPROBE_getTotalSleepTimeAllreduce() {
  return _EEPROBE_TOTAL_SLEEP_TIME_ALLREDUCE;
}

unsigned long
EEPROBE_getTotalSleepTimeAlltoall() {
  return _EEPROBE_TOTAL_SLEEP_TIME_ALLTOALL;
}

unsigned long
EEPROBE_getTotalSleepTimeAlltoallv() {
  return _EEPROBE_TOTAL_SLEEP_TIME_ALLTOALLV;
}

unsigned long
EEPROBE_getTotalSleepTimeAlltoallw() {
  return _EEPROBE_TOTAL_SLEEP_TIME_ALLTOALLW;
}

unsigned long
EEPROBE_getTotalSleepTimeBcast() {
  return _EEPROBE_TOTAL_SLEEP_TIME_BCAST;
}

unsigned long
EEPROBE_getTotalSleepTimeScatter() {
  return _EEPROBE_TOTAL_SLEEP_TIME_SCATTER;
}

unsigned long
EEPROBE_getTotalSleepTimeScatterv() {
  return _EEPROBE_TOTAL_SLEEP_TIME_SCATTERV;
}

unsigned long
EEPROBE_getTotalSleepTimeGather() {
  return _EEPROBE_TOTAL_SLEEP_TIME_GATHER;
}

unsigned long
EEPROBE_getTotalSleepTimeGatherv() {
  return _EEPROBE_TOTAL_SLEEP_TIME_GATHERV;
}

unsigned long
EEPROBE_getTotalSleepTimeAllgather() {
  return _EEPROBE_TOTAL_SLEEP_TIME_ALLGATHER;
}

unsigned long
EEPROBE_getTotalSleepTimeAllgatherv() {
  return _EEPROBE_TOTAL_SLEEP_TIME_ALLGATHERV;
}

unsigned long
EEPROBE_getTotalSleepTimeBarrier() {
  return _EEPROBE_TOTAL_SLEEP_TIME_BARRIER;
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
  case EEPROBE_RECV:
    _EEPROBE_TOTAL_SLEEP_TIME_RECV += time;
    break;
  case EEPROBE_REDUCE:
    _EEPROBE_TOTAL_SLEEP_TIME_REDUCE += time;
    break;
  case EEPROBE_ALLREDUCE:
    _EEPROBE_TOTAL_SLEEP_TIME_ALLREDUCE += time;
    break;
  case EEPROBE_ALLTOALL:
    _EEPROBE_TOTAL_SLEEP_TIME_ALLTOALL += time;
    break;
  case EEPROBE_ALLTOALLV:
    _EEPROBE_TOTAL_SLEEP_TIME_ALLTOALLV += time;
    break;
  case EEPROBE_ALLTOALLW:
    _EEPROBE_TOTAL_SLEEP_TIME_ALLTOALLW += time;
    break;
  case EEPROBE_BCAST:
    _EEPROBE_TOTAL_SLEEP_TIME_BCAST += time;
    break;
  case EEPROBE_SCATTER:
    _EEPROBE_TOTAL_SLEEP_TIME_SCATTER += time;
    break;
  case EEPROBE_SCATTERV:
    _EEPROBE_TOTAL_SLEEP_TIME_SCATTERV += time;
    break;
  case EEPROBE_GATHER:
    _EEPROBE_TOTAL_SLEEP_TIME_GATHER += time;
    break;
  case EEPROBE_GATHERV:
    _EEPROBE_TOTAL_SLEEP_TIME_GATHERV += time;
    break;
  case EEPROBE_ALLGATHER:
    _EEPROBE_TOTAL_SLEEP_TIME_ALLGATHER += time;
    break;
  case EEPROBE_ALLGATHERV:
    _EEPROBE_TOTAL_SLEEP_TIME_ALLGATHERV += time;
    break;
  case EEPROBE_BARRIER:
    _EEPROBE_TOTAL_SLEEP_TIME_BARRIER += time;
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
EEPROBE_Recv(void *buf, int count, MPI_Datatype datatype,
	     int source, int tag, MPI_Comm comm, MPI_Status *status) {
  return EEPROBE_Recv_Switch(buf, count, datatype, source, tag, comm, status, EEPROBE_ENABLE);
}

int
EEPROBE_Recv_Switch(void *buf, int count, MPI_Datatype datatype,
		    int source, int tag, MPI_Comm comm, MPI_Status *status,
		    EEPROBE_Enable enable) {

  MPI_Request request;

  int errno = MPI_SUCCESS;

  if (enable == EEPROBE_ENABLE) {

    errno = MPI_Irecv(buf, count, datatype, source, tag, comm, &request);

    errno = EEPROBE_Wait_Core(&request, status, enable, EEPROBE_RECV);

  } else {

    errno = MPI_Recv(buf, count, datatype, source, tag, comm, status);

  }

  return errno;

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


int
EEPROBE_Allreduce(const void *sendbuf, void *recvbuf, int count,
                  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {

  return EEPROBE_Allreduce_Switch(sendbuf, recvbuf, count, datatype, op, comm, EEPROBE_ENABLE);

}


int
EEPROBE_Allreduce_Switch(const void *sendbuf, void *recvbuf, int count,
			 MPI_Datatype datatype, MPI_Op op, MPI_Comm comm,
			 EEPROBE_Enable enable) {

  MPI_Request request;

  MPI_Status status;
  
  int errno = MPI_SUCCESS;

  if (enable == EEPROBE_ENABLE) {

    errno = MPI_Iallreduce(sendbuf, recvbuf, count, datatype, op, comm, &request);

    errno = EEPROBE_Wait_Core(&request, &status, enable, EEPROBE_ALLREDUCE);

  } else {

    errno = MPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);

  }

  return errno;

}

/* ---------------------------------------------------------------------------------- */


int
EEPROBE_Alltoall(const void *sendbuf, int sendcount,
		 MPI_Datatype sendtype, void *recvbuf, int recvcount,
		 MPI_Datatype recvtype, MPI_Comm comm) {

  return EEPROBE_Alltoall_Switch(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm,
				 EEPROBE_ENABLE);

}

int
EEPROBE_Alltoall_Switch(const void *sendbuf, int sendcount,
			MPI_Datatype sendtype, void *recvbuf, int recvcount,
			MPI_Datatype recvtype, MPI_Comm comm,
			EEPROBE_Enable enable) {

  MPI_Request request;

  MPI_Status status;
  
  int errno = MPI_SUCCESS;

  if (enable == EEPROBE_ENABLE) {

    errno = MPI_Ialltoall(sendbuf, sendcount, sendtype, recvbuf,
			  recvcount, recvtype, comm, &request);

    errno = EEPROBE_Wait_Core(&request, &status, enable, EEPROBE_ALLTOALL);

  } else {

    errno = MPI_Alltoall(sendbuf, sendcount, sendtype, recvbuf,
			 recvcount, recvtype, comm);

  }

  return errno;

}

/* ---------------------------------------------------------------------------------- */


int
EEPROBE_Alltoallv(const void *sendbuf, const int sendcounts[],
		  const int sdispls[], MPI_Datatype sendtype,
		  void *recvbuf, const int recvcounts[],
		  const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm) {

  return EEPROBE_Alltoallv_Switch(sendbuf, sendcounts, sdispls, sendtype,
				  recvbuf, recvcounts, rdispls, recvtype, comm,
				  EEPROBE_ENABLE);

}

int
EEPROBE_Alltoallv_Switch(const void *sendbuf, const int sendcounts[],
			 const int sdispls[], MPI_Datatype sendtype,
			 void *recvbuf, const int recvcounts[],
			 const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm,
			 EEPROBE_Enable enable) {

  MPI_Request request;

  MPI_Status status;
  
  int errno = MPI_SUCCESS;

  if (enable == EEPROBE_ENABLE) {

    errno = MPI_Ialltoallv(sendbuf, sendcounts, sdispls, sendtype,
			   recvbuf, recvcounts, rdispls, recvtype, comm, &request);

    errno = EEPROBE_Wait_Core(&request, &status, enable, EEPROBE_ALLTOALLV);

  } else {

    errno = MPI_Alltoallv(sendbuf, sendcounts, sdispls, sendtype,
			  recvbuf, recvcounts, rdispls, recvtype, comm);

  }

  return errno;

}

/* ---------------------------------------------------------------------------------- */


int
EEPROBE_Alltoallw(const void *sendbuf, const int sendcounts[],
		  const int sdispls[], const MPI_Datatype sendtypes[],
		  void *recvbuf, const int recvcounts[], const int rdispls[],
		  const MPI_Datatype recvtypes[], MPI_Comm comm) {

  return EEPROBE_Alltoallw_Switch(sendbuf, sendcounts, sdispls, sendtypes,
				  recvbuf, recvcounts, rdispls, recvtypes, comm,
				  EEPROBE_ENABLE);

}

int
EEPROBE_Alltoallw_Switch(const void *sendbuf, const int sendcounts[],
			 const int sdispls[], const MPI_Datatype sendtypes[],
			 void *recvbuf, const int recvcounts[], const int rdispls[],
			 const MPI_Datatype recvtypes[], MPI_Comm comm,
			 EEPROBE_Enable enable) {

  MPI_Request request;

  MPI_Status status;
  
  int errno = MPI_SUCCESS;

  if (enable == EEPROBE_ENABLE) {

    errno = MPI_Ialltoallw(sendbuf, sendcounts, sdispls, sendtypes,
			   recvbuf, recvcounts, rdispls, recvtypes, comm, &request);

    errno = EEPROBE_Wait_Core(&request, &status, enable, EEPROBE_ALLTOALLV);

  } else {

    errno = MPI_Alltoallw(sendbuf, sendcounts, sdispls, sendtypes,
			  recvbuf, recvcounts, rdispls, recvtypes, comm);

  }

  return errno;

}

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Bcast(void *buffer, int count, MPI_Datatype datatype,
	      int root, MPI_Comm comm) {

  return EEPROBE_Bcast_Switch(buffer, count, datatype, root, comm, EEPROBE_ENABLE);
  
}

int
EEPROBE_Bcast_Switch(void *buffer, int count, MPI_Datatype datatype,
		     int root, MPI_Comm comm, EEPROBE_Enable enable) {

  MPI_Request request;

  MPI_Status status;
  
  int errno = MPI_SUCCESS;

  if (enable == EEPROBE_ENABLE) {

    errno = MPI_Ibcast(buffer, count, datatype, root, comm, &request);

    errno = EEPROBE_Wait_Core(&request, &status, enable, EEPROBE_BCAST);

  } else {

    errno = MPI_Bcast(buffer, count, datatype, root, comm);

  }

  return errno;
  
}

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
		void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
		MPI_Comm comm) {

  return EEPROBE_Scatter_Switch(sendbuf, sendcount, sendtype,
				recvbuf, recvcount, recvtype, root, comm,
				EEPROBE_ENABLE);
  
}

int
EEPROBE_Scatter_Switch(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
		       void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
		       MPI_Comm comm, EEPROBE_Enable enable) {

  MPI_Request request;

  MPI_Status status;
  
  int errno = MPI_SUCCESS;

  if (enable == EEPROBE_ENABLE) {

    errno = MPI_Iscatter(sendbuf, sendcount, sendtype,
			 recvbuf, recvcount, recvtype, root, comm, &request);

    errno = EEPROBE_Wait_Core(&request, &status, enable, EEPROBE_SCATTER);

  } else {

    errno = MPI_Scatter(sendbuf, sendcount, sendtype,
			recvbuf, recvcount, recvtype, root, comm);

  }

  return errno;
  
}

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Scatterv(const void *sendbuf, const int sendcounts[], const int displs[],
		 MPI_Datatype sendtype, void *recvbuf, int recvcount,
		 MPI_Datatype recvtype, int root, MPI_Comm comm) {

  return EEPROBE_Scatterv_Switch(sendbuf, sendcounts, displs, sendtype,
				 recvbuf, recvcount, recvtype, root, comm,
				 EEPROBE_ENABLE);
  
}

int
EEPROBE_Scatterv_Switch(const void *sendbuf, const int sendcounts[], const int displs[],
			MPI_Datatype sendtype, void *recvbuf, int recvcount,
			MPI_Datatype recvtype, int root, MPI_Comm comm,
			EEPROBE_Enable enable) {

  MPI_Request request;

  MPI_Status status;
  
  int errno = MPI_SUCCESS;

  if (enable == EEPROBE_ENABLE) {

    errno = MPI_Iscatterv(sendbuf, sendcounts, displs, sendtype,
			  recvbuf, recvcount, recvtype, root, comm, &request);

    errno = EEPROBE_Wait_Core(&request, &status, enable, EEPROBE_SCATTERV);

  } else {

    errno = MPI_Scatterv(sendbuf, sendcounts, displs, sendtype,
			 recvbuf, recvcount, recvtype, root, comm);

  }

  return errno;
  
}

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
	       void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
	       MPI_Comm comm) {

  return EEPROBE_Gather_Switch(sendbuf, sendcount, sendtype,
			       recvbuf, recvcount, recvtype, root, comm,
			       EEPROBE_ENABLE);
  
}

int
EEPROBE_Gather_Switch(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
		      void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
		      MPI_Comm comm, EEPROBE_Enable enable) {

  MPI_Request request;

  MPI_Status status;
  
  int errno = MPI_SUCCESS;

  if (enable == EEPROBE_ENABLE) {

    errno = MPI_Igather(sendbuf, sendcount, sendtype,
			recvbuf, recvcount, recvtype, root, comm, &request);

    errno = EEPROBE_Wait_Core(&request, &status, enable, EEPROBE_GATHER);

  } else {

    errno = MPI_Gather(sendbuf, sendcount, sendtype,
		       recvbuf, recvcount, recvtype, root, comm);

  }

  return errno;
  
}

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
		void *recvbuf, const int recvcounts[], const int displs[],
		MPI_Datatype recvtype, int root, MPI_Comm comm) {

  return EEPROBE_Gatherv_Switch(sendbuf, sendcount, sendtype,
				recvbuf, recvcounts, displs, recvtype, root, comm,
				EEPROBE_ENABLE);
  
}

int
EEPROBE_Gatherv_Switch(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
		       void *recvbuf, const int recvcounts[], const int displs[],
		       MPI_Datatype recvtype, int root, MPI_Comm comm,
		       EEPROBE_Enable enable) {

  MPI_Request request;

  MPI_Status status;
  
  int errno = MPI_SUCCESS;

  if (enable == EEPROBE_ENABLE) {

    errno = MPI_Igatherv(sendbuf, sendcount, sendtype,
			 recvbuf, recvcounts, displs, recvtype, root, comm, &request);

    errno = EEPROBE_Wait_Core(&request, &status, enable, EEPROBE_GATHERV);

  } else {

    errno = MPI_Gatherv(sendbuf, sendcount, sendtype,
			recvbuf, recvcounts, displs, recvtype, root, comm);

  }

  return errno;
  
}

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Allgather(const void *sendbuf, int  sendcount,
		  MPI_Datatype sendtype, void *recvbuf, int recvcount,
		  MPI_Datatype recvtype, MPI_Comm comm) {

  return EEPROBE_Allgather_Switch(sendbuf, sendcount, sendtype,
				  recvbuf, recvcount, recvtype, comm,
				  EEPROBE_ENABLE);
  
}

int
EEPROBE_Allgather_Switch(const void *sendbuf, int  sendcount,
			 MPI_Datatype sendtype, void *recvbuf, int recvcount,
			 MPI_Datatype recvtype, MPI_Comm comm, EEPROBE_Enable enable) {

  MPI_Request request;

  MPI_Status status;
  
  int errno = MPI_SUCCESS;

  if (enable == EEPROBE_ENABLE) {

    errno = MPI_Iallgather(sendbuf, sendcount, sendtype,
			   recvbuf, recvcount, recvtype, comm, &request);

    errno = EEPROBE_Wait_Core(&request, &status, enable, EEPROBE_ALLGATHER);

  } else {

    errno = MPI_Allgather(sendbuf, sendcount, sendtype,
			  recvbuf, recvcount, recvtype, comm);

  }

  return errno;
  
}

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Allgatherv(const void *sendbuf, int sendcount,
		   MPI_Datatype sendtype, void *recvbuf, const int recvcounts[],
		   const int displs[], MPI_Datatype recvtype, MPI_Comm comm) {

  return EEPROBE_Allgatherv_Switch(sendbuf, sendcount, sendtype,
				   recvbuf, recvcounts, displs, recvtype, comm,
				   EEPROBE_ENABLE);
  
}

int
EEPROBE_Allgatherv_Switch(const void *sendbuf, int sendcount,
			  MPI_Datatype sendtype, void *recvbuf, const int recvcounts[],
			  const int displs[], MPI_Datatype recvtype, MPI_Comm comm,
			  EEPROBE_Enable enable) {

  MPI_Request request;

  MPI_Status status;
  
  int errno = MPI_SUCCESS;

  if (enable == EEPROBE_ENABLE) {

    errno = MPI_Iallgatherv(sendbuf, sendcount, sendtype,
			    recvbuf, recvcounts, displs, recvtype, comm, &request);

    errno = EEPROBE_Wait_Core(&request, &status, enable, EEPROBE_ALLGATHERV);

  } else {

    errno = MPI_Allgatherv(sendbuf, sendcount, sendtype,
			   recvbuf, recvcounts, displs, recvtype, comm);

  }

  return errno;
  
}

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Barrier(MPI_Comm comm) {

  return EEPROBE_Barrier_Switch(comm, EEPROBE_ENABLE);
  
}

int
EEPROBE_Barrier_Switch(MPI_Comm comm, EEPROBE_Enable enable) {

  MPI_Request request;

  MPI_Status status;
  
  int errno = MPI_SUCCESS;

  if (enable == EEPROBE_ENABLE) {

    errno = MPI_Ibarrier(comm, &request);

    errno = EEPROBE_Wait_Core(&request, &status, enable, EEPROBE_BARRIER);

  } else {

    errno = MPI_Barrier(comm);

  }

  return errno;
  
}


/* ---------------------------------------------------------------------------------- */
