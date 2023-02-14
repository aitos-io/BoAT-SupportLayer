/**
****************************************************************************************
* @FilePath: boat_timer.h
* @Author: aitos
* @Date: 2022-12-22 16:10:56
* @LastEditors:
* @LastEditTime: 2022-12-22 16:10:56
* @Descripttion:
****************************************************************************************
*/

#ifndef _boatTimer_H_
#define _boatTimer_H_
#include "boatiotsdk.h"
#include "fibo_opencpu.h"

BOAT_RESULT boatTimer_period_new(BUINT32 ms, void (*fn)(void *arg), void *arg);
BOAT_RESULT boatTimer_free(BUINT32 timerID);
BOAT_RESULT boat_Timestamp(BSINT64 *timestamp);

#endif
