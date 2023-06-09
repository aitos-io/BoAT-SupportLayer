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

#include "fibo_opencpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"
#include "boatdal.h"


#define UART_BUF_LEN_DEFAULT 256
#define UART_PORT_NUMBER_MAX 5
#define UART_DATABITS_MIN 5
#define UART_DATABITS_MAX 8
#define UART_STOPBITS_MIN 1
#define UART_STOPBITS_MAX 2
#define UART_PARITY       2

#define FIBO_UART_MAX 5

static struct boatFibocomUartCallbackInfo
{
	hal_uart_port_t port;
	boatUart *uartRef;		//! note it is a pointer.
	boatUartRxCallback callback;
}boatFiboUartRxCbInfo[FIBO_UART_MAX];

BOAT_RESULT boatUartCbRegister(hal_uart_port_t port, boatUart *uartRef, boatUartRxCallback rxCallback)
{
	if(uartRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart device uartRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	if(rxCallback == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart device rxCallback is NULL!\r\n");
		return BOAT_ERROR;
	}
	//! shared resource, need lock!
	for(int i=0; i< FIBO_UART_MAX; i++)
	{
		if((boatFiboUartRxCbInfo[i].callback == NULL) && (boatFiboUartRxCbInfo[i].uartRef == NULL))
		{
			boatFiboUartRxCbInfo[i].port = port;
			boatFiboUartRxCbInfo[i].uartRef = uartRef;
			boatFiboUartRxCbInfo[i].callback = rxCallback;
			BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart rxCallback regitster succ!\r\n");
			return BOAT_SUCCESS;
		}
	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart rxCallback regitster full!\r\n");
	return BOAT_ERROR;
}

BOAT_RESULT boatUartCbDeregister(hal_uart_port_t port, boatUart *uartRef)
{
	if(uartRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart device deregister uartRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	if(port < 0)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart device deregister rxCallback is NULL!\r\n");
		return BOAT_ERROR;
	}

	//! shared resource, need lock!	
	for(int i=0; i< FIBO_UART_MAX; i++)
	{
		if((boatFiboUartRxCbInfo[i].port == port) && (boatFiboUartRxCbInfo[i].uartRef == uartRef))
		{
			boatFiboUartRxCbInfo[i].port = 0;
			boatFiboUartRxCbInfo[i].uartRef = NULL;
			boatFiboUartRxCbInfo[i].callback = NULL;
			BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart rxCallback deregitster succ, port[%d]!\r\n", port);
			return BOAT_SUCCESS;
		}
	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart rxCallback deregitster not fount the port[%d]!\r\n", port);
	return BOAT_ERROR;
}

void fiboUartCommCallback(hal_uart_port_t port, UINT8 *data, UINT16 len, void *arg)
{
	if(port < 0)	//! typedef int hal_uart_port_t
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart callback port number invalid\r\n");
		return;
	}
	if(data == 0)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart callback *data invalid\r\n");
		return;
	}
	for(int i = 0; i < FIBO_UART_MAX; i++)
	{
		if( port == boatFiboUartRxCbInfo[i].port)
		{
			if((boatFiboUartRxCbInfo[i].callback != NULL) && (boatFiboUartRxCbInfo[i].uartRef != NULL))
			{
				boatFiboUartRxCbInfo[i].callback(boatFiboUartRxCbInfo[i].uartRef,data,len);
			}
			return;
		}
	}
}

//! function uart init of boat interface
BOAT_RESULT boatUartInit(boatUart *uartRef, BUINT8 port, boatUartConfig config, boatUartRxCallback rxCallback)
{
	BSINT32 ret;
	//! uartRef check
	if(uartRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart device uartRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	//! config check
	//! fibocom port is int, BUINT8 never make any mistake
	/////if(port < 0)	//! > UART_PORT_NUMBER_MAX)	
	/////{
	/////	BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart port is invalid!\r\n");
	/////	return BOAT_ERROR;
	/////}
	//! baudrate : Not be limited, it can be any value. 0 set as auto baud
	/////if(config.baudrate)	
	/////{
	/////	BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart config.baudrate is[%d]!\r\n", config.baudrate);
		/////return BOAT_ERROR;
	/////}
	//! stopbit : 1/2
	if((config.stopbit > UART_STOPBITS_MAX)	|| (config.stopbit < UART_STOPBITS_MIN)) 
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart config.stopbit is invalid![%d]\r\n",config.stopbit);
		return BOAT_ERROR;
	}
	//! databit : 5\6\7\8
	if((config.databit < UART_DATABITS_MIN)	 || (config.databit > UART_DATABITS_MAX))
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart config.databit is invalid![%d]\r\n",config.databit);
		return BOAT_ERROR;
	}
 	//! parity 	 0: No parity check, 1: Parity check is odd, 2: Parity check is even
	if(config.parity > UART_PARITY)	
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart config.parity is invalid![%d]\r\n",config.parity);
		return BOAT_ERROR;
	}
	//! rxBufSize; 
	if(config.rxBufSize == 0)	
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart config.rxBufSize equal 0, set as default[%d] \r\n",UART_BUF_LEN_DEFAULT);
		config.rxBufSize = UART_BUF_LEN_DEFAULT;
	}

	//! txBufSize 
	if(config.txBufSize == 0)	
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart config.txBufSize equal 0, set as default[%d] \r\n",UART_BUF_LEN_DEFAULT);
		config.txBufSize = UART_BUF_LEN_DEFAULT;
	}

	//! rxCallback
	if(rxCallback == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart device rxCallback is NULL!\r\n");
		return BOAT_ERROR;
	}

	hal_uart_config_t uartConfig;

	uartConfig.baud         = config.baudrate;
    uartConfig.data_bits    = config.databit;
    uartConfig.stop_bits    = config.stopbit;
    uartConfig.parity       = config.parity;
    uartConfig.rx_buf_size  = config.rxBufSize;
    uartConfig.tx_buf_size  = config.txBufSize;
    uartConfig.recv_timeout = 100;               ///// copy from cbbATRelayer, need debug
    uartConfig.cts_enable   = FALSE;             ///// copy from cbbATRelayer, need debug
    uartConfig.rts_enable   = FALSE;             ///// copy from cbbATRelayer, need debug

	ret = boatUartCbRegister(port, uartRef, rxCallback);
	if(ret == BOAT_ERROR)
	{
        BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart regitster callback full, port[%d].",port);
		return BOAT_ERROR;
	}

	/////if (fibo_hal_uart_init(UART1, &uartConfig, uart1_recv_cb, NULL) < 0)
	ret = fibo_hal_uart_init(port, &uartConfig, fiboUartCommCallback, NULL);
	
	if (ret < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 Initialize the uart[%d] failed.", port);
		ret = boatUartCbDeregister(port, uartRef);
		
		return BOAT_ERROR;
    }

	uartRef->uartPort = port;
	
	return BOAT_SUCCESS;
}

