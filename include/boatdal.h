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
#include "boatplatformdal.h"

#ifdef PLATFORM_DAL_UART
//!@brief DAL boat uart object descriptor
typedef struct boatPlatformUART boatUart;

//!@brief DAL boat uart configuration parameters
typedef struct {
//! move out    BUINT8          portNum;	   //!< The DAL uart device port number
    BUINT32         baudrate;	   //!< The baud rate of the DAL uart device,115200\57600\38400\9600...
    BUINT8          databit;	   //!< The data bits of the DAL uart device,5\6\7\8
    BUINT8          stopbit;	   //!< The stop bits of the DAL uart device,1\2
    BUINT8          parity;        //!< The parity check of the DAL uart device,0\1\2 : NONE\ODD\EVEN
    BUINT32         rxBufSize;     //!< rx buffer size , if input 0 then use the default size 256 bytes
    BUINT32         txBufSize;     //!< tx buffer size , if input 0 then use the default size 256 bytes
}boatUartConfig;

/*!*****************************************************************************
@brief Uart receiving call back function type defination

Function: boatUartRxCallback()

     The boatUartRxCallback() transmits the data received by the DAL uart\n
     device to the user who defined this call back function.

@return
    This function returns void.    

@param[in] uartRef
    A boatUart struct pointer,the uartRef is a boat uart descriptor\n
    indicates which uart the received data comes from.

@param[in] data
    The data points to the first address of uart receiving data.

@param[in] len
    The len indicates the number of data bytes received by the \n
    uart device.

*******************************************************************************/
typedef void (*boatUartRxCallback)(boatUart *uartRef,  unsigned char *data, BUINT32 len);

/*!*****************************************************************************
@brief Init a BoAT uart device

Function: boatUartInit()

     The boatUartInit() function open a uart devie and \n
     init the boatUart descriptor.

@return
    This function returns BOAT_SUCCESS if the initialization is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.  

@param[in] uartRef
    A boatUart struct pointer,the uartRef is the boatUart descriptor \n
    used to indicate which uart device is opened.

@param[in] port
    Port is the number of uart device, start at 0

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
BOAT_RESULT boatUartInit(boatUart *uartRef, BUINT8 port, boatUartConfig config,boatUartRxCallback rxCallback);

/*!*****************************************************************************
@brief Sending data through the uart

Function: boatUartWrite()

     The boatUartWrite() sends data to the uart device specified by uartRef.

@return
    This function returns BOAT_SUCCESS if the transmission is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.     

@param[in] uartRef
    A boatUart struct pointer,the uartRef indicates which uart device\n
    will be used to send data.

@param[in] data
    The data points to the first address of the sending data.

@param[in] len
    The len is the length of sending data in bytes.
*******************************************************************************/
BOAT_RESULT boatUartWrite(boatUart *uartRef,unsigned char *data, BUINT32 len);

/*!*****************************************************************************
@brief Deinit a BoAT uart device

Function: boatUartDeinit()

     The boatUartDeinit() function close a uart devie and \n
     deinit the boatUart descriptor.

@return
    This function returns BOAT_SUCCESS if the deinit is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.     

@param[in] uartRef
    A boatUart struct pointer,the uartRef indicates which uart device\n
    will be closed.

*******************************************************************************/
BOAT_RESULT boatUartDeinit(boatUart *uartRef);
#endif//PLATFORM_DAL_UART

//!@brief Abstract definition of VIRTUALAT object based on Linux
#ifdef PLATFORM_DAL_VIRTUALAT

//!@brief DAL boat uart object descriptor
//typedef struct boatPlatformVirtualAT boatVirtualAt;

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
typedef BUINT32 (* boatVirtualAtRxCallback)(char *content,BUINT32 len);


