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

/*!@brief The DAL APIs

@file
* boatdal.h define the APIs for abstract object of DAL in BoAT Support Layer,
* and it must include the platform_dal.h of the specific platform.
*/
#ifndef __BOATDAL_H__
#define __BOATDAL_H__

#include <stdio.h>
#include "boatiotsdk.h"

#include "boattypes.h"
#include "boaterrcode.h"
#include "platformDAL.h"

#ifdef PLATFORM_DAL_UART
//!@brief DAL boat uart object descriptor
typedef struct boatPlatformUART boatUART;

//!@brief DAL boat uart configuration parameters
typedef struct {
    int             portNum;	//!< The DAL uart device port number
    int             baudrate;	//!< The baud rate of the DAL uart device,115200\57600\38400\9600...
    unsgined char   databit;	//!< The data bits of the DAL uart device,5\6\7\8
    unsgined char   stopbit;	//!< The stop bits of the DAL uart device,1\2
    unsgined char   parity;	//!< The parity check of the DAL uart device,ODD\EVEN
}baot_uart_config;

/*!*****************************************************************************
@brief Uart receiving call back function type defination

Function: uart_rx_callback()

     The uart_rx_callback() transmits the data received by the DAL uart\n
     device to the user who defined this call back function.

@return
    This function returns void.    

@param[in] UARTRef
    A boat_uart struct pointer,the UARTRef is a boat uart descriptor\n
    indicates which uart the received data comes from.

@param[in] data
    The data points to the first address of uart receiving data.

@param[in] len
    The len indicates the number of data bytes received by the \n
    uart device.

*******************************************************************************/
typedef void (*uart_rx_callback)(boat_uart *UARTRef,  unsigned char *data, BUINT32 len);

/*!*****************************************************************************
@brief Init a BoAT uart device

Function: BoAT_uart_init()

     The BoAT_uart_init() function open a uart devie and \n
     init the boat_uart descriptor.

@return
    This function returns BOAT_SUCCESS if the initialization is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.  

@param[in] UARTRef
    A boat_uart struct pointer,the UARTRef is the boat_uart descriptor \n
    used to indicate which uart device is opened.

@param[in] config
    The config is the configuration parameters of uart device,including:\n
    	uart device number 
    	specific rate 
    	data bit 
    	stop bit
    	check bit

@param[in] rxCallback
    The rxCallback is the uart receiving call back function defined by user.
    This function is used to get data from uart device.

*******************************************************************************/
BOAT_RESULT BoAT_uart_init(boat_uart *UARTRef,BoAT_uart_config_t config,uart_rx_callback rxCallback);

/*!*****************************************************************************
@brief Sending data through the uart

Function: BoAT_uart_write()

     The BoAT_uart_write() sends data to the uart device specified by UARTRef.

@return
    This function returns BOAT_SUCCESS if the transmission is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.     

@param[in] UARTRef
    A boat_uart struct pointer,the UARTRef indicates which uart device\n
    will be used to send data.

@param[in] data
    The data points to the first address of the sending data.

@param[in] len
    The len is the length of sending data in bytes.
*******************************************************************************/
BOAT_RESULT BoAT_uart_write(boat_uart *UARTRef,unsigned char *data, BBUINT32 len);

/*!*****************************************************************************
@brief Deinit a BoAT uart device

Function: BoAT_uart_deinit()

     The BoAT_uart_deinit() function close a uart devie and \n
     deinit the boat_uart descriptor.

@return
    This function returns BOAT_SUCCESS if the deinit is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.     

@param[in] UARTRef
    A boat_uart struct pointer,the UARTRef indicates which uart device\n
    will be closed.

*******************************************************************************/
BOAT_RESULT BoAT_uart_deinit(boat_uart *UARTRef);
#endif

//!@brief Abstract definition of VIRTUALAT object based on Linux
#ifdef PLATFORM_DAL_VIRTUALAT

//!@brief DAL boat uart object descriptor
typedef struct boatPlatformVirtualAT boatVirtualAT;

