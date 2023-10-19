/**
****************************************************************************************
* @FilePath: boat_storage.c
* @Author: aitos
* @Date: 2023-02-15 10:03:19
* @LastEditors:
* @LastEditTime: 2023-02-15 10:03:19
* @Descripttion:
****************************************************************************************
*/
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatstorage.h"
#include "boatplatformosal.h"
#include "boatlog.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sffs.h"
#include "sffs_vfs.h"
#include "hal_chip.h"
#include "errno.h"
#include "drv_spi.h"


/******************************************************************************
                              BOAT FILE OPERATION WARPPER
*******************************************************************************/


#define BOAT_8850_FILE_STOREDATA "/boat/boat_wallet_file"


BOAT_RESULT BoatGetStorageSize(BUINT32 *size, void *rsvd)
{
	if (size == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which  'size' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	ssize_t file_sz = vfs_file_size(BOAT_8850_FILE_STOREDATA);

	if(file_sz < 0)
	{
		*size = 0;
		BoatLog(BOAT_LOG_CRITICAL, "vfs_file_size Failed!");
		return BOAT_ERROR;
	}

	*size = (BUINT32)file_sz;

	return BOAT_SUCCESS;

}

/**
 * @description:
 * 	This function wirte data into file.
 * @param[in] {BUINT32} offset
 * 	write data from offset as the starting point of the file
 * @param[in] {BUINT8} *writeBuf
 * 	the data that will be written into file
 * @param {BUINT32} writeLen
 * 	the length of data that will be written
 * @param {void} *rsvd
 * 	reserved , not used now
 * @return {*}
 * 	This function will return BOAT_SUCCESS if write successfully.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoatWriteStorage(BUINT32 offset, BUINT8 *writeBuf, BUINT32 writeLen, void *rsvd)
{

	int ret,fd;
	ssize_t file_sz,write_sz;

	BSINT32 count = 0;
	BUINT8 *buf_zero = NULL;

	if (writeBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which  'writeBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	DIR *dir = vfs_opendir("/boat");
	if(dir == NULL)
	{
		ret = vfs_mkdir("/boat",0);
		if(ret != 0)
		{
			return BOAT_ERROR;
		}
	}

	fd = vfs_open(BOAT_8850_FILE_STOREDATA, O_RDWR | O_CREAT);
	if(fd < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL,"Open %s failed.",BOAT_8850_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}

	file_sz = vfs_file_size(BOAT_8850_FILE_STOREDATA);


	if(offset > file_sz)
	{
		vfs_lseek(fd,0, SEEK_END);
		buf_zero = BoatMalloc(offset - file_sz);
		if (NULL == buf_zero)
		{
			vfs_close(fd);
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
		memset(buf_zero, 0x00, offset - file_sz);
		count = vfs_write(fd,buf_zero, offset - file_sz);
		BoatFree(buf_zero);
		if (count != (offset - file_sz))
		{
			vfs_close(fd);
			BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_8850_FILE_STOREDATA);
			return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
		}
		vfs_lseek(fd, 0, SEEK_END);

	}
	else
	{
		vfs_lseek(fd,offset,SEEK_SET);
	}

	write_sz = vfs_write(fd,writeBuf,writeLen);
	vfs_close(fd);

	if(write_sz != writeLen)
	{
		BoatLog(BOAT_LOG_CRITICAL, "vfs_sfile_write Failed!");
		return BOAT_ERROR;
	}

	return BOAT_SUCCESS;
}

/**
 * @description:
 * 	This function read data from file
 * @param {BUINT32} offset
 * 	read data from offset as the starting point of the file
 * @param {BUINT8} *readBuf
 * 	store data reading form the file
 * @param {BUINT32} readLen
 * 	the length of data that want to read
 * @param {void} *rsvd
 * 	reserved , not used now
 * @return {*}
 * 	This function will return BOAT_SUCCESS if read successfully.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoatReadStorage(BUINT32 offset, BUINT8 *readBuf, BUINT32 readLen, void *rsvd)
{
	int ret,fd;
	ssize_t file_sz;
	if (readBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'readBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	fd = vfs_open(BOAT_8850_FILE_STOREDATA, O_RDONLY);
	if(fd < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL,"Open %s failed.",BOAT_8850_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}

	file_sz = vfs_file_size(BOAT_8850_FILE_STOREDATA);
	if (readLen + offset > file_sz)
	{
		vfs_close(fd);
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}
	
	vfs_lseek(fd,offset,SEEK_SET);

	ssize_t read_sz = vfs_read(fd,readBuf,readLen);
	vfs_close(fd);
	if(read_sz != readLen)
	{
		BoatLog(BOAT_LOG_NORMAL,"vfs_sfile_read failed!");
		return BOAT_ERROR;
	}

	return BOAT_SUCCESS;
}


BOAT_RESULT BoatRemoveFile(const BCHAR *fileName, void *rsvd)
{
	if (fileName == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "param which 'fileName' can't be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

	vfs_unlink(BOAT_8850_FILE_STOREDATA);

	return BOAT_SUCCESS;
}