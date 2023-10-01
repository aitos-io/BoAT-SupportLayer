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

#include "mbtk_os.h"



BOAT_RESULT boatTimerStart(boatTimer *timerRef, BUINT32 initialTime, BUINT32 intervalTime, void (*callbackRoutine)(void *), void *argv)
{
    BUINT32 rtnVal;

    if ((timerRef == NULL) || (callbackRoutine == NULL) || ((initialTime == 0) && (intervalTime == 0)))
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][timer] boatTimerStart bad parameter\r\n");
        return BOAT_ERROR;
    }

    mbtk_os_status os_status = ol_os_timer_creat(&(timerRef->timerId));
    if((os_status != mbtk_os_success) || (timerRef->timerId == NULL))
    {
        return BOAT_ERROR;
    }

    os_status = ol_os_timer_start(timerRef->timerId,initialTime,intervalTime,callbackRoutine,argv);
    if(os_status != mbtk_os_success)
    {
        ol_os_timer_delete(timerRef->timerId);

        return BOAT_ERROR;
    }

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
	if(timerRef->timerId != NULL)
    {
        ol_os_timer_stop(timerRef->timerId);
        ol_os_timer_delete(timerRef->timerId);
        timerRef->timerId = NULL;
		return BOAT_SUCCESS;
	}
	return BOAT_ERROR;
}

BOAT_RESULT boatTimestamp(BSINT64 *timestamp)
{
    *timestamp  = ol_time(NULL);
    return BOAT_SUCCESS;
}

void boatTimerInitTimeridZero(boatTimer *timerRef)
{
	timerRef->timerId = NULL;
}
