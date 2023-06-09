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
#include "fibo_opencpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"
//! #include "i2c_hal.h"
#include "boatdal.h"
#include "boatplatformosal.h"

//! static HAL_I2C_BPS_T bsp1_1 = {2, 0};
// drvI2cSlave_t drv_i2c = {0x48, 0, 0, false, false};
// i2c_Handle i2c_handle = 0;

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
    int rtnVal = 0;
    i2c_Handle i2c_handle = 0;
	static HAL_I2C_BPS_T bsp1_1; //! = {2, 0};
	//! L610 I2C port number 1-2
	if((i2cPortNum < 1) || (i2cPortNum > 2))
	{
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c device port number[%d] is invalid!\r\n",i2cPortNum);
		return BOAT_ERROR;
	}
	if(i2cRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c device i2cRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	
	//! L610 only support speed 100k and 400k
	if((i2cConfig.i2cSpeed != 100000) && (i2cConfig.i2cSpeed != 400000))
	{
		//BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c device speed[%x] is invalid!\r\n",i2cConfig.i2cSpeed);
		return BOAT_ERROR;
	}
	//! L610 only support i2c slave address 7 bits
	if(i2cConfig.i2cSlaveAddrBits != 0)
	{
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c device i2cSlaveAddrBits[%d] is invalid!\r\n",i2cConfig.i2cSlaveAddrBits);
		return BOAT_ERROR;
	}
	//! L610 I2C slabe device register address is 2 bytes
	if(i2cConfig.i2cSlaveDevRegisterAddrLen != 1)
	{
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c device i2cSlaveDevRegisterAddrLen[%d] is invalid!\r\n",i2cConfig.i2cSlaveDevRegisterAddrLen);
		return BOAT_ERROR;
	}

	BoatLog(BOAT_LOG_NORMAL, "[boat][i2c] boatI2cOpen default value i2cRef->i2cId[%x] i2cConfig.i2cSpeed[%d] \n",i2cRef->i2cId,i2cConfig.i2cSpeed);

	bsp1_1.i2c_name = i2cPortNum;
	if(i2cConfig.i2cSpeed == 100000)
	{
		bsp1_1.value = 0; //! set the speed to 100K
	}
	else
	{
		bsp1_1.value = 1; //! set the speed to 400K
	}
	
	//! L610 I2C Pin set , Refer to "FIBOCOM L610-CN-00-00 GPIO功能复用_V1.0.5.xlsx"
    switch (i2cPortNum)
    {
    case 1:
        BoatLog(BOAT_LOG_NORMAL, "[boat][i2c] open L610 i2c device port number 1 \r\n");
        fibo_gpio_mode_set(41, 0); //! pin function 0
        fibo_gpio_mode_set(42, 0); //! pin function 0
        break;
    case 2:
        BoatLog(BOAT_LOG_NORMAL, "[boat][i2c] open L610 i2c device port number 2 \r\n");
        fibo_gpio_mode_set(141, 1); //! origin pin function 1
        fibo_gpio_mode_set(142, 1); //! origin pin function 1
        break;
    }

    rtnVal = fibo_i2c_open(bsp1_1, &i2c_handle);
    if (rtnVal < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][i2c] Error opening I2C device \n");
        return BOAT_ERROR;
    }

	//! set i2cRef value
	i2cRef->i2cId = i2c_handle;
	//! i2cRef->i2cConfig = i2cConfig;
    fibo_taskSleep(1000);
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
	BSINT32 rtnVal = 0;
	if(i2cRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c device i2cRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	//! i2c_Handle i2c_device = (i2c_Handle)fd;
    //! fibo_i2c_close(&i2c_device);
    if(i2cRef->i2cId > 0)
    {
    	rtnVal = fibo_i2c_close(&i2cRef->i2cId);
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c fibo_i2c_close[%x]\r\n",rtnVal);
		if (rtnVal < 0)
		{
			BoatLog(BOAT_LOG_NORMAL, "[boat][i2c] Error close I2C device \n");
			return BOAT_ERROR;
		}
    }
	else
	{
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] i2cRef->i2cId error[%x]\r\n",i2cRef->i2cId);
		return BOAT_ERROR;
	}
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
    BOAT_RESULT ret = BOAT_SUCCESS;
	if(i2cRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c writing i2cRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	if(slaveAddr > 0x7f)
	{
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c writing slaveAddr is invalid!\r\n");
		return BOAT_ERROR;
	}
	if(regAddr > 0xffff)
	{
		/////BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c writing slave device regAddr is invalid!\r\n");
		return BOAT_ERROR;
	}
    if (data == NULL)
    {
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c writing data is invalid!\r\n");
        return BOAT_ERROR;
    }
	if (dataLen == 0)
    {
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c writing dataLen is 0 , no data to write!\r\n");
        return BOAT_SUCCESS;
    }
	//! address width translation
	BUINT8 L610_slave_addr = (BUINT8)slaveAddr; //! 16 bits to 8 bits, 7 bits I2C slave address
	BUINT16 L610_reg_addr = (BUINT16)regAddr; //! 32 bits to 16 bits(2 bytes) I2C slave device register address 
	
    // BoatLog_hexdump(BOAT_LOG_NORMAL, "i2c send block : ", data, dataLen);
    //! i2c_Handle i2c_device = (i2c_Handle)fd;
    //! drvI2cSlave_t drv_i2c = {L610_slave_addr, 0, 0, false, false};
    drvI2cSlave_t drv_i2c = {L610_slave_addr, L610_reg_addr, 0, false, false};
    //! ret = fibo_i2c_Write(i2c_device, drv_i2c, data, dataLen);
    ret = fibo_i2c_Write(i2cRef->i2cId, drv_i2c, data, dataLen);
    if (ret != 0)
    {
        BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] Error sending[%x] \n",ret);
        return BOAT_ERROR;
    }
    BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] boatI2cMasterWrite boat_i2c_send_block OK  ");
    BoatSleepMs(1);
    BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] boatI2cMasterWrite boat_i2c_send_block OK 111  ");
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
    BOAT_RESULT ret = BOAT_SUCCESS;

	if(i2cRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c reading i2cRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	if(slaveAddr > 0x7f)
	{
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c reading slaveAddr is invalid!\r\n");
		return BOAT_ERROR;
	}
	if(regAddr > 0xffff)
	{
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c reading slave device regAddr is invalid!\r\n");
		return BOAT_ERROR;
	}
    if (data == NULL)
    {
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c reading data is invalid!\r\n");
        return BOAT_ERROR;
    }
	if (dataLen == 0)
    {
		BoatLog(BOAT_LOG_NORMAL,"[boat][i2c] L610 i2c reading dataLen is 0 , no data to write!\r\n");
        return BOAT_SUCCESS;
    }
	//! address width translation
	BUINT8 L610_slave_addr = (BUINT8)slaveAddr; //! 16 bits to 8 bits, 7 bits I2C slave address
	BUINT16 L610_reg_addr = (BUINT16)regAddr; //! 32 bits to 16 bits(2 bytes) I2C slave device register address 

    //! i2c_Handle i2c_device = (i2c_Handle)fd;
    //! drvI2cSlave_t drv_i2c = {slaveAddr, 0, 0, false, false};
    drvI2cSlave_t drv_i2c = {L610_slave_addr, L610_reg_addr, 0, false, false};
    //! ret = fibo_i2c_Read(i2c_device, drv_i2c, data, dataLen);
    ret = fibo_i2c_Read(i2cRef->i2cId, drv_i2c, data, dataLen);
    if (ret != 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][i2c] Error reading[%x] \n",ret);
        return BOAT_ERROR;
    }
    return BOAT_SUCCESS;
}


void boatI2CInitI2CIDZero(boatI2c *I2C)
{
	I2C->i2cId = 0;	///// 230123 modified to -1
}

