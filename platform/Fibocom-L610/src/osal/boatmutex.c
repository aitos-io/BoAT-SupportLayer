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
#include "fibo_opencpu.h"
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
#define BSSSTART 0x80f40840 ///// 0x80f40840: __bss_start
#define BSSEND 0x80f412a0   ///// 0x80f412a0: __ram_end = (ADDR (.bss) + SIZEOF (.bss))
// #define BoatPrintf ;\/\/BoatPrintf
BOAT_RESULT boatMutexInit(boatMutex *mutex)
{
    BOAT_RESULT ret = BOAT_SUCCESS;
    BUINT32 fibo_mutex = 0;
    BoatLog(BOAT_LOG_NORMAL, "begin to boatMutexInit mutex address[%x]", (BUINT32)mutex);
    if (mutex == NULL) /////if((mutex < BSSSTART) || (mutex >= BSSEND))
    {
        BoatLog(BOAT_LOG_NORMAL, "The boatMutex address is illegal in boatMutexInit, [%x ~ %x] bad address:%x,ph", BSSSTART, BSSEND, (BUINT32)mutex);
        return BOAT_ERROR;
    }

    fibo_mutex = fibo_mutex_create();
    BoatLog(BOAT_LOG_NORMAL, "boatMutexInit OK id[%x]", fibo_mutex);
    if (fibo_mutex != 0) /////	fibocom mutex id is a UINT32 type, if the return value does not equal 0,the value is valid.
    {
        mutex->mutexID = fibo_mutex;
        BoatLog(BOAT_LOG_NORMAL, "boatMutexInit OK 222,rtnVal[%x]", mutex->mutexID);
    }
    else
    {
        mutex->mutexID = fibo_mutex;
        BoatLog(BOAT_LOG_NORMAL, "boatMutexInit failed,rtnVal is 0, ph");
        ret = BOAT_ERROR;
    }
    return ret;
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
    BOAT_RESULT ret = BOAT_SUCCESS;

    if (mutex == NULL) /////if((mutex < BSSSTART) || (mutex >= BSSEND))
    {
        BoatLog(BOAT_LOG_NORMAL, "The boatMutex address is illegal in boatMutexDestroy, [%x ~ %x] bad address:%x,ph", BSSSTART, BSSEND, (BUINT32)mutex);
        return BOAT_ERROR;
    }

    BoatLog(BOAT_LOG_NORMAL, "begin to boatMutexDestroy ID[%x]", mutex->mutexID);
    if (mutex->mutexID != 0)
    {
        fibo_mutex_delete(mutex->mutexID); ///// no return value
        BoatLog(BOAT_LOG_NORMAL, "boatMutexDestroy ok");
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "boatMutexDestroy failed,rtnVal is 0, ph");
        ret = BOAT_ERROR;
    }
    return ret;
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
    BOAT_RESULT ret = BOAT_SUCCESS;
    BSINT32 rtnVal = 0;

    if (mutex == NULL) /////if((mutex < BSSSTART) || (mutex >= BSSEND))
    {
        BoatLog(BOAT_LOG_NORMAL, "The boatMutex address is illegal in boatMutexLock, [%x ~ %x] bad address:%x,ph", BSSSTART, BSSEND, (BUINT32)mutex);
        return BOAT_ERROR;
    }
    BoatLog(BOAT_LOG_NORMAL, "begin to boatMutexLock ID[%x] timeout[%d]", mutex->mutexID, timeout);
    if (timeout > 0) ///// try lock
    {
        if (mutex->mutexID != 0)
        {
            BoatLog(BOAT_LOG_NORMAL, "boatMutexLock call fibo_mutex_try_lock ID[%x] timeout[%d]", mutex->mutexID, timeout);
            rtnVal = fibo_mutex_try_lock(mutex->mutexID, timeout); // it will be blocked untill the mutex is unloked
            BoatLog(BOAT_LOG_NORMAL, "boatMutexLock retrun from fibo_mutex_try_lock ID[%x] timeout[%d]", mutex->mutexID, timeout);
            if (rtnVal < 0) // timeout
            {
                BoatLog(BOAT_LOG_NORMAL, "boatMutexLock time out ID[%x] timeout[%d]", mutex->mutexID, timeout);
                ret = rtnVal + BOAT_ERROR; ///// need check the error value, define the same TIMEOUT ERROR value for OSAL ,ph
            }
            else
            {

                BoatLog(BOAT_LOG_NORMAL, "boatMutexLock succ ID[%x] timeout[%d]", mutex->mutexID, timeout);
            }
        }
        else
        {
            ret = BOAT_ERROR;
        }
    }
    else
    {
        if (mutex->mutexID != 0)
        {
            BoatLog(BOAT_LOG_NORMAL, "boatMutexLock call fibo_mutex_lock ID[%x]", mutex->mutexID);
            fibo_mutex_lock(mutex->mutexID); ///// it will be blocked untill the mutex is unloked ,no return value
            BoatLog(BOAT_LOG_NORMAL, "boatMutexLock return from fibo_mutex_lock ID[%x]", mutex->mutexID);
        }
        else
        {
            BoatLog(BOAT_LOG_NORMAL, "boatMutexLock mutex id = 0  ID[%x]", mutex->mutexID);
            ret = BOAT_ERROR;
        }
    }

    return ret;
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
    BOAT_RESULT ret = BOAT_SUCCESS;

    if (mutex == NULL) /////if((mutex < BSSSTART) || (mutex >= BSSEND))
    {
        BoatLog(BOAT_LOG_NORMAL, "The boatMutex address is illegal in boatMutexUnlock, [%x ~ %x] bad address:%x,ph", BSSSTART, BSSEND, (BUINT32)mutex);
        return BOAT_ERROR;
    }
    if (mutex->mutexID != 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "boatMutexUnlock call fibo_mutex_unlock ID[%x] ", mutex->mutexID);
        fibo_mutex_unlock(mutex->mutexID); ///// no return value
        BoatLog(BOAT_LOG_NORMAL, "boatMutexUnlock retrun from fibo_mutex_unlock ID[%x] ", mutex->mutexID);
    }
    else
    {
        ret = BOAT_ERROR;
    }

    return ret;
}

///// test functions
BOAT_RESULT boatMutexInitMutexidZero(boatMutex *mutex)
{
    if (mutex != NULL) /////if((mutex < BSSSTART) || (mutex >= BSSEND))
    {
        mutex->mutexID = 0;
        return BOAT_SUCCESS;
    }
    return BOAT_ERROR;
}
