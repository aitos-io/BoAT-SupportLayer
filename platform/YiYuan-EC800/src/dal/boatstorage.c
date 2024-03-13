
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

#include "fs/ql_fs.h"
#include "string.h"

// Those macro define as SEEK_SET/CUR/END in ql_fs.h in the kernel folder, but not define in ql_fs.h in the app folder 
#define QL_SEEK_SET 0						/* Seek from start of file	   */
#define QL_SEEK_CUR 1						/* Seek from current position  */
#define QL_SEEK_END 2						/* Seek from end of file	   */

/*
QFILE * ql_fopen(const char *fname, const char *mode);
int ql_fclose(QFILE * fp);
int ql_fsync(QFILE * fp);
int ql_fread(void * buffer, size_t size, size_t num, QFILE * fp);
int ql_fwrite(void * buffer, size_t size, size_t num, QFILE * fp);
int ql_fseek(QFILE *fp, long offset, int origin);
int ql_fsize(QFILE *fp);
int ql_remove(const char *fname);
*/



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

	char *filename = "U:/" BOAT_FILE_STOREDATA ;
	QFILE * file_fd = ql_fopen(filename, "r");

	if(!file_fd)
	{

        BoatLog(BOAT_LOG_CRITICAL, "ql_fopen fail");
        return BOAT_ERROR;
	}
	BUINT32 ret = ql_fsize(file_fd);
	if(ret < 0)
	{
        BoatLog(BOAT_LOG_CRITICAL, "ql_fsize fail");
        return BOAT_ERROR;
	}

   *size = (BUINT32) ret;

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
    BSINT32 size = 0;
    BSINT32 totalLen = 0;
    BUINT8 *tmp_write_buf = NULL;
    BUINT8 *buf_zero = NULL;

    (void)rsvd;

    if (writeBuf == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "param which  'writeBuf' can't be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
	
	char *filename = "U:/" BOAT_FILE_STOREDATA ;
	QFILE * file_fd = ql_fopen(filename, "rb");
	if(!file_fd)
	{
		
        BoatLog(BOAT_LOG_CRITICAL, "ql_fopen fail, create new file");		

		file_fd = ql_fopen(filename, "wb");	// wb , seek to the beginning of the file
		if(!file_fd)
		{			
			BoatLog(BOAT_LOG_CRITICAL, "ql_fopen, create new file fail");			
			return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
		}
		
        ql_fseek(file_fd,offset,QL_SEEK_SET);

		count = ql_fwrite(writeBuf, 1, writeLen, file_fd); // buf, 1, len, fp
        ql_fclose(file_fd);
        if (count != writeLen)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
            return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
        }
        return BOAT_SUCCESS;
	}
	else
	{
		/* write to file-system */
        size = ql_fsize(file_fd);
        BoatLog(BOAT_LOG_CRITICAL, "cm_fs_filesize get size is %d",size);

        tmp_write_buf = malloc(size+offset+writeLen);
        if(tmp_write_buf == NULL)
        {
            ql_fclose(file_fd);
            return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        memset(tmp_write_buf,0U,size+offset+writeLen);

        count = ql_fread(tmp_write_buf, 1, size, file_fd); // rdbuf, 1, size, fp
        if(count != size)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Failed to read file: %s.", BOAT_FILE_STOREDATA);
            ql_fclose(file_fd);
            free(tmp_write_buf);
            tmp_write_buf = NULL;
            return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
        }

        ql_fclose(file_fd);

        if (size < offset)
        {
            /*move to the end of the file*/
            buf_zero = malloc(offset - size);
            if (NULL == buf_zero)
            {
                free(tmp_write_buf);
                tmp_write_buf = NULL;
                return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
            }
            memset(buf_zero, 0x00, offset - size);
            memcpy(tmp_write_buf + count,buf_zero,offset - size);

            free(buf_zero);
            buf_zero = NULL;

            memcpy(tmp_write_buf + count + offset - size,writeBuf,writeLen);

            totalLen = offset + writeLen;

        }
        else
        {
            memcpy(tmp_write_buf + offset,writeBuf,writeLen);
            if(size >= (offset + writeLen))
            {
                totalLen = size;
            }
            else
            {
                totalLen = offset + writeLen;
            }
        }

        file_fd = ql_fopen(filename,"wb");
        if(file_fd < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
            free(tmp_write_buf);
            tmp_write_buf = NULL;
            return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
        }

    
        count = ql_fwrite(tmp_write_buf, 1, totalLen, file_fd);
        ql_fclose(file_fd);
        if (count != totalLen)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
            free(tmp_write_buf);
            tmp_write_buf = NULL;
            return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
        }

        free(tmp_write_buf);
        tmp_write_buf = NULL;
        return BOAT_SUCCESS;
        
	}


