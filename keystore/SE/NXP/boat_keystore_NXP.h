/**
****************************************************************************************
* @FilePath: boat_keystore_soft.h
* @Author: aitos
* @Date: 2022-12-22 14:47:54
* @LastEditors:
* @LastEditTime: 2022-12-22 14:47:54
* @Descripttion:
****************************************************************************************
*/
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
#ifndef __BOAT_KEYSTORE_SOFT_H__
#define __BOAT_KEYSTORE_SOFT_H__

#include "boattypes.h"

#define OBJECT_SECP256K1_ID 0xEF000000
#define OBJECT_BINARY_ID 0xEF000010
#define OBJECT_ROOT_TRUST_SIGNKEY_ID 0xF0000012
#define OBJECT_ROOT_TRUST_CERT_ID 0xF0000013
#define LEN_KEYPAIR_GEN_FLAG 0x01
#define LEN_ONBOARD_FLAG 0x01
#define OFFSET_KEYPAIR_GEN_FLAG 0x00
#define OFFSET_ONBOARD_FLAG OFFSET_KEYPAIR_GEN_FLAG + LEN_KEYPAIR_GEN_FLAG

//!@brief Boat keypair  key context
typedef struct TBoatKeystoreSoft
{
    BUINT8 keypair_index;             //!< Index of keypair
    BoatFieldVariable prikey_content; //!< Content of prikey key, native format .

} BoatKeystoreSoft;

/**
 * @description:
 *  This function write data into binary object
 * @param[in] {BUINT8} data
 *  data to be writed
 * @param[in] {BUINT32} datalen
 *  length of data
 * @param[in] {BUINT32} offset
 *  offset to be writed
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoAT_Keystore_Write_Binary(BUINT8 *data, BUINT32 datalen, BUINT32 offset);

/**
 * @description:
 *  This function read data into binary object
 * @param[in] {BUINT8} data
 *  data to be readed
 * @param[in] {BUINT32} datalen
 *  length of data
 * @param[in] {BUINT32} offset
 *  offset to be readed
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoAT_Keystore_read_Binary(BUINT8 *data, BUINT32 *datalen, BUINT32 offset);

#endif