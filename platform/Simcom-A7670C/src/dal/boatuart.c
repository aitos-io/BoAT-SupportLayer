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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"
#include "boatdal.h"
#include "simcom_uart.h"
#include "simcom_common.h"
#include "simcom_system.h"


#define UART_BUF_LEN_DEFAULT 256
#define UART_PORT_NUMBER_MAX 5
#define UART_DATABITS_MIN 5
#define UART_DATABITS_MAX 8
#define UART_STOPBITS_MIN 1
#define UART_STOPBITS_MAX 2
#define UART_PARITY       2

void (*dalBoatUartRxCallback)(boatUart *uartRef,  unsigned char *data, BUINT32 len);

static void SC_boat_Uart_Callback(SC_Uart_Port_Number port, void *para)
{
    int readLen = 0;
    uint8_t tmp[512] ={0};
    readLen = sAPI_UartRead(port, tmp, sizeof(tmp));
    dalBoatUartRxCallback(NULL,tmp,readLen);
}


//! function uart init of boat interface
BOAT_RESULT boatUartInit(boatUart *uartRef, BUINT8 port, boatUartConfig config, boatUartRxCallback rxCallback)
{
    SC_Uart_Return_Code result = -1;

    dalBoatUartRxCallback = rxCallback;

    SCuartConfiguration nativeConf = {0};

    nativeConf.BaudRate = config.baudrate;
    switch (config.databit)
    {
    case 5:
        nativeConf.DataBits = SC_UART_WORD_LEN_5;
        break;
    case 6:
         nativeConf.DataBits = SC_UART_WORD_LEN_6;
        break;
    case 7:
        nativeConf.DataBits = SC_UART_WORD_LEN_7;
        break;
    case 8:
        nativeConf.DataBits = SC_UART_WORD_LEN_8;
        break;
    default:
        break;
    }
    
    switch (config.parity)
    {
    case 0:
        nativeConf.ParityBit = SC_UART_NO_PARITY_BITS;
        break;
    case 1:
        nativeConf.ParityBit = SC_UART_ODD_PARITY_SELECT;
        break;
    case 2:
        nativeConf.ParityBit = SC_UART_EVEN_PARITY_SELECT;
        break;
    default:
        break;
    }

    switch (config.stopbit)
    {
    case 1:
        nativeConf.StopBits = SC_UART_ONE_STOP_BIT;
        break;
    case 2:
        nativeConf.StopBits = SC_UART_ONE_HALF_OR_TWO_STOP_BITS;
        break;
    default:
        break;
    }

    result = sAPI_UartSetConfig(port,&nativeConf);
    if(result == SC_UART_RETURN_CODE_OK)
    {
        uartRef->uartPort = port;
    }
    else
    {
        uartRef->uartPort = -1;
        return BOAT_ERROR;
    }
    
    result = sAPI_UartControl(port,SC_UART_OPEN);
    if(result != SC_UART_RETURN_CODE_OK)
    {
        uartRef->uartPort = -1;
        return BOAT_ERROR;
    }

    result = sAPI_UartRegisterCallback(port,SC_boat_Uart_Callback);
    if(result != SC_UART_RETURN_CODE_OK)
    {
        uartRef->uartPort = -1;
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}

BOAT_RESULT boatUartWrite(boatUart *uartRef, unsigned char *data, BUINT32 len)
{
	sAPI_UartWrite(uartRef->uartPort,data,len);
	return BOAT_SUCCESS;
}

BOAT_RESULT boatUartDeinit(boatUart *uartRef)
{
	sAPI_UartControl(uartRef->uartPort,SC_UART_CLOSE);
	return BOAT_SUCCESS;
}


void boatUartInitUartIdNagtive(boatUart *uart)
{
	uart->uartPort = -1;
}