#if 0	
    ret = cm_fs_exist(BOAT_FILE_STOREDATA);
    if(ret == 0)
    {
        //file is not exist
        file_fd = cm_fs_open(BOAT_FILE_STOREDATA,CM_FS_WB);
        if(file_fd < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
            return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
        }

        cm_fs_seek(file_fd,offset,CM_FS_SEEK_SET);
    
        count = cm_fs_write(file_fd,writeBuf,writeLen);
        cm_fs_close(file_fd);
        if (count != writeLen)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
            return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
        }
        return BOAT_SUCCESS;
    }
    else
    {
        //file exist
        //read all first,then write
        file_fd = cm_fs_open(BOAT_FILE_STOREDATA,CM_FS_RB);
        if(file_fd < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
            return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
        }

        /* write to file-system */
        size = cm_fs_filesize(BOAT_FILE_STOREDATA);
        BoatLog(BOAT_LOG_CRITICAL, "cm_fs_filesize get size is %d",size);

        tmp_write_buf = cm_malloc(size+offset+writeLen);
        if(tmp_write_buf == NULL)
        {
            cm_fs_close(file_fd);
            return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        memset(tmp_write_buf,0U,size+offset+writeLen);

        count = cm_fs_read(file_fd,tmp_write_buf,size);
        if(count != size)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Failed to read file: %s.", BOAT_FILE_STOREDATA);
            cm_fs_close(file_fd);
            cm_free(tmp_write_buf);
            tmp_write_buf = NULL;
            return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
        }

        cm_fs_close(file_fd);

        if (size < offset)
        {
            /*move to the end of the file*/
            //cm_fs_seek(file_fd,0,CM_FS_SEEK_END);
            buf_zero = BoatMalloc(offset - size);
            if (NULL == buf_zero)
            {
                cm_free(tmp_write_buf);
                tmp_write_buf = NULL;
                return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
            }
            memset(buf_zero, 0x00, offset - size);
            //count = cm_fs_write(file_fd,buf_zero,offset - size);
            memcpy(tmp_write_buf + count,buf_zero,offset - size);

            BoatFree(buf_zero);
            buf_zero = NULL;

            memcpy(tmp_write_buf + count + offset - size,writeBuf,writeLen);

            totalLen = offset + writeLen;

        }
        else
        {
            // if (cm_fs_seek(file_fd,offset,CM_FS_SEEK_SET) < 0)
            // {
            //     BoatLog(BOAT_LOG_CRITICAL, "file seek err ");
            // }
            memcpy(tmp_write_buf + offset,writeBuf,writeLen);
            if(size >= (offset + writeLen))
            {
                totalLen = size;
            }
            else
            {
                totalLen = offset + writeLen;
            }
        }

        file_fd = cm_fs_open(BOAT_FILE_STOREDATA,CM_FS_WB);
        if(file_fd < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Failed to open file: %s.", BOAT_FILE_STOREDATA);
            cm_free(tmp_write_buf);
            tmp_write_buf = NULL;
            return BOAT_ERROR_STORAGE_FILE_OPEN_FAIL;
        }

    
        count = cm_fs_write(file_fd,tmp_write_buf,totalLen);
        cm_fs_close(file_fd);
        if (count != totalLen)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Failed to write file: %s.", BOAT_FILE_STOREDATA);
            cm_free(tmp_write_buf);
            tmp_write_buf = NULL;
            return BOAT_ERROR_STORAGE_FILE_WRITE_FAIL;
        }

        cm_free(tmp_write_buf);
        tmp_write_buf = NULL;
        return BOAT_SUCCESS;
        
    }
#endif
    
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
    BSINT32 size = 0;
    (void)rsvd;

    if ((BOAT_FILE_STOREDATA == NULL) || (readBuf == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "param 'readBuf' can't be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    /* read from file-system */
	char *filename = "U:/" BOAT_FILE_STOREDATA ;
	QFILE * file_fd = ql_fopen(filename, "rb");
    if (file_fd >= 0)
    {
        /*move to the end of the file*/
        size = ql_fsize(file_fd);
        if ((BSINT32)(readLen + offset) > size)
        {
            ql_fclose(file_fd);
            return BOAT_ERROR_STORAGE_FILE_READ_FAIL;
        }
        ql_fseek(file_fd,offset,QL_SEEK_SET);
        count = ql_fread(readBuf, 1, readLen, file_fd);
        ql_fclose(file_fd);
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

#if 0
    file_fd = cm_fs_open(BOAT_FILE_STOREDATA,CM_FS_RB);
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
#endif
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
	char pfilename[strlen(fileName)+4];
	memset(pfilename, 0, sizeof(pfilename));
	memcpy(pfilename,"U:/",3);
	memcpy(&pfilename[3],fileName,strlen(fileName));
	pfilename[strlen(pfilename)] = 0;

	BoatLog(BOAT_LOG_CRITICAL, "ql_remove file :%s",pfilename);
    if (0 != ql_remove(pfilename))
    {
        return BOAT_ERROR_STORAGE_FILE_REMOVE_FAIL;
    }
    else
    {
        return BOAT_SUCCESS;
    }
}