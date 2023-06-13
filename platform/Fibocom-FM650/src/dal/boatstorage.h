/**
****************************************************************************************
* @FilePath: boat_storage.h
* @Author: aitos
* @Date: 2023-02-15 10:03:32
* @LastEditors:
* @LastEditTime: 2023-02-15 10:03:32
* @Descripttion:
****************************************************************************************
*/
#ifndef _BOAT_STORAGE_H_
#define _BOAT_STORAGE_H_

#define BOAT_KEYSTORE_PRIKEY_NUM_LEN 4                 // AA 55 NUM CRC
#define BOAT_KEYSTORE_PRIKEY_LEN 1 + 16 + 32 + 32 + 16 // L : 1byte , 16: 16bytes random , 32 : hash , 32 : prikey ,16 : padding
#define BOAT_KEYSTORE_PRIKEYINDEX_LEN 2                // 01+INDEX

#define BOAT_FILE_STOREDATA "boat_wallet_file"
#define BOAT_STORAGE_KEYPAIR_OFFSET 0
#define BOAT_STORAGE_KEYPAIR_MAXLEN BOAT_KEYPAIR_NUM_LEN + BOAT_KEYPAIR_ALL_LEN_MAX +      \
                                        BOAT_MAX_KEYPAIR_NUM *(BOAT_KEYPAIR_INDEX_LEN +    \
                                                               BOAT_KEYPAIR_NAME_LEN_MAX + \
                                                               BOAT_KEYPAIR_FORMAT_LEN +   \
                                                               BOAT_KEYPAIR_TYPE_LEN +     \
                                                               BOAT_KEYPAIR_PUBKEY_LEN)

#define BOAT_STORAGE_PRIKEY_OFFSET BOAT_STORAGE_KEYPAIR_OFFSET + BOAT_STORAGE_KEYPAIR_MAXLEN + 128 // reserved 128 bytes
#define BOAT_STORAGE_PRIKEY_MAXLEN BOAT_KEYSTORE_PRIKEY_NUM_LEN + BOAT_MAX_KEYPAIR_NUM *(BOAT_KEYSTORE_PRIKEYINDEX_LEN + BOAT_KEYSTORE_PRIKEY_LEN)

#define BOAT_STORAGE_NETWORK_OFFSET BOAT_STORAGE_PRIKEY_OFFSET + BOAT_STORAGE_PRIKEY_MAXLEN + 128 // reserved 128 bytes

BOAT_RESULT BoatGetStorageSize(BUINT32 *size, void *rsvd);
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
BOAT_RESULT BoatWriteStorage(BUINT32 offset, BUINT8 *writeBuf, BUINT32 writeLen, void *rsvd);

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
BOAT_RESULT BoatReadStorage(BUINT32 offset, BUINT8 *readBuf, BUINT32 readLen, void *rsvd);

BOAT_RESULT BoatRemoveFile(const BCHAR *fileName, void *rsvd);

#endif