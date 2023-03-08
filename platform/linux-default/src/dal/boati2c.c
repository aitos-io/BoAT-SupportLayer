#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include "boattypes.h"
#include "boatlog.h"
#include "boatdal.h"

/**
 * \brief I2C msg write data
 *
 * \param[in] fd：I2C device descriptor
 * \param[in] dev_addr：I2C device addr, 7bits only
 * \param[in] reg_addr：slave device register addr, 1 byte only
 * \param[in] data：data to write
 * \param[in] len：data length
 *
 * \retval, if write successful return 0，else return -1
 */
int i2c_write(int fd, unsigned short dev_addr, unsigned char reg_addr, unsigned char* data, unsigned int len)
{
    int ret = -1;
    unsigned char buff[20] = { 0 };

    buff[0] = reg_addr;
    memcpy(&buff[1], data, len);

    //write data, just use one msg
    struct i2c_msg msg = {
        .addr = dev_addr,
        .flags = 0,
        .len = len + 1,
        .buf = buff,
    };

    struct i2c_rdwr_ioctl_data rdwr_msg = {
        .msgs = &msg,
        .nmsgs = 1,
    };

    ret = ioctl(fd, I2C_RDWR, &rdwr_msg);

    return ret;
}

/**
 * \brief I2C读数据
 *
 * \param[in] fd：I2C device descriptor
 * \param[in] dev_addr：I2C device addr , 7bits only
 * \param[in] reg_addr：slave device register addr, 1 byte only
 * \param[out] data：place for reading data
 * \param[in] len：reading data length
 *
 * \retval, if write successful return 0，else return -1
 *
 */
int i2c_read(int fd, unsigned short dev_addr,  unsigned char reg_addr, unsigned char* data, unsigned int len)
{
    int ret = -1;

    //mast read has 2 msgs
    struct i2c_msg msg[2] = {
        {
            .addr = dev_addr,   //client dev address
            .flags = 0,         //flag, 0 write
            .len = 1,           //write length
            .buf = &reg_addr,   //client dev register address
        },
        {
            .addr = dev_addr,   //client dev address
            .flags = I2C_M_RD,  //flag, I2C_M_RD, read data from client device
            .len = len,         //read data length
            .buf = data,        //place to store reading data
        },
    };

    struct i2c_rdwr_ioctl_data rdwr_msg = {
        .msgs = msg,
        .nmsgs = 2,
    };

    ret = ioctl(fd, I2C_RDWR, &rdwr_msg);

    return ret;
}

BOAT_RESULT boatI2cOpen(boatI2c *i2cRef,BUINT8 i2cPortNum,boatI2cConfig i2cConfig)
{
	(void)i2cRef;
	(void)i2cPortNum;
	(void)i2cConfig;
	return BOAT_ERROR;
}
BOAT_RESULT boatI2cClose(boatI2c *i2cRef)
{
	(void)i2cRef;
	return BOAT_ERROR;
}
BOAT_RESULT boatI2cMasterWrite(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 dataLen)
{
	(void)i2cRef;
	(void)slaveAddr;
	(void)regAddr;
	(void)data;
	(void)dataLen;
	return BOAT_ERROR;
}
BOAT_RESULT boatI2cMasterRead(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 dataLen)
{
	(void)i2cRef;
	(void)slaveAddr;
	(void)regAddr;
	(void)data;
	(void)dataLen;
	return BOAT_ERROR;
}
void boatI2CInitI2CIDZero(boatI2c *I2C)
{
	I2C->i2cId = 0;
}

