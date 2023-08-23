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

#include "hal_gpio.h"
#include "hal_i2c.h"
#include "driver_demo_config.h"


static void i2c_master_poll_init(HAL_I2C_HandleTypeDef *i2cHandle,BUINT32 speed,BUINT8 addrBits)
{
	HAL_GPIO_InitTypeDef i2c_gpio_init = {0};

	//gpio初始化
	i2c_gpio_init.Pin = GPIO_PIN35;
	i2c_gpio_init.Mode = GPIO_MODE_PER;
	i2c_gpio_init.Remap = GPIO_I2C0_SCL;
	HAL_GPIO_Init(&i2c_gpio_init);

	i2c_gpio_init.Pin = GPIO_PIN36;
	i2c_gpio_init.Mode = GPIO_MODE_PER;
	i2c_gpio_init.Remap = GPIO_I2C0_SDA;
	HAL_GPIO_Init(&i2c_gpio_init);

	//初始化I2C
	i2cHandle->Instance = I2C0;
	i2cHandle->Init.WorkMode = I2C_WORKMODE_MASTER;
	switch (addrBits)
	{
	case 0:
		i2cHandle->Init.AddrMode = I2C_ADDRMODE_7BITS;
		break;
	case 1:
		i2cHandle->Init.AddrMode = I2C_ADDRMODE_10BITS;
		break;
	default:
		break;
	}

	switch (speed)
	{
	case 100000:
		i2cHandle->Init.Speed = I2C_SPEED_100K;
		break;
	case 400000:
		i2cHandle->Init.Speed = I2C_SPEED_400K;
		break;
	default:
		break;
	}
	
	HAL_I2C_Init(i2cHandle);
}

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
	Clktick_Init();
	i2c_master_poll_init(&(i2cRef->i2cHandle),i2cConfig.i2cSpeed,i2cConfig.i2cSlaveAddrBits);

    BoatSleepMs(1000);
    return BOAT_SUCCESS;
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
	HAL_I2C_DeInit(&(i2cRef->i2cHandle));
	memset(&(i2cRef->i2cHandle),0U,sizeof(HAL_I2C_HandleTypeDef));

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
	(void)regAddr;

	HAL_I2C_Master_Transmit(&(i2cRef->i2cHandle),slaveAddr,data,dataLen,500);
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
	(void)regAddr;

	HAL_I2C_Master_Receive(&(i2cRef->i2cHandle),slaveAddr,data,dataLen,500);

    return BOAT_SUCCESS;
}


void boatI2CInitI2CIDZero(boatI2c *I2C)
{
//	I2C->i2cId = 0;	///// 230123 modified to -1
}

