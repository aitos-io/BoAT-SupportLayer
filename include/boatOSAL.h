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

/*!@brief The OSAL APIs

@file
* boatosal.h define the APIs for abstract object of OSAL in BoAT Support Layer,
* and it must include the platformOSAL.h of the specific OS.
*/
#ifndef __BOATOSAL_H__
#define __BOATOSAL_H__

#include <stdio.h>
#include "boattypes.h"
#include "boaterrcode.h"
#include "platformOSAL.h"


#ifdef PLATFORM_OSAL_SEMA
typedef struct boat_platform_sem boat_sem;

/*!*****************************************************************************
@brief Init a Boat semaphore

Function: BoAT_sem_init()

    This function create a semaphore instance and init the boat semaphore.

    BoAT_sem_init() MUST be called before any use of BoAT semaphore.
    BoAT_sem_destroy() MUST be called after use of BoAT semaphore.

@return
    This function returns BOAT_SUCCESS if the initialization is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.
    

@param[in] semaRef
    A boat_sem struct pointer,the BoAT_sem_init() function shall \n
    initialize the boat semaphore referred to by semaRef.

@param[in] initialCount
    The value of the initialized boat semaphore shall be initialCount.
        
*******************************************************************************/
BOAT_RESULT BoAT_sem_init(boat_sem *semaRef,int initialCount);

/*!*****************************************************************************
@brief Destroy a Boat semaphore

Function: BoAT_sem_destroy()

    This function destroys a semaphore instance and deinit the boat semaphore.

@return
    This function returns BOAT_SUCCESS if the destroying is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.
    

@param[in] semaRef
    A boat_sem struct pointer,the BoAT_sem_destroy() function shall \n
    destroy the boat semaphore indicated by semaRef.

*******************************************************************************/
BOAT_RESULT BoAT_sem_destroy(boat_sem *semaRef);

/*!*****************************************************************************
@brief Unlock a Boat semaphore

Function: BoAT_sem_post()

     The BoAT_sem_post() function shall unlock the Boat semaphore referenced by\n
     semaRef. 

@return
    This function returns BOAT_SUCCESS if the unlock operation is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to indicate the error, \n
    for reference in boaterrcode.h.    

@param[in] semaRef
    A boat_sem struct pointer,The BoAT_sem_post() function shall unlock\n
    the Boat semaphore referenced by semaRef by performing a semaphore \n
    unlock operation on that BoAT semaphore.

*******************************************************************************/
BOAT_RESULT BoAT_sem_post(boat_sem *semaRef);

/*!*****************************************************************************
@brief Lock a Boat semaphore

Function: BoAT_sem_wait()

     The BoAT_sem_wait() function shall lock the Boat semaphore referenced by
     semaRef. 

@return
    This function returns BOAT_SUCCESS if the lock operation is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.\n\n

       If the timeout has already expired by the time of the call, and the \n
    boat semaphore could not be locked immediately, then BoAT_sem_wait() \n
    return a timeout error (BOAT_ERROR_TIMEOUT)

@param[in] semaRef
    A boat_sem struct pointer,The BoAT_sem_wait() function shall lock\n
    the Boat semaphore referenced by semaRef by performing a semaphore \n
    lock operation on that BoAT semaphore.

@param[in] timeout
    timeout specifies a limit on the amount of time that the call should \n
    block if the decrement cannot be immediately performed.

*******************************************************************************/
BOAT_RESULT BoAT_sem_wait(boat_sem *semaRef,BUINT32 timeout);
#endif

//!@brief The APIs of BoAT mutex object

#ifdef PLATFORM_OSAL_MUTEX
typedef struct boatPlatformMutex boatMutex;

/*!*****************************************************************************
@brief Init a Boat mutex

Function: boatMutexInit()

    This function create a mutex instance and init the boat mutex.

    boatMutexInit() MUST be called before any use of BoAT mutex.
    boatMutexDestroy() MUST be called after use of BoAT mutex.

@return
    This function returns BOAT_SUCCESS if the initialization is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.
    

@param[in] mutexRef
    A boatMutex struct pointer,the boatMutexInit() function shall \n
    initialize the boat mutex referred to by mutexRef.
        
*******************************************************************************/
BOAT_RESULT boatMutexInit(boatMutex *mutexRef);


/*!*****************************************************************************
@brief Destroy a Boat mutex

Function: boatMutexDestroy()

    This function destroys a mutex instance and deinit the boat mutex.

@return
    This function returns BOAT_SUCCESS if the destroying is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.
    

@param[in] mutexRef
    A boatMutex struct pointer,the boatMutexDestroy() function shall \n
    destroy the boat mutex indicated by mutexRef.

*******************************************************************************/
BOAT_RESULT boatMutexDestroy(boatMutex *mutexRef);


