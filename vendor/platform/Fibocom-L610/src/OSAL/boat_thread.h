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

#ifndef _BOAT_THREAD_H_
#define _BOAT_THREAD_H_
#include "fibo_opencpu.h"

BOAT_RESULT boat_thread_creat(void *pvTaskCode, INT8 *pcName, UINT32 usStackDepth, void *pvParameters, UINT32 uxPriority);
void boat_thread_delete(void);

#endif
