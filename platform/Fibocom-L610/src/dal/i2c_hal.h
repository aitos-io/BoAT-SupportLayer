/**
****************************************************************************************
* @FilePath: i2c_hal.h
* @Author: aitos
* @Date: 2022-11-21 10:06:16
* @LastEditors:
* @LastEditTime: 2022-11-21 10:06:16
* @Descripttion:
****************************************************************************************
*/
#ifndef _I2CHAL_H_
#define _I2CHAL_H_

#include "boattypes.h"

/**
****************************************************************************************
* @brief:
*  This function open i2c interface.
*  which i2c device will be opened according to platform, input param do not care which device.
* @return
*   This function return device handle id if opened successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
BOAT_RESULT boat_open_i2c(void);

/**
****************************************************************************************
* @brief:
*  This function close i2c interface.
* @param[in] fd
*  device handle id opened.
* @return
*   This function return BOAT_SUCCESS if closed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
BOAT_RESULT boat_close_i2c(int fd);

/**
****************************************************************************************
* @brief:
*  This function send data block to slave device.
* @param[in] fd
*  device handle id opened;
* @param[in] slaveAddr
*  address of slave device;
* @param[in] regAddr
*  address of register
* @param[in] *data
*  data to send out
* @param[in] datalen
*  data len
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
BOAT_RESULT boat_i2c_send_block(int fd, BUINT8 slaveAddr, BUINT16 regAddr, BUINT8 *data, BUINT32 datalen);

/**
****************************************************************************************
* @brief:
*  This function read data from slave device;
* @param[in] fd
*  device handle id opened;
* @param[in] slaveAddr
*  address of slave device;
* @param[in] regAddr
*  address of register
* @param[in] *data
*  data from slave device to store
* @param[in] datalen
*  data len want to read from slave device
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
BOAT_RESULT boat_i2c_read_data(int fd, BUINT8 slaveAddr, BUINT16 regAddr, BUINT8 *data, BUINT32 datalen);

void BoatSleepMs(BUINT32 ms);
void *BoatMalloc(size_t size);
void BoatFree(void *mem_ptr);
#endif