/*!*****************************************************************************
@brief VirtualAT receieing call back function defination

Function: BoAT_virtualAT_rx_callback()

    The BoAT_virtualAT_rx_callback() receives AT response from virtual AT\n     

@return
    This function has no return value.  

@param[in] content
    The content points to the first address of returned AT response.

@param[in] len
    The len indicates the length of data placed in content.
*******************************************************************************/
typedef void (* boatVirtualATRxCallback)(char *content,BUINT32 len);


/*!*****************************************************************************
@brief Init a BoAT virtual AT channel

Function: boatVirtualATOpen()

     The boatVirtualATOpen() function open/create a Virtual at channel\n
     used to send AT command to Module and receive AT response from Module.

@return
    This function returns BOAT_SUCCESS if the initialization is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.  

@param[in] rxCallback
    The rxCallback is the virtual AT receiving call back function defined by user.
    This function is used to get AT response from Module.
*******************************************************************************/
//! BOAT_RESULT BoAT_virtualAT_init(BoAT_virtualAT_rx_callback fun_cb);
BOAT_RESULT boatVirtualATOpen(boatVirtualATRxCallback rxCallback);


/*!*****************************************************************************
@brief Sending AT command to Module through virtualAT channel

Function: boatVirtualATSend()

     The boatVirtualATSend() sends AT command to Module.

@return
    This function returns BOAT_SUCCESS if the transmission is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.

@param[in] cmd
    The cmd points to the first address of the AT command.

@param[in] len
	The len indicates the number of cmd.

*******************************************************************************/
//! BOAT_RESULT BoAT_virtualAT_send(char *cmd,int timeout);
BOAT_RESULT boatVirtualATSend(char *cmd, BUINT16 len);

/*!*****************************************************************************
@brief Sending AT command to Module through virtualAT channel

Function: boatVirtualATTimedSend()

     The boatVirtualATTimedSend() sends AT command to Module.

@return
    This function returns BOAT_SUCCESS if the transmission is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.

@param[in] cmd
    The cmd points to the first address of the AT command.

@param[in] len
	The len indicates the number of bytes of cmd.

@param[in] timeout
    The timeout indicates the timeout time of AT command response.
    0: invalid , return error
    n: set timeout time to n millisecond

*******************************************************************************/
BOAT_RESULT boatVirtualATTimedSend(char *cmd, BUINT16 len, BUINT16 timeout);

/*!*****************************************************************************
@brief Deinit a BoAT virtual AT channel

Function: baotVirtualATClose()

     The baotVirtualATClose() function close a virtual AT channel.

@return
    This function returns BOAT_SUCCESS if the closing is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.    

*******************************************************************************/
//! BOAT_RESULT BoAT_virtualAT_deinit(void);
BOAT_RESULT baotVirtualATClose(void);

#endif




#ifdef PLATFORM_DAL_I2C
typedef struct {
	BUINT32 I2CSpeed;	//! 100000 = 100k, 400000 = 400k
	BUINT8  I2CSlaveAddrBits; //! 0: 7 bits, 1: 10 bits
	BUINT8	I2CSlaveDevRegisterAddrLen;	//! 0: 1 byte, 1: 2 bytes, 2: 4 byte
}BoATI2CConfig_t;

typedef struct boatPlatformI2C boatI2C;

BOAT_RESULT boatI2COpen(boatI2C *I2CRef,BUINT8 I2CPortNum,BoATI2CConfig_t I2CConfig);
BOAT_RESULT boatI2CClose(boatI2C *I2CRef);
BOAT_RESULT boatI2CMasterWrite(boatI2C *I2CRef, BUINT16 slave_addr, BUINT32 reg_addr, BUINT8 *data, BUINT16 datalen);
BOAT_RESULT boatI2CMasterRead(boatI2C *I2CRef, BUINT16 slave_addr, BUINT32 reg_addr, BUINT8 *data, BUINT16 datalen);
				
//! BOAT_RESULT boatI2CMasterSend(boatI2C *I2CRef, BUINT16 slave_addr, BUINT32 reg_addr, BUINT8 *data, BUINT16 datalen);
//! BOAT_RESULT boatI2CSlaveReceie(boatI2C *I2CRef, BUINT16 slave_addr, BUINT32 reg_addr, BUINT8 *data, BUINT16 datalen);
#endif

#endif
