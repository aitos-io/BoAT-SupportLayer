/**
****************************************************************************************
* @FilePath: boat_storage.c
* @Author: aitos
* @Date: 2023-03-21 19:47:19
* @LastEditors:
* @LastEditTime: 2023-03-21 20:03:19
* @Descripttion:
****************************************************************************************
*/
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatstorage.h"
#include "boatplatformosal.h"
#include "boatlog.h"

#include "cm_fs.h"
#include "string.h"


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

    int32_t ret = cm_fs_filesize(BOAT_FILE_STOREDATA);
    if(ret < 0)
    {
        return BOAT_ERROR;
    }
    else
    {
        *size = (BUINT32) ret;
    }

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
    BSINT32 file_fd = -1;

    BSINT32 count = 0;
    BSINT32 size = 0;
    BUINT8 *buf_zero = NULL;

    (void)rsvd;

    if (writeBuf == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "param which  'writeBuf' can't be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    

    file_fd = cm_fs_open(BOAT_FILE_STOREDATA,CM_FS_WBPLUS);
    if(file_fd < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to open/create file: %s.", BOAT_FILE_STOREDATA);
        return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
    }

    /* write to file-system */
    size = cm_fs_filesize(BOAT_FILE_STOREDATA);
    BoatLog(BOAT_LOG_CRITICAL, "cm_fs_filesize get size is %d",size);
    if (size < offset)
    {
        /*move to the end of the file*/
        cm_fs_seek(file_fd,0,CM_FS_SEEK_END);
        buf_zero = BoatMalloc(offset - size);
        if (NULL == buf_zero)
        {
            cm_fs_close(file_fd);
            return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        memset(buf_zero, 0x00, offset - size);
        count = cm_fs_write(file_fd,buf_zero,offset - size);
        BoatFree(buf_zero);
        if (count != (offset - size))
        {
            cm_fs_close(file_fd);
            BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
            return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
        }
        cm_fs_seek(file_fd,0,CM_FS_SEEK_END);
    }
    else
    {
        if (cm_fs_seek(file_fd,offset,CM_FS_SEEK_SET) < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "file seek err ");
        }
    }
    count = cm_fs_write(file_fd,writeBuf,writeLen);
    cm_fs_close(file_fd);
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
    BSINT32 file_fd = -1;

    BSINT32 count = 0;
    BSINT32 size = 0;
    (void)rsvd;

    if ((BOAT_FILE_STOREDATA == NULL) || (readBuf == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "param 'readBuf' can't be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    /* read from file-system */
    file_fd = cm_fs_open(BOAT_FILE_STOREDATA,CM_FS_ABPLUS);
    if (file_fd >= 0)
    {
        /*move to the end of the file*/
        size = cm_fs_filesize(BOAT_FILE_STOREDATA);
        if ((BSINT32)(readLen + offset) > size)
        {
            cm_fs_close(file_fd);
            return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
        }
        cm_fs_seek(file_fd,offset,CM_FS_SEEK_SET);
        count = cm_fs_read(file_fd,readBuf,readLen);
        cm_fs_close(file_fd);
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
        return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
    }

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

    if (0 > cm_fs_delete(fileName))
    {
        return BOAT_ERROR_STORAGE_FILE_REMOVE_FAIL;
    }
    else
    {
        return BOAT_SUCCESS;
    }
}