/*!*****************************************************************************
@brief Unlock a Boat mutex

Function: boatMutexUnlock()

     The boatMutexUnlock() function shall unlock the Boat mutex referenced by\n
     mutexRef. 

@return
    This function returns BOAT_SUCCESS if the unlock operation is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to indicate the error, \n
    for reference in boaterrcode.h.    

@param[in] mutexRef
    A boatMutex struct pointer,The boatMutexUnlock() function shall unlock\n
    the Boat mutex referenced by mutexRef by performing a mutex \n
    unlock operation on that BoAT mutex.

*******************************************************************************/
BOAT_RESULT boatMutexUnlock(boatMutex *mutexRef);


/*!*****************************************************************************
@brief Lock a Boat mutex

Function: boatMutexLock()

     The boatMutexLock() function shall lock the Boat mutex referenced by
     mutexRef. 

@return
    This function returns BOAT_SUCCESS if the lock operation is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.\n\n

       If the mutex could not be locked before the specified timeout expired.\n
       , then boatMutexLock() return a timeout error (BOAT_ERROR_TIMEOUT)

@param[in] mutexRef
    A boatMutex struct pointer,The boatMutexLock() function shall lock\n
    the Boat mutex referenced by mutexRef by performing a mutex \n
    lock operation on that BoAT mutex.

@param[in] timeout 
    timeout specifies a limit on the amount of time (seconds) that the call should \n
    block if the decrement cannot be immediately performed.

*******************************************************************************/
BOAT_RESULT boatMutexLock(boatMutex *mutexRef,BUINT32 timeout);

#endif

//!@brief The APIs of BoAT queue object

#ifdef PLATFORM_OSAL_QUEUE
typedef struct boat_platform_queue boat_queue;

/*!*****************************************************************************
@brief Init a Boat queue

Function: BoAT_msgqueue_init()

    This function create a queue instance and init the boat queue.

    BoAT_msgqueue_init() MUST be called before any use of BoAT queue.
    BoAT_msgqueue_delete() MUST be called after use of BoAT queue.

@return
    This function returns BOAT_SUCCESS if the initialization is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.
    

@param[in] msgQRef
    A boat_queue struct pointer,the BoAT_msgqueue_init() function shall \n
    initialize the boat queue referred to by msgQRef.

@param[in] queueName
    The name of the initialized boat queue.

@param[in] maxSize
    The maxSize is the max size of one message.

@param[in] maxNumber
    The maxNumber is the max amount of messages on queue.

*******************************************************************************/
BOAT_RESULT BoAT_msgqueue_init(boat_queue *msgQRef, char *queueName, BUINT32 maxSize, BUINT32 maxNumber);


/*!*****************************************************************************
@brief Destroy a Boat queue

Function: BoAT_msgqueue_delete()

    This function destroys a queue instance and deinit the boat queue.

@return
    This function returns BOAT_SUCCESS if the destroying is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.    

@param[in] msgQRef
    A boat_queue struct pointer,the BoAT_msgqueue_delete() function shall \n
    destroy the boat queue indicated by msgQRef.

*******************************************************************************/
BOAT_RESULT BoAT_msgqueue_delete(boat_queue *msgQRef);

/*!*****************************************************************************
@brief Send data to a boat queue

Function: BoAT_msgqueue_send()

     The BoAT_msgqueue_send() function shall send a message to the Boat queue \n
     referenced by msgQRef. 

@return
    This function returns BOAT_SUCCESS if the send operation is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to indicate the error,\n
    for reference in boaterrcode.h.    

@param[in] msgQRef
    A boat_queue struct pointer,the BoAT_msgqueue_send() function shall send \n
    data to the boat queue referred to by the boat queue descriptor msgQRef.

@param[in] msgPtr
    The msgPtr pointed to the start address of the data which will be put into\n
    the boat queue.

@param[in] msgLen
    The msgLen specifies the length (by byte) of the data pointed to by msg_ptr.

*******************************************************************************/
BOAT_RESULT BoAT_msgqueue_send(const boat_queue *msgQRef, unsigned char *msgPtr, BUINT32 msgLen);

/*!*****************************************************************************
@brief Receive data from a boat queue

Function: BoAT_msgqueue_recv()

     The BoAT_msgqueue_recv() function shall receive data from the Boat queue
     referenced by msgQRef. 

@return
    This function returns BOAT_SUCCESS if the operation is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.\n\n

       If the BoAT_msgqueue_recv() could not receive data frome the queue before\n
       the specified timeout expired, then BoAT_msgqueue_recv() return a timeout\n
       error (BOAT_ERROR_TIMEOUT)

@param[in] msgQRef
    A boat_queue struct pointer,the BoAT_msgqueue_recv() function shall receive\n
    data from the boat queue referred to by msgQRef.

@param[in] msgPtr
    The msg_ptr points to the buffer which is used to place the data received\n
    from the boat queue.

@param[in] msgLen
    The msgLen specifies the length (by byte) of the data received form a\n
    boat queue.

@param[in] timeout 
    The timeout specifies a limit on the amount of time (in seconds) that the call\n
    should block if the receiving cannot be immediately performed.

*******************************************************************************/
BOAT_RESULT BoAT_msgqueue_recv(const boat_queue *msgQRef, unsigned char *msgPtr, BUINT32 msgLen ,BUINT32 timeout);


