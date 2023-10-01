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
#include "ol_flash_fs.h"
/******************************************************************************
                              BOAT FILE OPERATION WARPPER
*******************************************************************************/

BOAT_RESULT BoatGetStorageSize(BUINT32 *size, void *rsvd)
{
    // BSINT32 file_fd = -1;
    (void)rsvd;

    if (size == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "param 'size' can't be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    *size = ol_ffs_getsize(BOAT_FILE_STOREDATA);

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

    int handle;

    BSINT32 count = 0;
    BUINT32 size = 0;
    BUINT8 *buf_zero = NULL;

    (void)rsvd;

    if (writeBuf == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "param which  'writeBuf' can't be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    handle = ol_ffs_open(BOAT_FILE_STOREDATA, "rb+");
	if(handle < 0){
		handle = ol_ffs_open(BOAT_FILE_STOREDATA, "wb");
		ol_ffs_close(handle);
		handle = ol_ffs_open(BOAT_FILE_STOREDATA, "rb+");
	}
	/* write to file-system */
	if (handle < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to create file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	/*move to the end of the file*/
	ol_ffs_seek(handle, 0, OL_FS_SEEK_END);
	size = ol_ffs_ftell(handle);
	BoatLog(BOAT_LOG_NORMAL,"size = %d , offset = %d ",size,offset);
	if(size < offset){
		buf_zero = BoatMalloc(offset - size);
		if(NULL == buf_zero){
			ol_ffs_close(handle);
			return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		}
		memset(buf_zero,0x00,offset - size);
		count = ol_ffs_write(handle, buf_zero, offset - size);
		BoatFree(buf_zero);
		if (count != (offset - size))
		{
			ol_ffs_close(handle);
			BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
			return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
		}
		ol_ffs_seek(handle, 0, OL_FS_SEEK_END);
	}else{
		ol_ffs_seek(handle, offset, OL_FS_SEEK_SET);
	}
	count = ol_ffs_write(handle, writeBuf, writeLen);
	ol_ffs_close(handle);
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
    int handle;

    BSINT32 count = 0;
	BUINT32 size = 0;
	(void)rsvd;

	if (readBuf == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "param which 'readBuf' can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

    /* read from file-system */
	handle = ol_ffs_open(BOAT_FILE_STOREDATA, "rb");
	if(handle < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
		return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
	}
	/*move to the end of the file*/
	ol_ffs_seek(handle, 0, OL_FS_SEEK_END);
	size = ol_ffs_ftell(handle);
	if(readLen + offset > size){
		ol_ffs_close(handle);
		return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
	}
	ol_ffs_seek(handle, offset, OL_FS_SEEK_SET);
	count = ol_ffs_read(handle, readBuf, readLen);
	ol_ffs_close(handle);
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

    ol_ffs_delete(fileName);
    
    return BOAT_SUCCESS;
}