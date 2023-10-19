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

static drvUart_t *g_uart_dev = NULL;
static BUINT8 g_rcv_buf[1024]={0};
static boatUart *g_boat_uart_dev = NULL;

static void (*boatUartRecvCallback)(boatUart *uartRef,  unsigned char *data, BUINT32 len);

static void _uartCallback(void *param, uint32_t evt)
{
    if(evt & DRV_UART_EVENT_RX_ARRIVED)
    {
        if(g_uart_dev != NULL)
        {
            int len = drvUartReceive(g_uart_dev,g_rcv_buf,1024);
            if(len > 0)
            {
                boatUartRecvCallback(g_boat_uart_dev,g_rcv_buf,len);
            }
        }
        
    }
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

    boatUartRecvCallback = rxCallback;

    drvUart_t *uart = drvUartCreate(port,&drvcfg);
    if(uart == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"drvUartCreate failed!");
        return BOAT_ERROR;
    }


    bool ret = drvUartOpen(uart);
    if(ret == false)
    {
        BoatLog(BOAT_LOG_CRITICAL,"drvUartOpen failed!");
        return BOAT_ERROR;
    }

    uartRef->uartId = uart;
    g_uart_dev = uart;
    g_boat_uart_dev = uartRef;

    return BOAT_SUCCESS;

}


BOAT_RESULT boatUartWrite(boatUart *uartRef, unsigned char *data, BUINT32 len)
{
    if((uartRef == NULL) || (data == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL,"bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(uartRef->uartId == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Wrong uart id!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    int send_len = drvUartSend(uartRef->uartId,data,len);
    if(send_len != len)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Uart write failed!");
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}


BOAT_RESULT boatUartDeinit(boatUart *uartRef)
{
    if(uartRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(uartRef->uartId == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Wrong uart id!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    drvUartClose(uartRef->uartId);

    return BOAT_SUCCESS;
}