BOAT_RESULT boatUartWrite(boatUart *uartRef, unsigned char *data, BUINT32 len)
{
	BSINT32 ret = 0;
	//! uartRef check
	if(uartRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart write uartRef is NULL!\r\n");
		return BOAT_ERROR;
	}

	if(uartRef->uartPort < 0)	//! > UART_PORT_NUMBER_MAX)	
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart port is invalid][%d]\r\n",uartRef->uartPort);
		return BOAT_ERROR;
	}

	//! data check
	if(data == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart write data is NULL!\r\n");
		return BOAT_ERROR;
	}

	//! len check
	if(len == 0)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart write len equal 0, return succ!\r\n");
		return BOAT_SUCCESS;
	}

	ret = fibo_hal_uart_put(uartRef->uartPort, data, len);
	if (ret < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610uart[%d] put(write) failed.", uartRef->uartPort);
		
		return BOAT_ERROR;
    }
	return BOAT_SUCCESS;
}

BOAT_RESULT boatUartDeinit(boatUart *uartRef)
{
	BSINT32 ret = 0;
	//! uartRef check
	if(uartRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart write uartRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	if(uartRef->uartPort < 0)	//! > UART_PORT_NUMBER_MAX)	
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610 uart port is invalid][%d]\r\n",uartRef->uartPort);
		return BOAT_ERROR;
	}
	
	ret = fibo_hal_uart_deinit(uartRef->uartPort);
	if (ret < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][uart] L610uart[%d] deinit failed.", uartRef->uartPort);
    }
	ret = boatUartCbDeregister(uartRef->uartPort, uartRef);
	return BOAT_SUCCESS;
}


void boatUartInitUartIdNagtive(boatUart *uart)
{
	uart->uartPort = -1;
}



