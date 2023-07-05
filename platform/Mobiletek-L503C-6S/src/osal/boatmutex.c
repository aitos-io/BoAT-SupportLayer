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
#include "mbtk_comm_api.h"
#include "mbtk_os.h"

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
    mbtk_os_status os_status;

    if (mutex == NULL)
    {
        return BOAT_ERROR;
    }

    os_status = ol_os_mutex_creat(&(mutex->mutexID), MBTK_OS_FIFO);
    if(os_status != mbtk_os_success) /////	fibocom mutex id is a UINT32 type, if the return value does not equal 0,the value is valid.
    {
        mutex->mutexID = NULL;
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
        ol_os_mutex_delete(mutex->mutexID); ///// no return value
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
    mbtk_os_status os_status;

    if (mutex == NULL)
    {
        return BOAT_ERROR;
    }

    if(mutex->mutexID == NULL)
    {
        return BOAT_ERROR;
    }

    os_status = ol_os_mutex_lock(mutex->mutexID,timeout);
    if(os_status != mbtk_os_success)
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
        ol_os_mutex_unlock(mutex->mutexID);
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