#endif

//!@brief The APIs of BoAT task object

#ifdef PLATFORM_OSAL_TASK
#define BOAT_TASK_PRIORITY_HIGH		30
#define BOAT_TASK_PRIORITY_NORMAL	20
#define BOAT_TASK_PRIORITY_LOW		10

typedef struct boatPlatformTask boatTask;

/*!*****************************************************************************
@brief Create a Boat task

Function: boatTaskCreat()

    This function create a task/thread of the specific OS and \n
    init the boat task descriptor referred to by taskRef.

@return
    This function returns BOAT_SUCCESS if the initialization is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.

@param[in] taskRef
    A boatTask struct pointer,the boatTaskCreat() function shall \n
    initialize the boat task descriptor referred to by taskRef.

@param[in] stacksize
    boatTaskCreat() use stacksize to set the task/thread stack size.

@param[in] taskfunc
    The new task/thread starts execution by invoking taskfunc().

@param[in] argv
    The arg is passed as the sole argument of taskfunc().
*******************************************************************************/
BOAT_RESULT boatTaskCreat(boatTask *taskRef, char *taskName, BUINT32 stacksize, BUINT32 priority, void (*taskfunc)(void *), void *argv);

/*!*****************************************************************************
@brief Destroy a Boat task

Function: boatTaskDelete()

    This function destroys a task/thread instance of the specific OS and\n
    deinit the boat task descriptor referred to by taskRef.

    The boatTaskDelete() function must be called at the end of the\n
    taskfunc() before it return.\n\n
    example:
    void taskfunc(void *arg){
        int ret;
        
        while(condition){
            ;//do job
        }

        ret = boatTaskDelete();
        return NULL;
    }

@return
    This function returns BOAT_SUCCESS if the destroying is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.

@param[in] taskRef
    A boatTask struct pointer,the boatTaskDelete() function shall \n
    destroy the boat task indicated by taskRef and deinit the taskRef.

*******************************************************************************/
BOAT_RESULT boatTaskDelete(boatTask *taskRef);

#endif

#ifdef PLATFORM_OSAL_TIMER
typedef struct boatPlatformTimer boatTimer;

/*!*****************************************************************************
@brief Start a Boat timer

Function: boatTimerStart()

    This function create a timer/thread of the specific OS and \n
    init the boa timer descriptor referred to by timerRef.

@return
    This function returns BOAT_SUCCESS if the initialization is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.

@param[in] timerRef
    A boatTimer struct pointer,the boatTimerStart() function shall \n
    initialize the boat timer descriptor referred to by timerRef.

@param[in] initialTime
    Initial expiration time in OS clock ticks.
    The BoAT_getTickPeriod() can get the period of a tick.(in millisecond)

@param[in] intervalTime
    It indicates the period, in OS clock ticks, of a cyclic timer.\n
    If this field is nonzero, then each time that an armed timer expires,\n
    the timer is reloaded from the value specified in intervalTime,it is a 
    cycle timer.  If intervalTime specifies a zero value, then the timer \n
    expires just once,at the time specified by initialTime.

@param[in] callBackRoutine
    The new timer starts execution by invoking callBackRoutine().

@param[in] argv
    The argv is passed as the sole argument of callBackRoutine().
*******************************************************************************/
BOAT_RESULT boatTimerStart(boatTimer *timerRef, BUINT32 initialTime, BUINT32 intervalTime, void (*callBackRoutine)(void *), void *argv);

/*!*****************************************************************************
@brief Destroy a Boat timer

Function: boatTimerDestroy()

    This function destroys a timer/thread instance of the specific OS and\n
    deinit the boat timer descriptor referred to by timerRef.\n

@return
    This function returns BOAT_SUCCESS if the destroying is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.

@param[in] timerRef
    A boatTimer struct pointer,the boatTimerDestroy() function shall \n
    destroy the boat timer indicated by timerRef and deinit the timerRef.

*******************************************************************************/
BOAT_RESULT boatTimerDestroy(boatTimer *timerRef);

/*!*****************************************************************************
@brief GET the period of a OS clock tick

Function: BoAT_getTickPeriod()

    This function return a OS clock tick period in millisecond.

@return
    This function returns a OS clock tick period in millisecond.

@param[in] null
*******************************************************************************/
BUINT32 BoAT_getTickPeriod(void);
#endif

#endif
