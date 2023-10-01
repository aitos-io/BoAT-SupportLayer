/**
****************************************************************************************
* @FilePath: boati2c.c
* @Author: aitos
* @Date: 2022-11-21 10:05:50
* @LastEditors:
* @LastEditTime: 2022-11-21 10:05:51
* @Descripttion:
****************************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"
#include "boatdal.h"
#include "boatplatformosal.h"

#include "sc_i2c.h"
#include "simcom_common.h"
#include "simcom_os.h"

/**
****************************************************************************************
* @brief:
*  This function open i2c interface.
*  which i2c device will be opened according to platform, input param do not care which device.
*  Only one i2c device can be opened at a time in the L610 openCPU development environment. 
* @return
*   This function return device handle id if opened successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
//BOAT_RESULT boat_open_i2c(void)
BOAT_RESULT boatI2cOpen(boatI2c *i2cRef,BUINT8 i2cPortNum,boatI2cConfig i2cConfig)
{
	BOAT_RESULT ret = BOAT_ERROR;

	(void) i2cRef,i2cPortNum;

	SC_I2C_DEV i2cDev;

    /* set i2c channel */
    i2cDev.i2c_channel = SC_I2C_CHANNEL0;

	switch (i2cConfig.i2cSpeed)
	{
	case 100000:
		i2cDev.i2c_clock = SC_I2C_STANDARD_MODE;
		sAPI_I2CConfigInit(&i2cDev);
		ret = BOAT_SUCCESS;
		break;
	case 400000:
		i2cDev.i2c_clock = SC_I2C_FAST_MODE;
		sAPI_I2CConfigInit(&i2cDev);
		ret = BOAT_SUCCESS;
		break;
	default:
		break;
	}

    BoatSleepMs(1000);
    return ret;
}

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
//! BOAT_RESULT boat_close_i2c(int fd)
BOAT_RESULT boatI2cClose(boatI2c *i2cRef)
{
	(void) i2cRef;

	return BOAT_SUCCESS;
}

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
* @param[in] dataLen
*  data len
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
//! BOAT_RESULT boat_i2c_send_block(int fd, BUINT8 slaveAddr, BUINT16 regAddr, BUINT8 *data, BUINT32 dataLen)
BOAT_RESULT boatI2cMasterWrite(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 dataLen)
{
	(void) i2cRef,regAddr;

	BUINT8 t_slaveAddr = slaveAddr & 0xff;

	sAPI_I2CWrite(SC_I2C_CHANNEL0,slaveAddr,regAddr,data,dataLen);
    BoatSleepMs(1);

    return BOAT_SUCCESS;
}
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
* @param[in] dataLen
*  data len want to read from slave device
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
//! BOAT_RESULT boat_i2c_read_data(int fd, BUINT8 slaveAddr, BUINT16 regAddr, BUINT8 *data, BUINT32 dataLen)
BOAT_RESULT boatI2cMasterRead(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 dataLen)
{
	(void) i2cRef,regAddr;

	sAPI_I2CRead(SC_I2C_CHANNEL0,slaveAddr,regAddr,data,dataLen);

    return BOAT_SUCCESS;
}


void boatI2CInitI2CIDZero(boatI2c *I2C)
{
//	I2C->i2cId = 0;	///// 230123 modified to -1
}

