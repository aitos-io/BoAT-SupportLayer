#include "boatosal.h"

#ifdef PLATFORM_OSAL_MUTEX

#include "stdio.h"
#include "osi_api.h"

#include "boattypes.h"
#include "boatlog.h"

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

    if(mutexRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    mutexRef->mutexId = osiMutexCreate();
    if(mutexRef->mutexId == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Mutex init failed!");
        return BOAT_ERROR;
    }

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

    if(mutexRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(mutexRef->mutexId == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad mutex id!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    osiMutexDelete(mutexRef->mutexId);

    mutexRef->mutexId = NULL;
    
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

    if(mutexRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(mutexRef->mutexId == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad mutexId!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    osiMutexUnlock(mutexRef->mutexId);


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

@param[in] timeout 
    timeout specifies a limit on the amount of time (in milliseconds) that the call should \n
    block if the decrement cannot be immediately performed.

*******************************************************************************/
BOAT_RESULT boatMutexLock(boatMutex *mutexRef,BUINT32 timeout)
{
    
    if(mutexRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(mutexRef->mutexId == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad mutexId!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(timeout > 0)
    {
        bool ret;
        ret = osiMutexTryLock(mutexRef->mutexId,timeout);
        if(ret == false)
        {
            BoatLog(BOAT_LOG_CRITICAL,"Mutex lock Failed!");
            return BOAT_ERROR;
        }
    }
    else
    {
        osiMutexLock(mutexRef->mutexId);
    }

    
    return BOAT_SUCCESS;
}

#endif/////PLATFORM_OSAL_MUTEX