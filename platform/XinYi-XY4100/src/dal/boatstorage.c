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

#include "xy_fs.h"

/******************************************************************************
                              BOAT FILE OPERATION WARPPER
*******************************************************************************/

BOAT_RESULT BoatGetStorageSize(BUINT32 *size, void *rsvd)
{

	(void)rsvd;

	if (size == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which  'size' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	XY_FILE  *file = xy_fopen(BOAT_FILE_STOREDATA,"r+");
	if(file == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}

	*size = xy_fsize(file);

	xy_fclose(file);

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

    XY_FILE *file_hdl = NULL;

	BSINT32 count = 0;
	BUINT32 size = 0;
	BUINT8 *buf_zero = NULL;

	(void)rsvd;

	if (writeBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which  'writeBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	file_hdl = xy_fopen(BOAT_FILE_STOREDATA, "rb+");
	if (file_hdl == NULL)
	{
		if(xy_fopen_dir(BOAT_FILE_PATH) < 0)
		{
			xy_fmkdir(BOAT_FILE_PATH);
		}
		
		file_hdl = xy_fopen(BOAT_FILE_STOREDATA, "wb");
		xy_fclose(file_hdl);
		file_hdl = xy_fopen(BOAT_FILE_STOREDATA, "rb+");
	}
	/* write to file-system */
	if (file_hdl == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to create file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	/*move to the end of the file*/
	xy_fseek(file_hdl, 0, FS_SEEK_END);
	size = xy_ftell(file_hdl);
	BoatLog(BOAT_LOG_NORMAL, "size = %d , offset = %d ", size, offset);
	if (size < offset)
	{
		buf_zero = BoatMalloc(offset - size);
		if (NULL == buf_zero)
		{
			xy_fclose(file_hdl);
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
		memset(buf_zero, 0x00, offset - size);
		count = xy_fwrite(buf_zero, 1, offset - size, file_hdl);
		BoatFree(buf_zero);
		if (count != (offset - size))
		{
			xy_fclose(file_hdl);
			BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
			return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
		}
		xy_fseek(file_hdl, 0, FS_SEEK_END);
	}
	else
	{
		xy_fseek(file_hdl, offset, FS_SEEK_BEGIN);
	}
	count = xy_fwrite(writeBuf, 1, writeLen, file_hdl);
	xy_fclose(file_hdl);
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
    XY_FILE *file_hdl = NULL;

	BSINT32 count = 0;
	BUINT32 size = 0;
	(void)rsvd;

	if (readBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'readBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	/* read from file-system */
	file_hdl = xy_fopen(BOAT_FILE_STOREDATA, "rb");
	if (file_hdl == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	/*move to the end of the file*/
	xy_fseek(file_hdl, 0, FS_SEEK_END);
	size = xy_ftell(file_hdl);
	if (readLen + offset > size)
	{
		xy_fclose(file_hdl);
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}
	xy_fseek(file_hdl, offset, FS_SEEK_SET);
	count = xy_fread(readBuf, 1, readLen, file_hdl);
	xy_fclose(file_hdl);
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

    xy_fremove(fileName);
    
    return BOAT_SUCCESS;
}