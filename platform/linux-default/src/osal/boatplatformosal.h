/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/*!@brief Define the abstract types of OSAL based on Linux,
 * and include the external header files.

@file
* platform_osal.h defines the abstract types for boatosal.h.
* Do not use the types in platform.h directly.
*/

#ifndef __BOATPLATFORMOSAL_H__
#define __BOATPLATFORMOSAL_H__


#include <semaphore.h>
#include <limits.h>
#include <pthread.h>

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <time.h>

/**
****************************************************************************************
* @brief:
*  This function generate random data with fixed length.
* @param[in] *output
*  output of random data
* @param[in] outputLen
*  length of random that wanted to generate
* @param[in] *rsvd
*  no used until now
* @return
*  This function returns BoAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT BoatRandom(BUINT8 *output, BUINT32 outputLen, void *rsvd);
/**
****************************************************************************************
* @brief:
*  This function malloc  size memory
* @param[in] size
* @return
*  return malloced memory, if failed return NULL.
****************************************************************************************
*/
void *BoatMalloc(size_t size);
/**
****************************************************************************************
* @brief:
*  This function free memory
* @param[in] *mem_ptr
* @return
*  This function has no return value
****************************************************************************************
*/
void BoatFree(void *mem_ptr);

/**
****************************************************************************************
* @brief:
* @param[in] second
* @return
****************************************************************************************
*/
void BoatSleep(BUINT32 second);
/**
****************************************************************************************
* @brief:
* @return
****************************************************************************************
*/
long int BoatGetTimes();
/**
****************************************************************************************
* @brief:
* @param[in] ms
* @return
****************************************************************************************
*/
void BoatSleepMs(BUINT32 ms);


//!@brief Abstract definition of semaphore object based on Linux
#ifndef PLATFORM_OSAL_SEM
#define PLATFORM_OSAL_SEM
struct boatPlatformSem{
    sem_t semid; //! Linux semaphore descriptor
};
#endif

//!@brief Abstract definition of mutex object based on Linux
#ifndef PLATFORM_OSAL_MUTEX
#define PLATFORM_OSAL_MUTEX
struct boatPlatformMutex{
    pthread_mutex_t mutexid; //! Linux mutex descriptor
};
#endif

//!@brief Abstract definition of message queue object based on Linux
#ifndef PLATFORM_OSAL_QUEUE
#define PLATFORM_OSAL_QUEUE
struct boatPlatformQueue{
    mqd_t queueId; //! Linux message queue descriptor
    char *name; //! The name of message queue
};
#endif

//!@brief Abstract definition of task object based on Linux
#ifndef PLATFORM_OSAL_TASK
#define PLATFORM_OSAL_TASK
//#define PTHREAD_STACK_MIN 8388608
struct boatPlatformTask{
    pthread_t taskId; //! Linux thread descriptor
};
#endif

//!@brief Abstract definition of timer object based on Linux

#ifndef PLATFORM_OSAL_TIMER
#define PLATFORM_OSAL_TIMER

typedef void (*timerCallback)(void *);

struct boatPlatformTimer{
    timer_t timerId; //! Linux timer descriptor
    timerCallback timerCb;
    void *argv;
    int oneTime;
};
#endif

#endif
