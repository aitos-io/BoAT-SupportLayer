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

#include "mbtk_uart.h"
#include "mbtk_comm_api.h"


#define UART_BUF_LEN_DEFAULT 256
#define UART_PORT_NUMBER_MAX 5
#define UART_DATABITS_MIN 5
#define UART_DATABITS_MAX 8
#define UART_STOPBITS_MIN 1
#define UART_STOPBITS_MAX 2
#define UART_PARITY       2

void (*dalBoatUartRxCallback)(boatUart *uartRef,  unsigned char *data, BUINT32 len);


void LYNQ_uart_cb(MBTK_UART_Port port)
{
    u8 data[UART_BUF_LEN_DEFAULT];
    u16 read_len = 0;
    uint8 status = 0;

    int err = ol_Uart_Control(OL_UART_PORT_STUART, OL_UART_CTL_TX_STATUS, (void *)&status);
    if(OL_UART_RC_OK != err)
    {
        BoatLog(BOAT_LOG_CRITICAL,"uart_demo ol_Uart_Control error %d", err);
        return;
    }
    BoatLog(BOAT_LOG_NORMAL,"uart_demo ol_Uart_Control TX success status = %d", status);

    memset(data, 0x0, sizeof(data));
    ol_Uart_Read(port, data, UART_BUF_LEN_DEFAULT, &read_len);
    if(read_len > 0)
    {
		dalBoatUartRxCallback(NULL,data,read_len);
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL,"ol_Uart_Read NULL");
    }
}



//! function uart init of boat interface
BOAT_RESULT boatUartInit(boatUart *uartRef, BUINT8 port, boatUartConfig config, boatUartRxCallback rxCallback)
{

	OL_UART_DCB uart_dcb = {0};
    int err = 0;

	err = ol_Uart_GetDcb(OL_UART_PORT_STUART, &uart_dcb);
    if(OL_UART_RC_OK != err)
    {
        BoatLog(BOAT_LOG_CRITICAL,"\r\nuart_demo ol_Uart_GetDcb error %d", err);
        return BOAT_ERROR;
    }

	BoatLog(BOAT_LOG_NORMAL,"uart_demo opMode:%d, baudRate:%d, numDataBits:%d, parityBitType:%d,flowControl%d\r\n",
                               uart_dcb.opMode, uart_dcb.baudRate, uart_dcb.numDataBits, uart_dcb.parityBitType, uart_dcb.flowControl);

    uart_dcb.rd_cb = LYNQ_uart_cb;

    ol_Uart_SetDcb(OL_UART_PORT_STUART, &uart_dcb);

	dalBoatUartRxCallback = rxCallback;

    err = ol_Uart_Open(OL_UART_PORT_STUART);

	if(OL_UART_RC_OK == err)
	{
		return BOAT_SUCCESS;
	}
	else
	{
		return BOAT_ERROR;
	}
	
}

BOAT_RESULT boatUartWrite(boatUart *uartRef, unsigned char *data, BUINT32 len)
{
	ol_Uart_Write(OL_UART_PORT_STUART,data,len);
	return BOAT_SUCCESS;
}

BOAT_RESULT boatUartDeinit(boatUart *uartRef)
{
	ol_Uart_Close(OL_UART_PORT_STUART);
	return BOAT_SUCCESS;
}


void boatUartInitUartIdNagtive(boatUart *uart)
{
//	uart->uartPort = -1;
}



