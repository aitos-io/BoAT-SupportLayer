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
#include "simcom_os.h"
#include "simcom_common.h"
#include "simcom_debug.h"

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
    SC_STATUS result = -1;
    if (mutex == NULL)
    {
        return BOAT_ERROR;
    }

    result = sAPI_MutexCreate(&(mutex->mutexRef),SC_FIFO);
    if(result != SC_SUCCESS)
    {
        mutex->mutexRef = NULL;
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

    if (mutex->mutexRef != NULL)
    {
        sAPI_MutexDelete(mutex->mutexRef); ///// no return value
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
    SC_STATUS os_status;

    if (mutex == NULL)
    {
        return BOAT_ERROR;
    }

    if(mutex->mutexRef == NULL)
    {
        return BOAT_ERROR;
    }

    os_status = sAPI_MutexLock(mutex->mutexRef,timeout);
    if(os_status != SC_SUCCESS)
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
    if (mutex->mutexRef != NULL)
    {
        sAPI_MutexUnLock(mutex->mutexRef);
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
        mutex->mutexRef = NULL;
        return BOAT_SUCCESS;
    }
    return BOAT_ERROR;
}
