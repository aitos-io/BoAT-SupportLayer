/*
 *
 * Copyright 2016-2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @par Description
 * Implements installable communication layer to exchange APDU's between Host and Secure Module.
 * Allows the top half of the Host Library to be independent of the actual interconnect
 * between Host and Secure Module
 */
#include <stdio.h>
#include "smCom.h"
/////#include "boat_Mutex.h"
#include "boatosal.h"
#include "boatlog.h"
// #include "nxLog_smCom.h"

#if defined(USE_RTOS) && (USE_RTOS == 1)
#include "FreeRTOS.h"
#include "semphr.h"
#endif

#if defined(SMCOM_JRCP_V2)
#include "smComJRCP.h"
#endif

/* Only for base session with os */
/////static boatMutex gSmComlock = 0;
static boatMutex gSmComlock;

#if (__GNUC__ && !AX_EMBEDDED) || (USE_RTOS)
#define USE_LOCK 1
#else
#define USE_LOCK 0
#endif

#define LOCK_TXN() \
    boatMutexLock(&gSmComlock,0);	///// add ",0",set timeout value 0,no timeout 230116

#define UNLOCK_TXN() \
    boatMutexUnlock(&gSmComlock);

static ApduTransceiveFunction_t pSmCom_Transceive = NULL;
static ApduTransceiveRawFunction_t pSmCom_TransceiveRaw = NULL;

/**
 * Install interconnect and protocol specific implementation of APDU transfer functions.
 *
 */
U16 smCom_Init(ApduTransceiveFunction_t pTransceive, ApduTransceiveRawFunction_t pTransceiveRaw)
{
    BoatLog(BOAT_LOG_NORMAL, "i2c test ");
    U16 ret = SMCOM_COM_INIT_FAILED;
    BOAT_RESULT result = BOAT_SUCCESS;

    result = boatMutexInit(&gSmComlock);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "\n fibo_mutex_create failed");
        return SMCOM_COM_INIT_FAILED;
    }

    pSmCom_Transceive = pTransceive;
    pSmCom_TransceiveRaw = pTransceiveRaw;
    ret = SMCOM_OK;
    return ret;
}

void smCom_DeInit(void)
{

    // if (gSmComlock != NULL)
    // {
    //     boatMutexDestroy(&gSmComlock);
    //     // gSmComlock = NULL;
    // }
    /////if (gSmComlock != 0)
    {
        boatMutexDestroy(&gSmComlock);
        /////gSmComlock = 0;
    }
    pSmCom_Transceive = NULL;
    pSmCom_TransceiveRaw = NULL;
}

/**
 * Exchanges APDU without interpreting the message exchanged
 *
 * @param[in,out] pApdu        apdu_t datastructure
 *
 * @retval ::SMCOM_OK          Operation successful
 * @retval ::SMCOM_SND_FAILED  Send Failed
 * @retval ::SMCOM_RCV_FAILED  Receive Failed
 */
U32 smCom_Transceive(void *conn_ctx, apdu_t *pApdu)
{
    U32 ret = SMCOM_NO_PRIOR_INIT;
    if (pSmCom_Transceive != NULL)
    {
        LOCK_TXN();
        ret = pSmCom_Transceive(conn_ctx, pApdu);
        UNLOCK_TXN();
    }
    return ret;
}

/**
 * Exchanges APDU without interpreting the message exchanged
 *
 * @param[in] pTx          Command to be sent to secure module
 * @param[in] txLen        Length of command to be sent
 * @param[in,out] pRx      IN: Buffer to contain response; OUT: Response received from secure module
 * @param[in,out] pRxLen   IN: [TBD]; OUT: Length of response received
 *
 * @retval ::SMCOM_OK          Operation successful
 * @retval ::SMCOM_SND_FAILED  Send Failed
 * @retval ::SMCOM_RCV_FAILED  Receive Failed
 */
U32 smCom_TransceiveRaw(void *conn_ctx, U8 *pTx, U16 txLen, U8 *pRx, U32 *pRxLen)
{
    U32 ret = SMCOM_NO_PRIOR_INIT;
    if (pSmCom_TransceiveRaw != NULL)
    {
        LOCK_TXN();
        ret = pSmCom_TransceiveRaw(conn_ctx, pTx, txLen, pRx, pRxLen);
        UNLOCK_TXN();
    }
    return ret;
}

#if defined(SMCOM_JRCP_V2)
void smCom_Echo(void *conn_ctx, const char *comp, const char *level, const char *buffer)
{
#if USE_LOCK
    /* If this function is called before smcom init
    then Lock fails, return without echo */
    if (pSmCom_TransceiveRaw == NULL)
    {
        return;
    }
#endif
    LOCK_TXN();
    smComJRCP_Echo(conn_ctx, comp, level, buffer);
    UNLOCK_TXN();
}
#endif
