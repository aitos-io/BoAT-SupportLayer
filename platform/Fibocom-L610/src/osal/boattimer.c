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
#include "fibo_opencpu.h"

BOAT_RESULT boatTimerStart(boatTimer *timerRef, BUINT32 initialTime, BUINT32 intervalTime, void (*callbackRoutine)(void *), void *argv)
{
    BUINT32 rtnVal;

    if ((timerRef == NULL) || (callbackRoutine == NULL) || ((initialTime == 0) && (intervalTime == 0)))
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart bad parameter\r\n");
        return BOAT_ERROR;
    }
    if (intervalTime > 0) ///// period timer
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart start a period timer[%x][%d]\r\n", intervalTime, intervalTime);
        rtnVal = fibo_timer_period_new(intervalTime, callbackRoutine, argv);
		timerRef->timerId = rtnVal;
        if (rtnVal > 0)
        {
            BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart start a period timer succ [%x]\r\n", rtnVal);
            return BOAT_SUCCESS;
        }
        else
        {
            BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart start a period timer failed [%x]\r\n", rtnVal);
            return BOAT_ERROR;
        }
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart start a one time timer[%x][%d]\r\n", initialTime, initialTime);
        rtnVal = fibo_timer_new(initialTime, callbackRoutine, argv);
		timerRef->timerId = rtnVal;
        if (rtnVal > 0)
        {
            BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart one time timer succ [%x]\r\n", rtnVal);
            return BOAT_SUCCESS;
        }
        else
        {
            BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart one time timer failed [%x]\r\n", rtnVal);
            return BOAT_ERROR;
        }
    }
}

BOAT_RESULT boatTimerDestroy(boatTimer *timerRef)
/////BOAT_RESULT boatTimer_free(BUINT32 timerID)
{
    if (timerRef == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerDestroy bad parameter\r\n");
        return BOAT_ERROR;
    }
	if(timerRef->timerId > 0)
    {
        bool bl;
        BSINT32 rtnVal = 0;

    	bl = fibo_timer_stop(timerRef->timerId);
        if (bl)
        {
            BoatLog(BOAT_LOG_NORMAL, "[boat][timer] fibo_timer_stop bool true\r\n");
        }
        else
        {
            BoatLog(BOAT_LOG_NORMAL, "[boat][timer] fibo_timer_stop bool false\r\n");
        }
		rtnVal = fibo_timer_free(timerRef->timerId);

        BoatLog(BOAT_LOG_NORMAL, "[boat][timer] fibo_timer_free rtnVal[%d]\r\n", rtnVal);

		timerRef->timerId = 0;
		return BOAT_SUCCESS;
	}
	return BOAT_ERROR;
}

BOAT_RESULT boatTimestamp(BSINT64 *timestamp)
{
    BOAT_RESULT ret = BOAT_SUCCESS;
    *timestamp = osiEpochSecond();
    return ret;
}

void boatTimerInitTimeridZero(boatTimer *timerRef)
{
	timerRef->timerId = 0;
}

void boatTimerInitTimeridInvalid(boatTimer *timerRef)
{
	timerRef->timerId = 0x80ba2d00;
}
