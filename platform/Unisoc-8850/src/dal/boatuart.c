/**
****************************************************************************************
* @FilePath: boatuart.c
* @Author: aitos
* @Date: 2023-02-17 16:00:41
* @LastEditors:
* @LastEditTime: 2023-02-17 16:01:03
* @Descripttion: creat boatuart.c
****************************************************************************************
*/
/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/types.h>
#include <unistd.h>
#include <string.h>


#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"
#include "boatdal.h"

#include "drv_names.h"
#include "drv_uart.h"

static void _drvCallback(void *param, uint32_t evt)
{
    atDeviceUart_t *uart = (atDeviceUart_t *)param;
    uart->pending_event |= evt;
    osiWorkEnqueue(uart->work, osiSysWorkQueueHighPriority());
}

BOAT_RESULT boatUartInit(boatUart *uartRef, BUINT8 port, boatUartConfig config, boatUartRxCallback rxCallback)
{
    if((uartRef == NULL) || (rxCallback == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    drvUartCfg_t drvcfg = {0};
    drvcfg.baud = config.baudrate;
    drvcfg.data_bits = config.databit;
    drvcfg.parity = config.parity;
    drvcfg.stop_bits = config.stopbit;
    drvcfg.rx_buf_size = config.rxBufSize;
    drvcfg.tx_buf_size = config.txBufSize;

    drvcfg.auto_baud_lc = 0;
    drvcfg.event_mask = DRV_UART_EVENT_RX_ARRIVED | DRV_UART_EVENT_RX_OVERFLOW | DRV_UART_EVENT_TX_COMPLETE;

    drvcfg.event_cb = _uartCallback;



}


BOAT_RESULT boatUartWrite(boatUart *uartRef, unsigned char *data, BUINT32 len)
{

}


BOAT_RESULT boatUartDeinit(boatUart *uartRef)
{

}