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
#include "boat_timer.h"
#include "boatosal.h"
#include "boatlog.h"


BOAT_RESULT boatTimerStart(boatTimer *timerRef, BUINT32 initialTime, BUINT32 intervalTime, void (*callbackRoutine)(void *), void *argv)
{
	BUINT32 rtnVal;

	if((timerRef == NULL) || (callbackRoutine == NULL) || ((initialTime == 0) && (intervalTime == 0)))
	{
		BoatPrintf(0,"[boat][timer] boatTimerStart bad parameter\r\n");
		return BOAT_ERROR;
	}
	if(intervalTime > 0) ///// period timer
	{
		BoatPrintf(0,"[boat][timer] boatTimerStart start a period timer[%x][%d]\r\n",intervalTime,intervalTime);
	    rtnVal = fibo_timer_period_new(intervalTime,callbackRoutine,argv);
		timerRef->timerID = rtnVal;
		if(rtnVal > 0)
		{
			BoatPrintf(0,"[boat][timer] boatTimerStart start a period timer succ [%x]\r\n",rtnVal);
			return BOAT_SUCCESS;
		}
		else
		{
			BoatPrintf(0,"[boat][timer] boatTimerStart start a period timer failed [%x]\r\n",rtnVal);
			return BOAT_ERROR;
		}		
	}
	else
	{
		BoatPrintf(0,"[boat][timer] boatTimerStart start a one time timer[%x][%d]\r\n",initialTime,initialTime);
	    rtnVal = fibo_timer_new(initialTime,callbackRoutine,argv);
		timerRef->timerID = rtnVal;
		if(rtnVal > 0)
		{
			BoatPrintf(0,"[boat][timer] boatTimerStart one time timer succ [%x]\r\n",rtnVal);
			return BOAT_SUCCESS;
		}
		else
		{
			BoatPrintf(0,"[boat][timer] boatTimerStart one time timer failed [%x]\r\n",rtnVal);
			return BOAT_ERROR;
		}		
	}
}
#if 0
BOAT_RESULT boatTimer_period_new(BUINT32 ms, void (*fn)(void *arg), void *arg)
{
    return fibo_timer_period_new(ms, fn, arg);
}
#endif

BOAT_RESULT boatTimerDestroy(boatTimer *timerRef)
/////BOAT_RESULT boatTimer_free(BUINT32 timerID)
{
	if(timerRef == NULL)
	{
		BoatPrintf(0,"[boat][timer] boatTimerDestroy bad parameter\r\n");
		return BOAT_ERROR;
	}
	if(timerRef->timerID > 0)
	{
		bool bl;
		BSINT32 rtnVal = 0;
		
    	bl = fibo_timer_stop(timerRef->timerID);
		if(bl)
		{
			BoatPrintf(0,"[boat][timer] fibo_timer_stop bool true\r\n");
    	}
		else
		{
			BoatPrintf(0,"[boat][timer] fibo_timer_stop bool false\r\n");
		}
		rtnVal = fibo_timer_free(timerRef->timerID);

		BoatPrintf(0,"[boat][timer] fibo_timer_free rtnVal[%d]\r\n",rtnVal);

		timerRef->timerID = 0;
		return BOAT_SUCCESS;
	}
	return BOAT_ERROR;
}

BOAT_RESULT boat_Timestamp(BSINT64 *timestamp)
{
    BOAT_RESULT ret = BOAT_SUCCESS;
    *timestamp = osiEpochSecond();
    return ret;
}

void boatTaskInitTimeridZero(boatTimer *timerRef)
{
	timerRef->timerID = 0;
}

void boatTaskInitTimeridInvalid(boatTimer *timerRef)
{
	timerRef->timerID = 0x80ba2d00;
}

