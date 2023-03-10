#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


#include "boatosal.h"
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"


#ifdef PLATFORM_OSAL_MUTEX


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
BOAT_RESULT boatMutexInit(boatMutex *mutexRef)
{
    if (mutexRef == NULL) 
    {
        BoatLog(BOAT_LOG_CRITICAL, "Bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    pthread_mutex_t myMutex;
    int ret = pthread_mutex_init(&myMutex , NULL);
    if(ret != 0)
    {
        return BOAT_ERROR;
    }

    mutexRef->mutexid = myMutex;

    return BOAT_SUCCESS;
}


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
BOAT_RESULT boatMutexDestroy(boatMutex *mutexRef)
{
    if (mutexRef == NULL) 
    {
        BoatLog(BOAT_LOG_CRITICAL, "Bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    int ret = pthread_mutex_destroy(&(mutexRef->mutexid));
    if(ret != 0)
    {
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}


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
BOAT_RESULT boatMutexUnlock(boatMutex *mutexRef)
{
    if (mutexRef == NULL) 
    {
        BoatLog(BOAT_LOG_CRITICAL, "Bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    int ret = pthread_mutex_unlock(&(mutexRef->mutexid));
    if(ret != 0)
    {
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}


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

@param[in] timeout (ms)
    timeout specifies a limit on the amount of time (in milliseconds) that the call should \n
    block if the decrement cannot be immediately performed.

*******************************************************************************/

// static void timeOutMutexTask(void *parm)
// {

//     BUINT32 timeTest = 0;//us

//     struct mutexTimerStru{
// 		boatMutex t_mutex;
// 		BUINT32 time;
// 	}  p_mutexTest;

//     //p_mutexTest = (struct mutexTimerStru *)parm;
//     memcpy(&p_mutexTest,(struct mutexTimerStru *)parm,sizeof(p_mutexTest));

//     while(timeTest < ((p_mutexTest.time)*1000))
//     {
//         usleep(1000);
//         timeTest += 1000;
//     }

//     pthread_mutex_unlock(&(p_mutexTest.t_mutex.mutexid));

//     return;

// }

BOAT_RESULT boatMutexLock(boatMutex *mutexRef,BUINT32 timeout)
{
    if(mutexRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    int ret = pthread_mutex_lock(&(mutexRef->mutexid));
    if(ret != 0)
    {
        BoatLog(BOAT_LOG_CRITICAL,"pthread_mutex_lock error!");
        return BOAT_ERROR;
    }
    
    if(timeout > 0)
    {
        // struct mutexTimerStru{
		//     boatMutex t_mutex;
		//     BUINT32 time;
	    // } mutexTest;

        // mutexTest.time = timeout;//ms
        // memcpy(&(mutexTest.t_mutex),mutexRef,sizeof(boatMutex));


        // printf("timeout = %d\n",mutexTest.time);

        // pthread_t timerThread;
	    // pthread_create(&timerThread,NULL,timeOutMutexTask,&mutexTest);
        
    }

    return BOAT_SUCCESS;
}

#endif/////PLATFORM_OSAL_MUTEX
