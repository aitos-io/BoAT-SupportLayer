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


#include "flash_block_device.h"
#include "drv_spi_flash.h"
#include "sffs.h"
#include "sffs_vfs.h"
#include "hal_chip.h"
#include "errno.h"

/******************************************************************************
                              BOAT FILE OPERATION WARPPER
*******************************************************************************/

sffsFs_t *g_boat_internal_fs = NULL;

BOAT_RESULT BoatInitFS(const BCHAR *fileName)
{	
	//DRV_NAME_SPI_FLASH
	drvSpiFlash_t *flash = drvSpiFlashOpen(HAL_FLASH_DEVICE_NAME(CONFIG_FS_SYS_FLASH_ADDRESS));
	if(flash ==  NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL,"drvSpiFlashOpen Failed!");
		return BOAT_ERROR;
	}

	blockDevice_t *bdev = flashBlockDeviceCreateV2(flash, 0, CONFIG_FS_SYS_FLASH_SIZE, CONFIG_FS_SYS_FLASH_ADDRESS, 16, false);
	if(bdev == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL,"flashBlockDeviceCreateV2 Failed!");
		return BOAT_ERROR;
	}

	g_boat_internal_fs = sffsMount(bdev, CONFIG_FS_SYS_FLASH_SIZE, false);
	if(g_boat_internal_fs == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL,"sffsMount Failed!");
		return BOAT_ERROR;
	}
	return BOAT_SUCCESS;
}


