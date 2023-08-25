/**
****************************************************************************************
* @FilePath: boatMutex.c
* @Author: aitos
* @Date: 2022-11-21 11:26:07
* @LastEditors:
* @LastEditTime: 2022-11-21 11:26:08
* @Descripttion:
****************************************************************************************
*/

#include "cmsis_os2.h"

/////#include "boatMutex.h"
#include "boatosal.h"
#include "boatlog.h"

// boatMutex mutex;
/**
****************************************************************************************
* @brief:
*  This function creat or init mutex;
* @param[in] *mutex
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
// #define BoatPrintf ;\/\/BoatPrintf
BOAT_RESULT boatMutexInit(boatMutex *mutex)
{
    if (mutex == NULL)
    {
        return BOAT_ERROR;
    }

    mutex->mutexID = osMutexNew(NULL);
    if(mutex->mutexID == NULL) 
    {
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}

/**
****************************************************************************************
* @brief:
*  This function delete or destory mutex;
* @param[in] mutex
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
BOAT_RESULT boatMutexDestroy(boatMutex *mutex)
{

    if (mutex == NULL)
    {
        return BOAT_ERROR;
    }

    if (mutex->mutexID != NULL)
    {
        osMutexDelete(mutex->mutexID); ///// no return value
        BoatLog(BOAT_LOG_NORMAL, "boatMutexDestroy ok");
    }

    return BOAT_SUCCESS;
}
/**
****************************************************************************************
* @brief:
*  This function lock mutex;
* @param[in] *mutex
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
BOAT_RESULT boatMutexLock(boatMutex *mutex, BUINT32 timeout)
{

    if (mutex == NULL)
    {
        return BOAT_ERROR;
    }

    if(mutex->mutexID == NULL)
    {
        return BOAT_ERROR;
    }

    osStatus_t os_status = osMutexAcquire(mutex->mutexID,timeout);
    if(os_status != osOK)
    {
        BoatLog(BOAT_LOG_NORMAL, "boatMutexLock ok");
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}
/**
****************************************************************************************
* @brief:
    This function unlock mutex
* @param[in] *mutex
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
BOAT_RESULT boatMutexUnlock(boatMutex *mutex)
{

    if (mutex == NULL)
    {
        return BOAT_ERROR;
    }
    if (mutex->mutexID != NULL)
    {
        osMutexRelease(mutex->mutexID);
        return BOAT_SUCCESS;
    }
    else
    {
        return BOAT_ERROR;
    }
}

///// test functions
BOAT_RESULT boatMutexInitMutexidZero(boatMutex *mutex)
{
    if (mutex != NULL) /////if((mutex < BSSSTART) || (mutex >= BSSEND))
    {
        mutex->mutexID = NULL;
        return BOAT_SUCCESS;
    }
    return BOAT_ERROR;
}
