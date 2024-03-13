#include "boatosal.h"


#ifdef PLATFORM_OSAL_TIMER

#include "boattypes.h"
#include "boatlog.h"

typedef struct cbarg{
void (*callbackRoutine)(void *);
void *argv;
}timerCbArg;
void ec800TimerRutine(BUINT32 arg)
{
	timerCbArg *pCbArg = (timerCbArg *)arg;
	pCbArg->callbackRoutine(pCbArg->argv);
}

/*!*****************************************************************************
@brief Start a Boat timer

Function: boatTimerStart()

    This function create a timer/thread of the specific OS and \n
    init the boa timer descriptor referred to by timerRef.

@return
    This function returns BOAT_SUCCESS if the initialization is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.

@param[in] timerRef
    A boatTimer struct pointer,the boatTimerStart() function shall \n
    initialize the boat timer descriptor referred to by timerRef.

@param[in] initialTime
    Initial expiration time in OS clock ticks.
    The boatGetTickPeriod() can get the period of a tick.(in millisecond)

@param[in] intervalTime
    It indicates the period, in OS clock ticks, of a cyclic timer.\n
    If this field is nonzero, then each time that an armed timer expires,\n
    the timer is reloaded from the value specified in intervalTime,it is a 
    cycle timer.  If intervalTime specifies a zero value, then the timer \n
    expires just once,at the time specified by initialTime.

@param[in] callBackRoutine
    The new timer starts execution by invoking callBackRoutine().

@param[in] argv
    The argv is passed as the sole argument of callBackRoutine().
*******************************************************************************/
BOAT_RESULT boatTimerStart(boatTimer *timerRef, BUINT32 initialTime, BUINT32 intervalTime, void (*callbackRoutine)(void *), void *argv)
{
    QlOSStatus  ret;
    if(timerRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR;
    }
	if(timerRef->timerId == 0)
	{
		ret = ql_rtos_timer_create(&timerRef->timerId);
		if(ret != 0)
		{
			return BOAT_ERROR;
		}
	}
	timerCbArg *pCbArg = malloc(sizeof(timerCbArg));
	if(NULL ==  pCbArg)
	{
		return BOAT_ERROR;
	}
	pCbArg->callbackRoutine = callbackRoutine;
	pCbArg->argv = argv;
	
    if(intervalTime > 0)
    {
        ret = ql_rtos_timer_start(timerRef->timerId,intervalTime,1,ec800TimerRutine,(BUINT32)argv);
    }
    else
    {
        ret = ql_rtos_timer_start(timerRef->timerId,initialTime,0,ec800TimerRutine,(BUINT32)argv);
    }

    if(ret != BOAT_SUCCESS)
    {
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief Destroy a Boat timer

Function: boatTimerDestroy()

    This function destroys a timer/thread instance of the specific OS and\n
    deinit the boat timer descriptor referred to by timerRef.\n

@return
    This function returns BOAT_SUCCESS if the destroying is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.

@param[in] timerRef
    A boatTimer struct pointer,the boatTimerDestroy() function shall \n
    destroy the boat timer indicated by timerRef and deinit the timerRef.

*******************************************************************************/
BOAT_RESULT boatTimerDestroy(boatTimer *timerRef)
{
    QlOSStatus ret;
    if(timerRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR;
    }

    ret = ql_rtos_timer_stop(timerRef->timerId);
    if(ret != BOAT_SUCCESS)
    {
        return BOAT_ERROR;
    }

    ret = ql_rtos_timer_delete(timerRef->timerId);
    if(ret != BOAT_SUCCESS)
    {
        return BOAT_ERROR;
    }
	timerRef->timerId = 0;

    return BOAT_SUCCESS;
}


#endif/////PLATFORM_OSAL_TIMER