/*!*****************************************************************************
@brief Init a BoAT virtual AT channel

Function: boatVirtualAtOpen()

     The boatVirtualAtOpen() function open/create a Virtual at channel\n
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
BOAT_RESULT boatVirtualAtOpen(boatVirtualAtRxCallback rxCallback);


/*!*****************************************************************************
@brief Sending AT command to Module through virtualAT channel

Function: boatVirtualAtSend()

     The boatVirtualAtSend() sends AT command to Module.

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
BOAT_RESULT boatVirtualAtSend(char *cmd, BUINT16 len);

/*!*****************************************************************************
@brief Sending AT command to Module through virtualAT channel

Function: boatVirtualAtTimedSend()

     The boatVirtualAtTimedSend() sends AT command to Module.

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
BOAT_RESULT boatVirtualAtTimedSend(char *cmd, BUINT16 len, BUINT16 timeout);

/*!*****************************************************************************
@brief Deinit a BoAT virtual AT channel

Function: boatVirtualAtClose()

     The boatVirtualAtClose() function close a virtual AT channel.

@return
    This function returns BOAT_SUCCESS if the closing is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.    

*******************************************************************************/
//! BOAT_RESULT BoAT_virtualAT_deinit(void);
BOAT_RESULT boatVirtualAtClose(void);
#endif

#ifdef PLATFORM_DAL_BLE_SERVICE

//extern void dal_bleService_sig_callback(GAPP_SIGNAL_ID_T sig, va_list arg);

/*!*****************************************************************************
@brief BLE service return message call back function defination

Function: boatBleServiceRxCallback()

    The boatBleServiceRxCallback() return message to BLE client \n     

@return
    The length of message.  

@param[in] buf
    The buf points to the first address of returned message.

@param[in] len
    The len indicates the length of data placed in content.
*******************************************************************************/
typedef BUINT32 (* boatBleServiceRxCallback)(BUINT8 *,BUINT32 );


/*!*****************************************************************************
@brief Init a BoAT BLE service

Function: boatBleServiceInit()

     The boatBleServiceInit() function init a BLE service \n
     used to register callback function,turn on BT,set ble service name, \n
     create a service and characteristic,open the broadcast.

@return
    This function returns BOAT_SUCCESS if the initialization is successful. \n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.  

@param[in] rxCallback
    The rxCallback is the ble service receiving call back function defined by user.
    This function is used to get response message from ble service by ble client.
*******************************************************************************/
BOAT_RESULT boatBleServiceInit(boatBleServiceRxCallback rxCallback);


/*!*****************************************************************************
@brief Sending notification message to ble client by ble service

Function: boatBleServiceSendNOTIFICATION()

     The boatBleServiceSendNOTIFICATION() sends notification message to ble client.

@return
    This function returns BOAT_SUCCESS if the transmission is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.

@param[in] buf
    The cmd points to the first address of the message.

@param[in] len
	The len indicates the number of buf.

*******************************************************************************/
BOAT_RESULT boatBleServiceSendNOTIFICATION(BUINT8 *buf, BUINT16 len);


/*!*****************************************************************************
@brief Sending indication message to ble client by ble service

Function: boatBleServiceSendINDICATION()

     The boatBleServiceSendINDICATION() sends indication message to ble client.

@return
    This function returns BOAT_SUCCESS if the transmission is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.

@param[in] buf
    The cmd points to the first address of the message.

@param[in] len
	The len indicates the number of buf.

*******************************************************************************/
BOAT_RESULT boatBleServiceSendINDICATION(BUINT8 *buf, BUINT16 len);

/*!*****************************************************************************
@brief Deinit a BoAT BLE service

Function: boatBleServiceDeinit()

     The boatBleServiceDeinit() function close a ble service.

@return
    This function returns BOAT_SUCCESS if the closing is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.    

*******************************************************************************/
BOAT_RESULT boatBleServiceDeinit(void);
#endif


