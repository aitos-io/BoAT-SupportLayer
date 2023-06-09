/**
****************************************************************************************
* @FilePath: boatTimer.c
* @Author: aitos
* @Date: 2022-12-22 16:10:46
* @LastEditors:
* @LastEditTime: 2022-12-22 16:10:46
* @Descripttion:
****************************************************************************************
*/
// #include "boat_timer.h"
#include "boatosal.h"
#include "boatlog.h"
//#include "fibo_opencpu.h"
#include "osi_api.h"
#include <time.h>

BOAT_RESULT boatTimerStart(boatTimer *timerRef, BUINT32 initialTime, BUINT32 intervalTime, void (*callbackRoutine)(void *), void *argv)
{
    bool rtnVal;
    if ((timerRef == NULL) || (callbackRoutine == NULL) || ((initialTime == 0) && (intervalTime == 0)))
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart bad parameter\r\n");
        return BOAT_ERROR;
    }

    
    osiTimer_t *timer = osiTimerCreate(osiThreadCurrent(),callbackRoutine,argv);
    if(timer == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart Create a timer failed !");
        return BOAT_ERROR;
    }

    if (intervalTime > 0) ///// period timer
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart start a period timer[%x][%d]\r\n", intervalTime, intervalTime);

        rtnVal = osiTimerSetPeriod(timer,intervalTime,true);
        if(!rtnVal)
        {
            BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart set a period timer failed [%x]\r\n", rtnVal);
            osiTimerStop(timer);
            osiTimerDelete(timer);
            return BOAT_ERROR;
        }

        rtnVal = osiTimerStartLast(timer);
        if(!rtnVal)
        {
            BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart start a period timer failed [%x]\r\n", rtnVal);
            osiTimerStop(timer);
            osiTimerDelete(timer);
            return BOAT_ERROR;
        }
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart start a one time timer[%x][%d]\r\n", initialTime, initialTime);
        rtnVal = osiTimerStart(timer,initialTime);
        if(!rtnVal)
        {
            BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart start a period timer failed [%x]\r\n", rtnVal);
            osiTimerStop(timer);
            osiTimerDelete(timer);
            return BOAT_ERROR;
        }  
    }

    timerRef->timer = timer;

    return BOAT_SUCCESS;
}

BOAT_RESULT boatTimerDestroy(boatTimer *timerRef)
/////BOAT_RESULT boatTimer_free(BUINT32 timerID)
{
    if (timerRef == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerDestroy bad parameter\r\n");
        return BOAT_ERROR;
    }
	if(timerRef->timer != NULL)
    {

        osiTimerStop(timerRef->timer);
        
        
        osiTimerDelete(timerRef->timer);

		timerRef->timer = NULL;
		return BOAT_SUCCESS;
	}
	return BOAT_ERROR;
}

BOAT_RESULT boatTimestamp(BSINT64 *timestamp)
{
    *timestamp = (BSINT64)(time(NULL));
	return BOAT_SUCCESS;
}

void boatTimerInitTimeridZero(boatTimer *timerRef)
{
	timerRef->timer = NULL;
}

void boatTimerInitTimeridInvalid(boatTimer *timerRef)
{
	timerRef->timer = NULL;
}
