/**
****************************************************************************************
* @FilePath: i2c_hal.c
* @Author: aitos
* @Date: 2022-12-22 14:07:29
* @LastEditors:
* @LastEditTime: 2022-12-22 14:07:29
* @Descripttion:
****************************************************************************************
*/
/**
****************************************************************************************
* @FilePath: i2c_hal.c
* @Author: aitos
* @Date: 2022-12-22 14:06:59
* @LastEditors:
* @LastEditTime: 2022-12-22 14:06:59
* @Descripttion:
****************************************************************************************
*/
/**
****************************************************************************************
* @FilePath: i2c_hal.c
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
#include "boatDAL.h"

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
BOAT_RESULT boatI2COpen(boatI2C *I2CRef,BUINT8 I2CPortNum,BoATI2CConfig_t I2CConfig)
{
    int rtnVal = 0;
    i2c_Handle i2c_handle = 0;
	static HAL_I2C_BPS_T bsp1_1; //! = {2, 0};
	//! L610 I2C port number 1-2
	if((I2CPortNum < 1) || (I2CPortNum > 2))
	{
		BoatPrintf(0,"[boat][I2C] L610 i2c device port number[%d] is invalid!\r\n",I2CPortNum);
		return BOAT_ERROR;
	}
	if(I2CRef == NULL)
	{
		BoatPrintf(0,"[boat][I2C] L610 i2c device I2CRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	
	//! L610 only support speed 100k and 400k
	if((I2CConfig.I2CSpeed != 100000) && (I2CConfig.I2CSpeed != 400000))
	{
		BoatPrintf(0,"[boat][I2C] L610 i2c device speed[%x] is invalid!\r\n",I2CConfig.I2CSpeed);
		return BOAT_ERROR;
	}
	//! L610 only support i2c slave address 7 bits
	if(I2CConfig.I2CSlaveAddrBits != 0)
	{
		BoatPrintf(0,"[boat][I2C] L610 i2c device I2CSlaveAddrBits[%d] is invalid!\r\n",I2CConfig.I2CSlaveAddrBits);
		return BOAT_ERROR;
	}
	//! L610 I2C slabe device register address is 2 bytes
	if(I2CConfig.I2CSlaveDevRegisterAddrLen != 1)
	{
		BoatPrintf(0,"[boat][I2C] L610 i2c device I2CSlaveDevRegisterAddrLen[%d] is invalid!\r\n",I2CConfig.I2CSlaveDevRegisterAddrLen);
		return BOAT_ERROR;
	}

	BoatPrintf(0, "[boat][I2C] boatI2COpen default value I2CRef->I2CID[%x] I2CConfig.I2CSpeed[%d] \n",I2CRef->I2CID,I2CConfig.I2CSpeed);

	bsp1_1.i2c_name = I2CPortNum;
	if(I2CConfig.I2CSpeed == 100000)
	{
		bsp1_1.value = 0; //! set the speed to 100K
	}
	else
	{
		bsp1_1.value = 1; //! set the speed to 400K
	}
	
	//! L610 I2C Pin set , Refer to "FIBOCOM L610-CN-00-00 GPIO功能复用_V1.0.5.xlsx"
	switch(I2CPortNum){
		case 1:
			BoatPrintf(0,"[boat][I2C] open L610 i2c device port number 1 \r\n");
			fibo_gpio_mode_set(41, 0); //! pin function 0
			fibo_gpio_mode_set(42, 0); //! pin function 0
			break;
		case 2:
			BoatPrintf(0,"[boat][I2C] open L610 i2c device port number 2 \r\n");
			fibo_gpio_mode_set(141, 1); //! origin pin function 1
			fibo_gpio_mode_set(142, 1); //! origin pin function 1
			break;
	}
	
    rtnVal = fibo_i2c_open(bsp1_1, &i2c_handle);
    if (rtnVal < 0)
    {
        BoatPrintf(0, "[boat][I2C] Error opening I2C device \n");
        return BOAT_ERROR;
    }

	//! set I2CRef value
	I2CRef->I2CID = i2c_handle;
	//! I2CRef->I2CConfig = I2CConfig;
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
BOAT_RESULT boatI2CClose(boatI2C *I2CRef)
{
	BSINT32 rtnVal = 0;
	if(I2CRef == NULL)
	{
		BoatPrintf(0,"[boat][I2C] L610 i2c device I2CRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	//! i2c_Handle i2c_device = (i2c_Handle)fd;
    //! fibo_i2c_close(&i2c_device);
    if(I2CRef->I2CID > 0)
    {
    	rtnVal = fibo_i2c_close(&I2CRef->I2CID);
		BoatPrintf(0,"[boat][I2C] L610 i2c fibo_i2c_close[%x]\r\n",rtnVal);
		if (rtnVal < 0)
		{
			BoatPrintf(0, "[boat][I2C] Error close I2C device \n");
			return BOAT_ERROR;
		}
    }
	else
	{
		BoatPrintf(0,"[boat][I2C] I2CRef->I2CID error[%x]\r\n",I2CRef->I2CID);
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
* @param[in] slave_addr
*  address of slave device;
* @param[in] reg_addr
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
//! BOAT_RESULT boat_i2c_send_block(int fd, BUINT8 slave_addr, BUINT16 reg_addr, BUINT8 *data, BUINT32 datalen)
BOAT_RESULT boatI2CMasterWrite(boatI2C *I2CRef, BUINT16 slave_addr, BUINT32 reg_addr, BUINT8 *data, BUINT16 datalen)
{
    BOAT_RESULT ret = BOAT_SUCCESS;
	if(I2CRef == NULL)
	{
		BoatPrintf(0,"[boat][I2C] L610 i2c writing I2CRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	if(slave_addr > 0x7f)
	{
		BoatPrintf(0,"[boat][I2C] L610 i2c writing slave_addr is invalid!\r\n");
		return BOAT_ERROR;
	}
	if(reg_addr > 0xffff)
	{
		BoatPrintf(0,"[boat][I2C] L610 i2c writing slave device reg_addr is invalid!\r\n");
		return BOAT_ERROR;
	}
    if (data == NULL)
    {
		BoatPrintf(0,"[boat][I2C] L610 i2c writing data is invalid!\r\n");
        return BOAT_ERROR;
    }
	if (datalen == 0)
    {
		BoatPrintf(0,"[boat][I2C] L610 i2c writing datalen is 0 , no data to write!\r\n");
        return BOAT_SUCCESS;
    }
	//! address width translation
	BUINT8 L610_slave_addr = (BUINT8)slave_addr; //! 16 bits to 8 bits, 7 bits I2C slave address
	BUINT16 L610_reg_addr = (BUINT16)reg_addr; //! 32 bits to 16 bits(2 bytes) I2C slave device register address 
	
    // BoatLog_hexdump(BOAT_LOG_NORMAL, "i2c send block : ", data, datalen);
    //! i2c_Handle i2c_device = (i2c_Handle)fd;
    //! drvI2cSlave_t drv_i2c = {L610_slave_addr, 0, 0, false, false};
    drvI2cSlave_t drv_i2c = {L610_slave_addr, L610_reg_addr, 0, false, false};
    //! ret = fibo_i2c_Write(i2c_device, drv_i2c, data, datalen);
    ret = fibo_i2c_Write(I2CRef->I2CID, drv_i2c, data, datalen);
    if (ret != 0)
    {
        BoatPrintf(0,"[boat][I2C] Error sending \n");
        return BOAT_ERROR;
    }
    BoatPrintf(0,"[boat][I2C] boatI2CMasterWrite boat_i2c_send_block OK  ");
    BoatSleepMs(1);
    BoatPrintf(0,"[boat][I2C] boatI2CMasterWrite boat_i2c_send_block OK 111  ");
    return BOAT_SUCCESS;
}
/**
****************************************************************************************
* @brief:
*  This function read data from slave device;
* @param[in] fd
*  device handle id opened;
* @param[in] slave_addr
*  address of slave device;
* @param[in] reg_addr
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
//! BOAT_RESULT boat_i2c_read_data(int fd, BUINT8 slave_addr, BUINT16 reg_addr, BUINT8 *data, BUINT32 datalen)
BOAT_RESULT boatI2CMasterRead(boatI2C *I2CRef, BUINT16 slave_addr, BUINT32 reg_addr, BUINT8 *data, BUINT16 datalen)
{
    BOAT_RESULT ret = BOAT_SUCCESS;

	if(I2CRef == NULL)
	{
		BoatPrintf(0,"[boat][I2C] L610 i2c reading I2CRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	if(slave_addr > 0x7f)
	{
		BoatPrintf(0,"[boat][I2C] L610 i2c reading slave_addr is invalid!\r\n");
		return BOAT_ERROR;
	}
	if(reg_addr > 0xffff)
	{
		BoatPrintf(0,"[boat][I2C] L610 i2c reading slave device reg_addr is invalid!\r\n");
		return BOAT_ERROR;
	}
    if (data == NULL)
    {
		BoatPrintf(0,"[boat][I2C] L610 i2c reading data is invalid!\r\n");
        return BOAT_ERROR;
    }
	if (datalen == 0)
    {
		BoatPrintf(0,"[boat][I2C] L610 i2c reading datalen is 0 , no data to write!\r\n");
        return BOAT_SUCCESS;
    }
	//! address width translation
	BUINT8 L610_slave_addr = (BUINT8)slave_addr; //! 16 bits to 8 bits, 7 bits I2C slave address
	BUINT16 L610_reg_addr = (BUINT16)reg_addr; //! 32 bits to 16 bits(2 bytes) I2C slave device register address 

    //! i2c_Handle i2c_device = (i2c_Handle)fd;
    //! drvI2cSlave_t drv_i2c = {slave_addr, 0, 0, false, false};
    drvI2cSlave_t drv_i2c = {L610_slave_addr, L610_reg_addr, 0, false, false};
    //! ret = fibo_i2c_Read(i2c_device, drv_i2c, data, datalen);
    ret = fibo_i2c_Read(I2CRef->I2CID, drv_i2c, data, datalen);
    if (ret != 0)
    {
        BoatPrintf(0, "[boat][I2C] Error reading[%x] \n",ret);
        return BOAT_ERROR;
    }
    return BOAT_SUCCESS;
}


void boatI2CInitI2CIDZero(boatI2C *I2C)
{
	I2C->I2CID =-1;	///// 230123 modified to -1
}

