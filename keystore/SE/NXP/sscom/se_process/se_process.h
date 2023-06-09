/**
****************************************************************************************
* @FilePath: se_process.h
* @Author: aitos
* @Date: 2022-11-21 12:08:38
* @LastEditors:
* @LastEditTime: 2022-11-21 12:08:38
* @Descripttion:
****************************************************************************************
*/
#ifndef _SE_PROCESS_H_
#define _SE_PROCESS_H_
#include "boaterrcode.h"
#include "boattypes.h"
#include "boatlog.h"

/**
****************************************************************************************
* @brief:
* This function open se and get atr response;
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
BOAT_RESULT boat_open_se(void);

/**
****************************************************************************************
* @brief:
*  This function send apdu to SE.
* @param[in] *apdu
*  apdu command to se
* @param[in] len
*  length of apdu command
* @param[in] *res
*  result of apdu executing
* @param[in] *reslen
*  length of result
* @param[in] *sw
*  return error code of apdu excuting
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
BOAT_RESULT boat_send_apdu(BUINT8 *apdu, BUINT16 len, BUINT8 *res, BUINT32 *reslen, BUINT16 *sw);

/**
****************************************************************************************
* @brief:
*  This function close se.
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
BOAT_RESULT boat_se_close(void);

#endif