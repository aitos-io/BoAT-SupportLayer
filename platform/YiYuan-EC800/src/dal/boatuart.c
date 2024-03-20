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
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"
#include "boatdal.h"

#include "ql_uart.h"

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
	QL_UART_PORT_NUMBER_E port;
	boatUart *uartRef;		//! note it is a pointer.
	boatUartRxCallback callback;
}boatUartRxCbInfo[FIBO_UART_MAX];

BOAT_RESULT boatUartCbRegister(QL_UART_PORT_NUMBER_E port, boatUart *uartRef, boatUartRxCallback rxCallback)
{
	if(uartRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart device uartRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	if(rxCallback == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart device rxCallback is NULL!\r\n");
		return BOAT_ERROR;
	}
	//! shared resource, need lock!
	for(int i=0; i< FIBO_UART_MAX; i++)
	{
		if((boatUartRxCbInfo[i].callback == NULL) && (boatUartRxCbInfo[i].uartRef == NULL))
		{
			boatUartRxCbInfo[i].port = port;
			boatUartRxCbInfo[i].uartRef = uartRef;
			boatUartRxCbInfo[i].callback = rxCallback;
			BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart rxCallback regitster succ!\r\n");
			return BOAT_SUCCESS;
		}
	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart rxCallback regitster full!\r\n");
	return BOAT_ERROR;
}

BOAT_RESULT boatUartCbDeregister(QL_UART_PORT_NUMBER_E port, boatUart *uartRef)
{
	if(uartRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart device deregister uartRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	if(port < 0)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart device deregister rxCallback is NULL!\r\n");
		return BOAT_ERROR;
	}

	//! shared resource, need lock!	
	for(int i=0; i< FIBO_UART_MAX; i++)
	{
		if((boatUartRxCbInfo[i].port == port) && (boatUartRxCbInfo[i].uartRef == uartRef))
		{
			boatUartRxCbInfo[i].port = 0;
			boatUartRxCbInfo[i].uartRef = NULL;
			boatUartRxCbInfo[i].callback = NULL;
			BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart rxCallback deregitster succ, port[%d]!\r\n", port);
			return BOAT_SUCCESS;
		}
	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart rxCallback deregitster not fount the port[%d]!\r\n", port);
	return BOAT_ERROR;
}

void fiboUartCommCallback(QL_UART_PORT_NUMBER_E port, void *para)
{
	if((port < 0) || (port >=  QL_MAX_UART_PORT))	//! typedef int hal_uart_port_t
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart callback port number invalid\r\n");
		return;
	}
	if(NULL == para)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart callback *data invalid\r\n");
		return;
	}
	for(int i = 0; i < FIBO_UART_MAX; i++)
	{
		if( port == boatUartRxCbInfo[i].port)
		{
			if((boatUartRxCbInfo[i].callback != NULL) && (boatUartRxCbInfo[i].uartRef != NULL))
			{
				BUINT32* plen = (UINT32 *)para;
				BUINT8 *data = malloc(*plen);// malloc data 130, malloc here, free it in taskuart rd queue
				if(data == NULL)
				{
					return; // no memory
				}
				BUINT32 len = ql_uart_read(port, (unsigned char *)data, *plen);

				boatUartRxCbInfo[i].callback(boatUartRxCbInfo[i].uartRef, data, len);
				free(data); // free data 130
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
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart device uartRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	//! stopbit : 1/2
	if((config.stopbit > UART_STOPBITS_MAX)	|| (config.stopbit < UART_STOPBITS_MIN)) 
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart config.stopbit is invalid![%d]\r\n",config.stopbit);
		return BOAT_ERROR;
	}
	//! databit : 5\6\7\8
	if((config.databit < UART_DATABITS_MIN)	 || (config.databit > UART_DATABITS_MAX))
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart config.databit is invalid![%d]\r\n",config.databit);
		return BOAT_ERROR;
	}
 	//! parity 	 0: No parity check, 1: Parity check is odd, 2: Parity check is even
	if(config.parity > UART_PARITY)	
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart config.parity is invalid![%d]\r\n",config.parity);
		return BOAT_ERROR;
	}
	//! rxCallback
	if(rxCallback == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart device rxCallback is NULL!\r\n");
		return BOAT_ERROR;
	}

	ret = boatUartCbRegister(port, uartRef, rxCallback);
	if(ret == BOAT_ERROR)
	{
        BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart regitster callback full, port[%d].",port);
		return BOAT_ERROR;
	}

	///ret = fibo_hal_uart_init(port, &dcb, fiboUartCommCallback, NULL);
    ret = ql_uart_open(port, config.baudrate, QL_FC_NONE);
	if (ret < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 Initialize the uart[%d] failed.", port);
		ret = boatUartCbDeregister(port, uartRef);
		
		return BOAT_ERROR;
    }
	
	ql_uart_config_t dcb;
	ql_uart_get_dcbconfig(QL_MAIN_UART_PORT, &dcb);
	dcb.baudrate    = config.baudrate;
    dcb.data_bit    = config.databit;
    dcb.stop_bit    = config.stopbit;
    dcb.parity_bit  = config.parity;

	ql_uart_set_dcbconfig(QL_MAIN_UART_PORT, &dcb);
	
    ql_uart_register_cb(QL_MAIN_UART_PORT, fiboUartCommCallback);	//use callback to read uart data

	uartRef->uartPort = port;
	
	return BOAT_SUCCESS;
}

BOAT_RESULT boatUartWrite(boatUart *uartRef, unsigned char *data, BUINT32 len)
{
	BSINT32 ret = 0;
	//! uartRef check
	if(uartRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart write uartRef is NULL!\r\n");
		return BOAT_ERROR;
	}

	if(uartRef->uartPort < 0)	//! > UART_PORT_NUMBER_MAX)	
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart port is invalid][%d]\r\n",uartRef->uartPort);
		return BOAT_ERROR;
	}

	//! data check
	if(data == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart write data is NULL!\r\n");
		return BOAT_ERROR;
	}

	//! len check
	if(len == 0)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart write len equal 0, return succ!\r\n");
		return BOAT_SUCCESS;
	}

	ret = ql_uart_write(uartRef->uartPort, data, len);
	if (ret < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800uart[%d] put(write) failed.", uartRef->uartPort);
		
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
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart write uartRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	if(uartRef->uartPort < 0)	//! > UART_PORT_NUMBER_MAX)	
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800 uart port is invalid][%d]\r\n",uartRef->uartPort);
		return BOAT_ERROR;
	}
	
	ret = ql_uart_close(uartRef->uartPort);
	if (ret < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][uart] EC800uart[%d] deinit failed.", uartRef->uartPort);
    }
	ret = boatUartCbDeregister(uartRef->uartPort, uartRef);
	return BOAT_SUCCESS;
}
