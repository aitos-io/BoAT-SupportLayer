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


#include "hal_gpio.h"
#include "hal_uart.h"

#define UART_BUF_LEN_DEFAULT 256
#define UART_PORT_NUMBER_MAX 5
#define UART_DATABITS_MIN 5
#define UART_DATABITS_MAX 8
#define UART_STOPBITS_MIN 1
#define UART_STOPBITS_MAX 2
#define UART_PARITY       2

void (*dalBoatUartRxCallback)(boatUart *uartRef,  unsigned char *data, BUINT32 len);


void uart_get_msg_task(void *uartHandle)
{
    boatUart *tmpUart = (boatUart *)uartHandle;
    #define MAX_DATA_LEN (64) 
    uint8_t data[MAX_DATA_LEN] = {0x00};

    while (1)
    {
        memset(data,0U,MAX_DATA_LEN);
        HAL_StatusTypeDef ret = HAL_UART_Receive(&(tmpUart->uartPort), data, MAX_DATA_LEN, 500);
        if(ret == HAL_OK)
        {
            dalBoatUartRxCallback(NULL,data,MAX_DATA_LEN);
        }
        else if(ret == HAL_TIMEOUT)
        {
            dalBoatUartRxCallback(NULL,data,tmpUart->uartPort.RxXferCount);
        }
        else
        {
            continue;
        }
    }
    
}

//! function uart init of boat interface
BOAT_RESULT boatUartInit(boatUart *uartRef, BUINT8 port, boatUartConfig config, boatUartRxCallback rxCallback)
{
    HAL_GPIO_InitTypeDef uart_gpio_init = {0};

	//uart0 gpio初始化
	uart_gpio_init.Pin = GPIO_PIN22;
	uart_gpio_init.Mode = GPIO_MODE_PER;
	uart_gpio_init.Remap = GPIO_UART_RXD;
	HAL_GPIO_Init(&uart_gpio_init);

	uart_gpio_init.Pin = GPIO_PIN23;
	uart_gpio_init.Mode = GPIO_MODE_PER;
	uart_gpio_init.Remap = GPIO_UART_TXD;
	HAL_GPIO_Init(&uart_gpio_init);

	uart_gpio_init.Pin = GPIO_PIN21;
	uart_gpio_init.Mode = GPIO_MODE_PER;
	uart_gpio_init.Remap = GPIO_UART_CTS;
	HAL_GPIO_Init(&uart_gpio_init);

	uart_gpio_init.Pin = GPIO_PIN20;
	uart_gpio_init.Mode = GPIO_MODE_PER;
	uart_gpio_init.Remap = GPIO_UART_RTS;
	HAL_GPIO_Init(&uart_gpio_init);

	//uart0 初始化
	uartRef->uartPort.Instance = UART0;
    switch (config.baudrate)
    {
    case 115200:
        uartRef->uartPort.Init.BaudRate = UART_BAUD_115200;
        break;
    case 57600:
        uartRef->uartPort.Init.BaudRate = UART_BAUD_57600;
        break;
    case 38400:
        uartRef->uartPort.Init.BaudRate = UART_BAUD_38400;
        break;
    case 9600:
        uartRef->uartPort.Init.BaudRate = UART_BAUD_9600;
        break;
    default:
        break;
    }
	
	uartRef->uartPort.Init.WordLength = UART_WORDLENGTH_8;
	uartRef->uartPort.Init.Parity = UART_PARITY_NONE;
	HAL_UART_Init(&(uartRef->uartPort));

    dalBoatUartRxCallback = rxCallback;

    //start a task to get uart msg
    osThreadAttr_t thread_attr = {0};
	thread_attr.name = "uart_get_msg_task";
	thread_attr.priority = osPriorityNormal;
	thread_attr.stack_size = 0x1000;
	osThreadId_t  uart_poll_TskHandle = osThreadNew((osThreadFunc_t)uart_get_msg_task, uartRef, &thread_attr);

    return BOAT_SUCCESS;
}

BOAT_RESULT boatUartWrite(boatUart *uartRef, unsigned char *data, BUINT32 len)
{
	HAL_UART_Transmit(&(uartRef->uartPort),data,len,500);
	return BOAT_SUCCESS;
}

BOAT_RESULT boatUartDeinit(boatUart *uartRef)
{
	HAL_UART_DeInit(&(uartRef->uartPort));
	return BOAT_SUCCESS;
}


void boatUartInitUartIdNagtive(boatUart *uart)
{
	uart->uartPort = {0};
}



