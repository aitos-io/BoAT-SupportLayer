#include "boatosal.h"


#ifdef PLATFORM_OSAL_TIMER

#include "cm_os.h"

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
BOAT_RESULT boatTimerStart(boatTimer *timerRef, BUINT32 initialTime, BUINT32 intervalTime, void (*callbackRoutine)(void *), void *argv);

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
BOAT_RESULT boatTimerDestroy(boatTimer *timerRef);

/*!*****************************************************************************
@brief GET the period of a OS clock tick

Function: boatGetTickPeriod()

    This function return a OS clock tick period in millisecond.

@return
    This function returns a OS clock tick period in millisecond.

@param[in] null
*******************************************************************************/
BUINT32 boatGetTickPeriod(void);
#endif/////PLATFORM_OSAL_TIMER