BOAT_RESULT BoatGetStorageSize(BUINT32 *size, void *rsvd)
{
	BSINT32 file_fd = -1;
	struct stat file_st;

	(void)rsvd;

	if (size == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which  'size' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	if (g_boat_internal_fs == NULL)
	{
		if (BoatInitFS(BOAT_FILE_STOREDATA) != BOAT_SUCCESS)
		{
			BoatLog(BOAT_LOG_CRITICAL, "BoatInitFS Failed!");
			return BOAT_ERROR;
		}
	}

	int fd = sffsOpen(g_boat_internal_fs, BOAT_FILE_STOREDATA, O_ACCMODE);

	if (fd == ENOENT || fd == EROFS || fd == ENOSPC || fd == ENAMETOOLONG || fd == ENOMEM)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}

	if (sffsFstat(g_boat_internal_fs, fd, &file_st) == 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		sffsClose(g_boat_internal_fs, fd);
		return BOAT_ERROR;
	}
	
	*size = file_st.st_size;
	sffsClose(g_boat_internal_fs, fd);

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

	BSINT32 count = 0;
	BUINT8 *buf_zero = NULL;
	BUINT8 *tmp_write_buf = NULL;
	BSINT32 totalLen = 0;
	struct stat file_st;


	(void)rsvd;

	if (writeBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which  'writeBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	
	if (g_boat_internal_fs == NULL)
	{
		if (BoatInitFS(BOAT_FILE_STOREDATA) != BOAT_SUCCESS)
		{
			BoatLog(BOAT_LOG_CRITICAL, "BoatInitFS Failed!");
			return BOAT_ERROR;
		}
	}

	//open or create file
	int fd = sffsOpen(g_boat_internal_fs, BOAT_FILE_STOREDATA, O_ACCMODE | O_CREAT);

	if (fd == ENOENT || fd == EROFS || fd == ENOSPC || fd == ENAMETOOLONG || fd == ENOMEM)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}

	if (sffsFstat(g_boat_internal_fs, fd, &file_st) == 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		sffsClose(g_boat_internal_fs, fd);
		return BOAT_ERROR;
	}

	tmp_write_buf = BoatMalloc(file_st.st_size + offset + writeLen);
	if(tmp_write_buf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to malloc buffer,size =  %d.", file_st.st_size + offset + writeLen);
		sffsClose(g_boat_internal_fs, fd);
		return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}
	memset(tmp_write_buf,0U,file_st.st_size + offset + writeLen);

	count = sffsRead(g_boat_internal_fs, fd, tmp_write_buf, file_st.st_size);
	if(count != file_st.st_size)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to read file.");
		sffsClose(g_boat_internal_fs, fd);
		BoatFree(tmp_write_buf);
		tmp_write_buf = NULL;
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}

	if(file_st.st_size < offset)
	{
		buf_zero = BoatMalloc(offset - file_st.st_size);
		if (NULL == buf_zero)
		{
			sffsClose(g_boat_internal_fs, fd);
			BoatFree(tmp_write_buf);
			tmp_write_buf = NULL;
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
		memset(buf_zero, 0x00, offset - file_st.st_size);

		memcpy(tmp_write_buf + count,buf_zero,offset - file_st.st_size);

		memcpy(tmp_write_buf + count + offset - file_st.st_size,writeBuf,writeLen);

		BoatFree(buf_zero);

		totalLen = file_st.st_size + offset + writeLen;
	}
	else
	{
		memcpy(tmp_write_buf + offset,writeBuf,writeLen);
		if(file_st.st_size >= (offset + writeLen))
        {
            totalLen = file_st.st_size;
        }
        else
        {
            totalLen = offset + writeLen;
        }
	}

	count = sffsSfileWrite(g_boat_internal_fs, BOAT_FILE_STOREDATA, tmp_write_buf,totalLen);
	sffsClose(g_boat_internal_fs, fd);
	if (count != totalLen)
	{
		BoatFree(tmp_write_buf);
		tmp_write_buf = NULL;
		BoatLog(BOAT_LOG_CRITICAL, "Failed to write file,total data size is %d.", totalLen);
		return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
	}

	BoatFree(tmp_write_buf);
	tmp_write_buf = NULL;

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

	BSINT32 count = 0;
	struct stat file_st;
	BUINT8 tmp_buf = NULL;

	(void)rsvd;

	if (readBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'readBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	if (g_boat_internal_fs == NULL)
	{
		if (BoatInitFS(BOAT_FILE_STOREDATA) != BOAT_SUCCESS)
		{
			BoatLog(BOAT_LOG_CRITICAL, "BoatInitFS Failed!");
			return BOAT_ERROR;
		}
	}

	//open or create file
	int fd = sffsOpen(g_boat_internal_fs, BOAT_FILE_STOREDATA, O_ACCMODE | O_CREAT);

	if (fd == ENOENT || fd == EROFS || fd == ENOSPC || fd == ENAMETOOLONG || fd == ENOMEM)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}

	if (sffsFstat(g_boat_internal_fs, fd, &file_st) == 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		sffsClose(g_boat_internal_fs, fd);
		return BOAT_ERROR;
	}

	if((readLen + offset) > file_st.st_size)
	{
		sffsClose(g_boat_internal_fs, fd);
        return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}

	tmp_buf = BoatMalloc(file_st.st_size);
	if(tmp_buf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Malloc %d failed.",file_st.st_size);
		sffsClose(g_boat_internal_fs, fd);
		return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
	}

	count = sffsRead(g_boat_internal_fs,fd,tmp_buf,file_st.st_size);

	if (count != file_st.st_size)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to read file: %s.", BOAT_FILE_STOREDATA);
		sffsClose(g_boat_internal_fs, fd);
		BoatFree(tmp_buf);
		tmp_buf = NULL;
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}

	memcpy(readBuf,tmp_buf+offset,readLen);

	sffsClose(g_boat_internal_fs, fd);
	BoatFree(tmp_buf);
	tmp_buf = NULL;
	return BOAT_SUCCESS;
}


BOAT_RESULT BoatRemoveFile(const BCHAR *fileName, void *rsvd)
{
    (void)rsvd;

    if (fileName == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "param which 'fileName' can't be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

	if (g_boat_internal_fs == NULL)
	{
		if (BoatInitFS(fileName) != BOAT_SUCCESS)
		{
			return BOAT_ERROR;
		}
	}

    if (0 != sffsUnlink(g_boat_internal_fs,fileName))
    {
        return BOAT_ERROR_STORAGE_FILE_REMOVE_FAIL;
    }
    else
    {
        return BOAT_SUCCESS;
    }
}