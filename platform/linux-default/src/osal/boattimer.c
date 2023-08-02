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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/times.h>
#include <sys/types.h>

static void boatTimerDefaultThread(union sigval sigv)
{
    boatTimer *timerRef = NULL;
	
	if(sigv.sival_ptr == NULL )
	{
		return;
	}

    timerRef = (boatTimer *)sigv.sival_ptr;
    if (timerRef->timerCb != NULL) {
        timerRef->timerCb(timerRef->argv);
    }

    if (timerRef->oneTime) {
        boatTimerDestroy(timerRef);
    }
}

#ifdef USEGETTIMESTAMP
void boatGetTimeStamp(char   * p)
{
    time_t ltime;
    struct tm *today;
    struct timeb timebuffer;

    time( &ltime );
    today = localtime( &ltime );
    ftime( &timebuffer);

    sprintf(p, 
			"%04d%02d%02d_%02d%02d%02d_%03d", 
			1900 + today->tm_year,
			today->tm_mon + 1,
			today->tm_mday,
			today->tm_hour,
			today->tm_min,
			today->tm_sec,
			timebuffer.millitm);
}
#endif

BOAT_RESULT boatTimerStart(boatTimer *timerRef, BUINT32 initialTime, BUINT32 intervalTime, void (*callbackRoutine)(void *), void *argv)
{
    timer_t timerId;
    struct sigevent sige;
    struct itimerspec its;
    

    if (timerRef == NULL) {
        return BOAT_ERROR;
    }
    if ((intervalTime == 0) && (initialTime == 0)) {
        return BOAT_ERROR;
    }
    if (callbackRoutine == NULL) {
        return BOAT_ERROR;
    }

    timerRef->argv = argv;
    timerRef->timerCb = callbackRoutine;
    timerRef->oneTime = (intervalTime) ? 0 : 1;

    memset(&sige, 0, sizeof(struct sigevent));

    sige.sigev_value.sival_ptr = (void *)timerRef;
    sige.sigev_notify = SIGEV_THREAD; // create a new thread for sigev_notify_function
    sige.sigev_notify_function = boatTimerDefaultThread;

    if (timer_create(CLOCK_REALTIME, &sige, &timerId) != 0) {
        return BOAT_ERROR;
    }
	timerRef->timerId = timerId;

	if (initialTime == 0)	/////? Is 0  triggering callback immediatly
	{
		initialTime = intervalTime;
	}
    its.it_value.tv_sec = initialTime / 1000;
    its.it_value.tv_nsec = (initialTime % 1000) * 1000 * 1000;

    its.it_interval.tv_sec = intervalTime / 1000;
    its.it_interval.tv_nsec = (intervalTime % 1000) * 1000 * 1000;

    if (timer_settime(timerRef->timerId, 0, &its, NULL) != 0) {
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
 }

BOAT_RESULT boatTimerDestroy(boatTimer *timerRef)
{
    if (timerRef == NULL) {
        return BOAT_ERROR;
    }
	if(timerRef->timerId == 0)
	{
		return BOAT_ERROR;
	}

    if(timer_delete(timerRef->timerId) == 0)
    {
		memset(timerRef,0,sizeof(boatTimer));
		return BOAT_SUCCESS;
    }
    BoatLog(0, "timer delete fialed (timerId=%p)", timerRef->timerId);
	return BOAT_ERROR;
}

BOAT_RESULT boatTimestamp(BSINT64 *timestamp)
{
    *timestamp = (BSINT64)(time(NULL));
	return BOAT_SUCCESS;
}

void boatTimerInitTimeridZero(boatTimer *timerRef)
{
	timerRef->timerId = 0;
}

#ifdef USETIMERIDINVALID
void boatTimerInitTimeridInvalid(boatTimer *timerRef)
{
	timerRef->timerId = (timer_t)0x80ba2d00;
}
#endif


