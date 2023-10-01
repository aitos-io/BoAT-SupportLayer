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
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

/////#include "fibo_opencpu.h"

/******************************************************************************
                              BOAT FILE OPERATION WARPPER
*******************************************************************************/
BOAT_RESULT BoatGetStorageSize(BUINT32 *size, void *rsvd)
{
	FILE *file_ptr;

	(void)rsvd;

	if (size == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which  'size' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	file_ptr = fopen(BOAT_FILE_STOREDATA, "rb");
	if (file_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}

	fseek(file_ptr, 0, SEEK_END);
	*size = ftell(file_ptr);
	fclose(file_ptr);

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
	FILE *file_ptr;
	BSINT32 count = 0;
	BUINT32 size = 0;
	BUINT8 *buf_zero = NULL;

	(void)rsvd;

	if (writeBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which  'writeBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	file_ptr = fopen(BOAT_FILE_STOREDATA, "rb+");
	if (file_ptr == NULL)
	{
		file_ptr = fopen(BOAT_FILE_STOREDATA, "wb");
		fclose(file_ptr);
		file_ptr = fopen(BOAT_FILE_STOREDATA, "rb+");
	}
	/* write to file-system */
	// file_ptr = fopen(fileName, "wb");
	if (file_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to create file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	/*move to the end of the file*/
	fseek(file_ptr, 0, SEEK_END);
	size = ftell(file_ptr);
	BoatLog(BOAT_LOG_NORMAL, "size = %d , offset = %d ", size, offset);
	if (size < offset)
	{
		buf_zero = BoatMalloc(offset - size);
		if (NULL == buf_zero)
		{// LCOV_EXCL_START
			fclose(file_ptr);
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}// LCOV_EXCL_STOP
		memset(buf_zero, 0x00, offset - size);
		count = fwrite(buf_zero, 1, offset - size, file_ptr);
		BoatFree(buf_zero);
		if (count != (offset - size))
		{
			fclose(file_ptr);
			BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
			return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
		}
		fseek(file_ptr, 0, SEEK_END);
	}
	else
	{
		fseek(file_ptr, offset, SEEK_SET);
	}
	count = fwrite(writeBuf, 1, writeLen, file_ptr);
	fclose(file_ptr);
	if (count != writeLen)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
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
	FILE *file_ptr;
	BSINT32 count = 0;
	BUINT32 size = 0;
	(void)rsvd;

	if (readBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'readBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	/* read from file-system */
	file_ptr = fopen(BOAT_FILE_STOREDATA, "rb");
	if (file_ptr == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	/*move to the end of the file*/
	fseek(file_ptr, 0, SEEK_END);
	size = ftell(file_ptr);
	if (readLen + offset > size)
	{
		fclose(file_ptr);
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}
	fseek(file_ptr, offset, SEEK_SET);
	count = fread(readBuf, 1, readLen, file_ptr);
	fclose(file_ptr);
	if (count != readLen)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to read file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}

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

    if (0 != unlink(fileName))
    {
        return BOAT_ERROR_STORAGE_FILE_REMOVE_FAIL;
    }
    else
    {
        return BOAT_SUCCESS;
    }
}