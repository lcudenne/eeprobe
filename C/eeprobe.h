
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

/* MPI */
#include "mpi.h"

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
   * Calculate the total amount of sleep duration since the beginning if set to 1.
   * Use EEPROBE_getTotalSleepTime() to read this value.
   * Set to 0 to disable.
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
   * @param enable Enable or disable the micro-sleep mechanism (Switch only).
   * @return MPI routine error value.
   */
int
EEPROBE_Probe(int source, int tag, MPI_Comm comm, MPI_Status * status);
int
EEPROBE_Probe_Switch(int source, int tag, MPI_Comm comm, MPI_Status * status,
		     EEPROBE_Enable enable);


/* ---------------------------------------------------------------------------------- */

  /**
   * EEPROBE_Wait takes the same parameters
   * as the default MPI Wait function and a specific parameter to enable or disable
   * the micro-sleeping mechanism. This function is synchronous and waits for an MPI
   * send or receive to complete.
   *
   * @param request The request handle.
   * @param status Status object (status).
   * @param enable Enable or disable the micro-sleep mechanism (Switch only).
   * @return MPI routine error value.
   */
int
EEPROBE_Wait(MPI_Request *request, MPI_Status *status);
int
EEPROBE_Wait_Switch(MPI_Request *request, MPI_Status *status, EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

  /**
   * EEPROBE_Recv takes the same parameters
   * as the default MPI Recv function and a specific parameter to enable or disable
   * the micro-sleeping mechanism. This function is synchronous and performs a
   * standard-mode blocking receive.
   *
   * @param buf Initial address of receive buffer.
   * @param count Maximum number of elements to receive.
   * @param datatype Datatype of each receive buffer entry.
   * @param source Rank of source.
   * @param tag Message tag.
   * @param comm Communicator (handle).
   * @param status Status object.
   * @param enable Enable or disable the micro-sleep mechanism (Switch only).
   * @return MPI routine error value.
   */
int
EEPROBE_Recv(void *buf, int count, MPI_Datatype datatype,
	     int source, int tag, MPI_Comm comm, MPI_Status *status);
int
EEPROBE_Recv_Switch(void *buf, int count, MPI_Datatype datatype,
		    int source, int tag, MPI_Comm comm, MPI_Status *status,
		    EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */
  
int
EEPROBE_Reduce(const void *sendbuf, void *recvbuf, int count,
	       MPI_Datatype datatype, MPI_Op op, int root,
	       MPI_Comm comm);
int
EEPROBE_Reduce_Switch(const void *sendbuf, void *recvbuf, int count,
		      MPI_Datatype datatype, MPI_Op op, int root,
		      MPI_Comm comm, EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Allreduce(const void *sendbuf, void *recvbuf, int count,
		  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int
EEPROBE_Allreduce_Switch(const void *sendbuf, void *recvbuf, int count,
			 MPI_Datatype datatype, MPI_Op op, MPI_Comm comm,
			 EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Alltoall(const void *sendbuf, int sendcount,
		 MPI_Datatype sendtype, void *recvbuf, int recvcount,
		 MPI_Datatype recvtype, MPI_Comm comm);
int
EEPROBE_Alltoall_Switch(const void *sendbuf, int sendcount,
			MPI_Datatype sendtype, void *recvbuf, int recvcount,
			MPI_Datatype recvtype, MPI_Comm comm,
			EEPROBE_Enable enable);
  
/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Alltoallv(const void *sendbuf, const int sendcounts[],
		  const int sdispls[], MPI_Datatype sendtype,
		  void *recvbuf, const int recvcounts[],
		  const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm);
int
EEPROBE_Alltoallv_Switch(const void *sendbuf, const int sendcounts[],
			 const int sdispls[], MPI_Datatype sendtype,
			 void *recvbuf, const int recvcounts[],
			 const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm,
			 EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Alltoallw(const void *sendbuf, const int sendcounts[],
		  const int sdispls[], const MPI_Datatype sendtypes[],
		  void *recvbuf, const int recvcounts[], const int rdispls[],
		  const MPI_Datatype recvtypes[], MPI_Comm comm);
int
EEPROBE_Alltoallw_Switch(const void *sendbuf, const int sendcounts[],
			 const int sdispls[], const MPI_Datatype sendtypes[],
			 void *recvbuf, const int recvcounts[], const int rdispls[],
			 const MPI_Datatype recvtypes[], MPI_Comm comm,
			 EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Bcast(void *buffer, int count, MPI_Datatype datatype,
	      int root, MPI_Comm comm);
int
EEPROBE_Bcast_Switch(void *buffer, int count, MPI_Datatype datatype,
		     int root, MPI_Comm comm, EEPROBE_Enable enable);


/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
		void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
		MPI_Comm comm);
int
EEPROBE_Scatter_Switch(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
		       void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
		       MPI_Comm comm, EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Scatterv(const void *sendbuf, const int sendcounts[], const int displs[],
		 MPI_Datatype sendtype, void *recvbuf, int recvcount,
		 MPI_Datatype recvtype, int root, MPI_Comm comm);
int
EEPROBE_Scatterv_Switch(const void *sendbuf, const int sendcounts[], const int displs[],
			MPI_Datatype sendtype, void *recvbuf, int recvcount,
			MPI_Datatype recvtype, int root, MPI_Comm comm,
			EEPROBE_Enable enable);
  
/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
	       void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
	       MPI_Comm comm);
int
EEPROBE_Gather_Switch(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
		      void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
		      MPI_Comm comm, EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
		void *recvbuf, const int recvcounts[], const int displs[],
		MPI_Datatype recvtype, int root, MPI_Comm comm);
int
EEPROBE_Gatherv_Switch(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
		       void *recvbuf, const int recvcounts[], const int displs[],
		       MPI_Datatype recvtype, int root, MPI_Comm comm,
		       EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Allgather(const void *sendbuf, int  sendcount,
		  MPI_Datatype sendtype, void *recvbuf, int recvcount,
		  MPI_Datatype recvtype, MPI_Comm comm);
int
EEPROBE_Allgather_Switch(const void *sendbuf, int  sendcount,
			 MPI_Datatype sendtype, void *recvbuf, int recvcount,
			 MPI_Datatype recvtype, MPI_Comm comm, EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Allgatherv(const void *sendbuf, int sendcount,
		   MPI_Datatype sendtype, void *recvbuf, const int recvcounts[],
		   const int displs[], MPI_Datatype recvtype, MPI_Comm comm);
int
EEPROBE_Allgatherv_Switch(const void *sendbuf, int sendcount,
			  MPI_Datatype sendtype, void *recvbuf, const int recvcounts[],
			  const int displs[], MPI_Datatype recvtype, MPI_Comm comm,
			  EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

int
EEPROBE_Barrier(MPI_Comm comm);
int
EEPROBE_Barrier_Switch(MPI_Comm comm, EEPROBE_Enable enable);


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

/* ---------------------------------------------------------------------------------- */
  
  /**
   * Returns the total sleep duration since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTime();

  /**
   * Returns the total sleep duration using EEPROBE_Probe since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTimeProbe();

  /**
   * Returns the total sleep duration using EEPROBE_Wait since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTimeWait();

unsigned long EEPROBE_getTotalSleepTimeRecv();

unsigned long EEPROBE_getTotalSleepTimeReduce();

unsigned long EEPROBE_getTotalSleepTimeAllreduce();

unsigned long EEPROBE_getTotalSleepTimeAlltoall();

unsigned long EEPROBE_getTotalSleepTimeAlltoallv();

unsigned long EEPROBE_getTotalSleepTimeAlltoallw();

unsigned long EEPROBE_getTotalSleepTimeBcast();

unsigned long EEPROBE_getTotalSleepTimeScatter();

unsigned long EEPROBE_getTotalSleepTimeScatterv();

unsigned long EEPROBE_getTotalSleepTimeGather();

unsigned long EEPROBE_getTotalSleepTimeGatherv();

unsigned long EEPROBE_getTotalSleepTimeAllgather();

unsigned long EEPROBE_getTotalSleepTimeAllgatherv();

unsigned long EEPROBE_getTotalSleepTimeBarrier();

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