#ifdef PLATFORM_DAL_I2C
typedef struct {
	BUINT32 i2cSpeed;	//! 100000 = 100k, 400000 = 400k
	BUINT8  i2cSlaveAddrBits; //! 0: 7 bits, 1: 10 bits
	BUINT8	i2cSlaveDevRegisterAddrLen;	//! 0: 1 byte, 1: 2 bytes, 2: 4 byte
}boatI2cConfig;

typedef struct boatPlatformI2C boatI2c;

BOAT_RESULT boatI2cOpen(boatI2c *i2cRef,BUINT8 i2cPortNum,boatI2cConfig i2cConfig);
BOAT_RESULT boatI2cClose(boatI2c *i2cRef);
BOAT_RESULT boatI2cMasterWrite(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 dataLen);
BOAT_RESULT boatI2cMasterRead(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 dataLen);
				
//! BOAT_RESULT boatI2CMasterSend(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen);
//! BOAT_RESULT boatI2CSlaveReceie(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen);
#endif

#ifdef PLATFORM_DAL_SSL

typedef struct boatPlatformSSL boatSSlCtx;

/**
****************************************************************************************
* @brief:
*  This function connect to address.
* @param[in] *address
*  URL of the network wanted to connect.
* @param[in] *rsvd
*  unused untill now
* @return
*  This function will return socketfd if executed successfully.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BSINT32 BoatConnect(const BCHAR *address, void *rsvd);

/**
****************************************************************************************
* @brief:
* This function initialize TLS connection. This initialization will set hostname and CA cert of
* server's TLS cert or client's TLS cert or key.
* This function will connect to server and complete HandShake process.
* @param[in] *address
* URL of server network
* @param[in] *hostName
* CN of server's TLS cert
* @param[in] caChain
* CA cert of server's TLS cert
* @param[in] clientPrikey
* client's tls prikey
* @param[in] clientCert
* client's tls cert
* @param[out] *socketfd
* socketfd of the connection between client and server
* @param[out] **tlsContext
* output of tls context
* @param[in] *rsvd
* unused untill now
* @return
*  This function will return BOAT_SUCCESS if executed successfully.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT BoatTlsInit(const BCHAR *address, const BCHAR *hostName, const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey, const BoatFieldVariable clientCert, BSINT32 *socketfd, boatSSlCtx **tlsContext, void *rsvd);

/**
****************************************************************************************
* @brief:
* This function send data with length equal to len to server.
* @param[in] sockfd
* socketfd of this connection.
* @param[in] *tlsContext
* tls context between client and server
* @param[in] *buf
* point to the data wanted to send
* @param[in] len
* length of the data wanted to send
* @param[in] *rsvd
* unused untill now
* @return
*  This function will return length of sent data if executed successfully.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BSINT32 BoatSend(BSINT32 sockfd, boatSSlCtx *tlsContext, const BUINT8 *buf, size_t len, void *rsvd);

/**
****************************************************************************************
* @brief:
* This function receive data from server
* @param[in] sockfd
* socketfd of this connection.
* @param[in] *tlsContext
* tls context between client and server
* @param[in] *buf
* point to the data wanted to receive
* @param[in] len
* length of the buf to store receiving data
* @param[in] *rsvd
* unused untill now
* @return
*  This function will return length of received data if executed successfully.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BSINT32 BoatRecv(BSINT32 sockfd, boatSSlCtx *tlsContext, BUINT8 *buf, size_t len, void *rsvd);

/**
****************************************************************************************
* @brief:
* This function close the connection between client and server.
* This function must release the tls context.
* @param[in] sockfd
* socketfd of this connection.
* @param[in] **tlsContext
* tls context between client and server
* @param[in] *rsvd
* unused untill now
* @return
* This function has no returned value.
****************************************************************************************
*/
void BoatClose(BSINT32 sockfd, boatSSlCtx **tlsContext, void *rsvd);

#endif

BOAT_RESULT boat_find_subject_common_name(const BCHAR *cert, const BUINT32 certlen, BCHAR *value, size_t value_length);


#endif
