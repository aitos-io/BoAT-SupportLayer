/**
****************************************************************************************
* @FilePath: se_process.c
* @Author: aitos
* @Date: 2022-11-21 12:08:30
* @LastEditors:
* @LastEditTime: 2022-11-21 12:08:30
* @Descripttion:
****************************************************************************************
*/
#include "se_process.h"
#include "smComT1oI2C.h"
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
BOAT_RESULT boat_open_se(void)
{
    BOAT_RESULT ret = BOAT_SUCCESS;
    BUINT8 ATR[64] = {0};
    BUINT16 atrlen = 0;

    ret = smComT1oI2C_Open(NULL, ESE_MODE_NORMAL, 0, ATR, &atrlen);
    if (ret != SMCOM_OK)
    {
        return BOAT_ERROR;
    }
    return BOAT_SUCCESS;
}

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
BOAT_RESULT boat_send_apdu(BUINT8 *apdu, BUINT16 len, BUINT8 *res, BUINT32 *reslen, BUINT16 *sw)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT32 ret = SMCOM_OK;
    ret = smCom_TransceiveRaw(NULL, apdu, len, res, (U32 *)reslen);
    if (ret != SMCOM_OK)
    {
        return BOAT_ERROR;
    }
    else
    {
        *sw = (res[*reslen - 1]) | (res[*reslen - 2] << 8);
        BoatLog(BOAT_LOG_NORMAL, " boat_send_apdu sw = %x ", *sw);
        return result;
    }
}
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
BOAT_RESULT boat_se_close(void)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT16 ret = SMCOM_OK;
    ret = smComT1oI2C_Close(NULL, ESE_MODE_NORMAL);
    if (ret != SMCOM_OK)
    {
        return BOAT_ERROR;
    }
    return result;
}