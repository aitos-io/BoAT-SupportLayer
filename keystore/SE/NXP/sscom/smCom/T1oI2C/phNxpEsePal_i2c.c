/*
 * Copyright 2010-2014,2018-2020 NXP
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * DAL i2c port implementation for linux
 *
 * Project: Trusted ESE Linux
 *
 */
#include <stdlib.h>
#include <errno.h>
#include "phNxpEsePal_i2c.h"
#include "phEseStatus.h"
#include <string.h>
#include "boatdal.h"
#include "boatlog.h"
#include "boaterrcode.h"

#ifdef FLOW_VERBOSE
#define NX_LOG_ENABLE_SMCOM_DEBUG 1
#endif

// #include "nxLog_smCom.h"
// #include "sm_timer.h"

// #include "se05x_apis.h"

#include <time.h>

#define MAX_RETRY_CNT 10
boatI2c i2c_fd = {0};

/*******************************************************************************
**
** Function         phPalEse_i2c_close
**
** Description      Closes PN547 device
**
** param[in]        pDevHandle - device handle
**
** Returns          None
**
*******************************************************************************/
void phPalEse_i2c_close(void *pDevHandle)
{
    boatI2cClose(&i2c_fd);
    pDevHandle = NULL;

    return;
}

/*******************************************************************************
**
** Function         phPalEse_i2c_open_and_configure
**
** Description      Open and configure pn547 device
**
** param[in]        pConfig     - hardware information
**
** Returns          ESE status:
**                  ESESTATUS_SUCCESS            - open_and_configure operation success
**                  ESESTATUS_INVALID_DEVICE     - device open operation failure
**
*******************************************************************************/
ESESTATUS phPalEse_i2c_open_and_configure(pphPalEse_Config_t pConfig)
{
    BOAT_RESULT ret = BOAT_SUCCESS;
    boatI2cConfig I2CConfig;
    I2CConfig.i2cSpeed = 100000;
    I2CConfig.i2cSlaveAddrBits = 0;
    I2CConfig.i2cSlaveDevRegisterAddrLen = 1;
    // void *conn_ctx = NULL;
    // i2c_fd = boat_open_i2c();
    ret = boatI2cOpen(&i2c_fd, 2, I2CConfig);
    if (ret != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, " open I2C device fail %d", i2c_fd);
        return ESESTATUS_INVALID_DEVICE;
    }
    pConfig->pDevHandle = NULL;
    return ESESTATUS_SUCCESS;
}

/*******************************************************************************
**
** Function         phPalEse_i2c_read
**
** Description      Reads requested number of bytes from pn547 device into given buffer
**
** param[in]       pDevHandle       - valid device handle
** param[in]       pBuffer          - buffer for read data
** param[in]       nNbBytesToRead   - number of bytes requested to be read
**
** Returns          numRead   - number of successfully read bytes
**                  -1        - read operation failure
**
*******************************************************************************/
int phPalEse_i2c_read(void *pDevHandle, uint8_t *pBuffer, int nNbBytesToRead)
{
    BOAT_RESULT ret = BOAT_SUCCESS;
    int retryCount = 0;
    int numRead = 0;
    BoatLog(BOAT_LOG_NORMAL, "%s Read Requested %d bytes ", __FUNCTION__, nNbBytesToRead);
    // sm_sleep(ESE_POLL_DELAY_MS);
    while (numRead != nNbBytesToRead)
    {
        ret = boatI2cMasterRead(&i2c_fd, SMCOM_I2C_ADDRESS, I2C_BUS_0, pBuffer, nNbBytesToRead);
        if (ret != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "_i2c_read() error : %d ", ret);
            if ((ret == I2C_NACK_ON_ADDRESS) && (retryCount < MAX_RETRY_COUNT))
            {
                retryCount++;
                /* 1ms delay to give ESE polling delay */
                /*i2c driver back off delay is providing 1ms wait time so ignoring waiting time at this level*/
                // sm_sleep(ESE_POLL_DELAY_MS);
                BoatLog(BOAT_LOG_NORMAL, "_i2c_read() failed. Going to retry, counter:%d  !", retryCount);
                continue;
            }
            return -1;
        }
        else
        {
            numRead = nNbBytesToRead;
            break;
        }
    }
    return numRead;
}

/*******************************************************************************
**
** Function         phPalEse_i2c_write
**
** Description      Writes requested number of bytes from given buffer into pn547 device
**
** param[in]       pDevHandle       - valid device handle
** param[in]       pBuffer          - buffer for read data
** param[in]       nNbBytesToWrite  - number of bytes requested to be written
**
** Returns          numWrote   - number of successfully written bytes
**                  -1         - write operation failure
**
*******************************************************************************/
int phPalEse_i2c_write(void *pDevHandle, uint8_t *pBuffer, int nNbBytesToWrite)
{
    BOAT_RESULT ret = BOAT_SUCCESS;
    int retryCount = 0;
    int numWrote = 0;
    pBuffer[0] = 0x5A; // Recovery if stack forgot to add NAD byte.
    do
    {
        /* 1ms delay to give ESE polling delay */
        BoatSleepMs(ESE_POLL_DELAY_MS);
        ret = boatI2cMasterWrite(&i2c_fd, SMCOM_I2C_ADDRESS, I2C_BUS_0, pBuffer, nNbBytesToWrite);
        if (ret != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "_i2c_write() error : %d ", ret);
            if (retryCount < MAX_RETRY_COUNT)
            {
                retryCount++;
                /* 1ms delay to give ESE polling delay */
                /*i2c driver back off delay is providing 1ms wait time so ignoring waiting time at this level*/
                // sm_sleep(ESE_POLL_DELAY_MS);
                BoatLog(BOAT_LOG_NORMAL, "_i2c_write() failed. Going to retry, counter:%d  !", retryCount);
                continue;
            }
            return -1;
        }
        else
        {

            numWrote = nNbBytesToWrite;
            // sm_sleep(ESE_POLL_DELAY_MS);
            BoatLog(BOAT_LOG_NORMAL, "_i2c_write() OK : %d ", numWrote);
            break;
        }
    } while (ret != BOAT_SUCCESS);
    return numWrote;
}
