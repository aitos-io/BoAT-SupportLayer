/*
 * @Description:
 * @Author: aitos
 * @Date: 2022-09-13 16:39:32
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-13 18:03:45
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

#define BOAT_KEYSTORE_SOFT_OFFSET

//!@brief Boat keypair  key context
typedef struct TBoatKeystoreSoft
{
    BUINT8 keypair_index;             //!< Index of keypair
    BoatFieldVariable prikey_content; //!< Content of prikey key, native format .

} BoatKeystoreSoft;

/**
 * @description:
 * 	This function gen keypair .
 * @param {BoatKeypairPriKeyType} type
 * 	now only support ecdsa and will support other alg such as SM
 * @param {BoatKeypairPriKeyFormat} format
 * 	support native and pkcs
 * @param {BoatKeypairKeypair} *keypair
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoAT_Keystore_Gen_Keypair_soft(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format, BoatKeypairKeypair *keypair);

/**
 * @description:
 * 	This function gen signature by digest.
 * @param[in] {BoatKeypairPriKeyType} type
 * 	support ecdsa now.
 * @param[in] {BUINT8} *prikey
 * 	private key
 * @param[in] {BUINT32} prikeylen
 * 	length of prikey
 * @param[in] {BUINT8*} digest
 * @param[in] {BUINT32} digestLen
 * @param[out] {BUINT8 *} signature
 * @param[out] {BUINT32} *signatureLen
 * @param[out] {BUINT8} *Prefix
 * 	recovery factor getting public key from signature. The value must bu 0 or 1.
 * @return {*}
 * @author: aitos
 */
BOAT_RESULT BoAT_sign_soft(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format, BUINT8 *prikey, BUINT32 prikeylen, const BUINT8 *digest, BUINT32 digestLen, BUINT8 *signature, BUINT32 *signatureLen, BUINT8 *Prefix);

/**
 * @description:
 * 	This function get prikey by index from Nvram.
 * @param[in] {BUINT8} index
 * @param[out] {BoatKeypairExtraData} *prikey
 * @return {*}
 *   This function returns BOAT_SUCCESS if success.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoAT_GetPrikeyByIndex_soft(BUINT8 index, BoatKeypairExtraData *prikey);

/**
 * @description:
 *  This function store prikey data. Prikey should bind with keypair
 * @param[in] {BUINT8} keypairIndex
 *  current index of keypair
 * @param[in] {BUINT8} *prikey
 *  prikey data
 * @param[in] {BUINT32} prikeyLen
 *  length of prikey
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoAT_Keystore_store_prikey_soft(BUINT8 keypairIndex, BUINT8 *prikey, BUINT32 prikeyLen);
/**
 * @description:
 *  This cunction delete prikey by index.
 * @param {BUINT8} index
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 */
BOAT_RESULT BoAT_DeletePrikeyByIndex_soft(BUINT8 index);

/**
****************************************************************************************
* @brief:
*  This function recover public key from signature and digest.
* @param[in] type
*  secp256K1 or secp256r1
* @param[in] format
*  unused
* @param[in] *pubkey
*  public key recovered
* @param[in] *digest
*  digest of sign data
* @param[in] *signature
* @param[in] Prefix
* recovery factor getting public key from signature. The value must bu 0 or 1.
* @return
*  This function returns BoAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
*
****************************************************************************************
*/
BOAT_RESULT BoAT_recover_pubkey_soft(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format, BUINT8 *pubkey, const BUINT8 *digest, const BUINT8 *signature, BUINT8 Prefix);

#endif