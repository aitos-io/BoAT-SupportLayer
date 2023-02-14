/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/*!@brief Default Persistent Storage

@file
persiststore.c contains APIs for default persistent storage as a file.
*/

#include "boatinternal.h"
#include "persiststore.h"
// #include "keccak.h"
// #include "sha3.h"
#include "boatlog.h"

/* mbedTLS header include */
// #include "mbedtls/aes.h"

//!@brief Salt size for keystore
#define BOAT_STORAGE_SALT_SIZE 16
// #define BOAT_NETWORK_FILE_STOREDATA	"boat_network_file"
BUINT8 RAM_BOAT_DATA[1024] = {0};

/*!****************************************************************************
@brief Store SoftRoT context to NVRAM

@details
    This function stores any SoftRoT implementation data (e.g. activation code) to
    NVRAM. The NVRAM size varies for different hardware
    configurations, which could be obtained by calling BoatGetSoftRotNvramSize().


    Note that a writing buffer may be set up to cache the data. Flash has a limited
    endurance for erasure and writing operations. A buffer is necessary to
    aggregrate multiple writing operations into much less erasure and writing
    operations. The data in the buffer may be lost if the device is powered off
    before the data could be flushed.

@param [in] storeFile
    Select for store for keypair or network

@param [in] offset
    The offset byte in the NVRAM to write to. For example, assuming offset = 4,\n
    then the data_ptr[0] will be written to the 4th byte of the NVRAM.\n
    offset + data_len should not exceed the NVRAM size.

@param[in] data_ptr
    The data buffer to write into the NVRAM.

@param[inout] data_len
    The data length (in byte) to write into the NVRAM.
    output the data length really written into the NVRAM

@return
    This function returns 0 in a successful case.

******************************************************************************/
BOAT_RESULT BoATStoreSoftRotNvram(BoatStoreFile storeFile, BUINT32 offset, BUINT8 const *data_ptr, BUINT32 data_len, BoatStoreType storeType)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT32 offset_base = 0;
    BUINT32 rambufLen = 0;
    BUINT8 *rambuf;

    rambuf = RAM_BOAT_DATA;
    rambufLen = sizeof(RAM_BOAT_DATA);
    offset_base = BOAT_STORAGE_KEYPAIR_OFFSET;
    if (offset + data_len > BOAT_STORAGE_KEYPAIR_MAXLEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }

    if ((data_ptr == NULL) || (data_len == 0) || (storeType == BOAT_STORE_TYPE_UNKNOWN))
    {
        BoatLog(BOAT_LOG_CRITICAL, "storage_name_str or data_ptr cannot be NULL, data_len cannot be zero.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if (storeType == BOAT_STORE_TYPE_FLASH)
    {
        result = BoatWriteStorage(offset_base + offset, (BUINT8 *)data_ptr, data_len, NULL);
    }
    else if (storeType == BOAT_STORE_TYPE_RAM)
    {
        if (offset + data_len > rambufLen)
        {
            result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        else
        {
            memcpy(rambuf + offset, data_ptr, data_len);
            result = BOAT_SUCCESS;
        }
    }

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "write data fail .");
        return result;
    }
    return BOAT_SUCCESS;
}

/*!****************************************************************************
@brief Read from SoftRoT NVRAM

@details
    This function reads any SoftRoT implementation data (e.g. activation code) from
    the persistent NVRAM. The SoftRoT implementation must take care of how many data
    are stored in the NVRAM and read no more than that.

@param [in] storeFile
    Select for reading from file of keypair or network

@param [in] offset
    The offset byte in the NVRAM to read from. For example, assuming offset = 4,\n
    then the 4th byte of the NVRAM will be written to data_ptr[0].

@param[out] data_ptr
    The data buffer to copy the NVRAM content to.

@param[in] data_len
    The data length (in byte) to read from the NVRAM.


@return
    This function returns 0 in a successful case.

******************************************************************************/
BOAT_RESULT BoatReadSoftRotNvram(BoatStoreFile storeFile, BUINT32 offset, BUINT8 *data_ptr, BUINT32 len_to_read, BoatStoreType storeType)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT32 offset_base = 0;
    BUINT32 rambufLen = 0;
    BUINT8 *rambuf;

    rambuf = RAM_BOAT_DATA;
    rambufLen = sizeof(RAM_BOAT_DATA);
    offset_base = BOAT_STORAGE_KEYPAIR_OFFSET;
    if (offset + len_to_read > BOAT_STORAGE_KEYPAIR_MAXLEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }

    if ((data_ptr == NULL) || (len_to_read == 0) || storeType == BOAT_STORE_TYPE_UNKNOWN)
    {
        BoatLog(BOAT_LOG_CRITICAL, "storage_name or data cannot be NULL, data_len cannot be zero.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if (storeType == BOAT_STORE_TYPE_FLASH)
    {
        result = BoatReadStorage(offset_base + offset, data_ptr, len_to_read, NULL);
    }
    else
    {
        if (offset + len_to_read > rambufLen)
        {
            result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        else
        {
            memcpy(data_ptr, rambuf + offset, len_to_read);
            result = BOAT_SUCCESS;
        }
    }

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, " read file  fail ,ret = %d \r\n", result);
        return result;
    }

    return BOAT_SUCCESS;
}
