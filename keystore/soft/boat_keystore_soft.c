/*
 * @Description:
 * @Author: aitos
 * @Date: 2022-09-13 16:37:32
 * @LastEditors: zt222 tao.zheng@aitos.io
 * @LastEditTime: 2022-10-24 20:44:16
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
#include "boatiotsdk.h"
#include "boat_keystore_soft.h"
#include "boatkeypair.h"
#include "boatstorage.h"
#include "boatlog.h"
#include "bignum.h"
#include "secp256k1.h"
#include "nist256p1.h"
#include "boatplatformosal.h"
#include "persiststore.h"
#include "boat_keystore_intf.h"

#define GENERATE_KEY_REPEAT_TIMES 100

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
BOAT_RESULT BoAT_Keystore_Gen_Keypair_soft(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format, BoatKeypairKeypair *keypair)
{
    /* Valid private key value (as a UINT256) for Ethereum is [1, n-1], where n is
           0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141 */
    const BUINT32 priv_key_max_u256[8] = {0xD0364141, 0xBFD25E8C, 0xAF48A03B, 0xBAAEDCE6,
                                          0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    bignum256 priv_key_bn256;
    bignum256 priv_key_max_bn256;
    BUINT8 prikeyTmp[32];
    BUINT32 key_try_count;
    BOAT_RESULT result = BOAT_SUCCESS;

    /* Convert priv_key_max_u256 from UINT256 to Bignum256 format */
    bn_read_le((const uint8_t *)priv_key_max_u256, &priv_key_max_bn256);

    // 1- update private key
    /* generate native private key loop */
    for (key_try_count = 0; key_try_count < GENERATE_KEY_REPEAT_TIMES; key_try_count++)
    {
        /* generate native private key */
        result = BoatRandom(prikeyTmp, 32, NULL);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Fail to generate private key.");
            break;
        }

        /* Convert private key from UINT256 to Bignum256 format */
        bn_read_le(prikeyTmp, &priv_key_bn256);

        /* check the generated private key is valid or not */
        if ((bn_is_zero(&priv_key_bn256) == 0) &&
            (bn_is_less(&priv_key_bn256, &priv_key_max_bn256) != 0))
        {
            /* key is valid */
            memcpy(keypair->prikey.value, prikeyTmp, 32);
            keypair->prikey.value_len = 32;
            result = BOAT_SUCCESS;
            break;
        }
        else if (key_try_count == GENERATE_KEY_REPEAT_TIMES - 1)
        {
            result = BOAT_ERROR;
        }
    }

    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "generate private key failed.");
        return result;
    }
    result = BoAT_getPubkey(type, format, keypair->prikey.value, keypair->prikey.value_len, keypair->pubkey.value, &keypair->pubkey.value_len);

    return result;
}

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
BOAT_RESULT BoAT_sign_soft(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format, BUINT8 *prikey, BUINT32 prikeylen, const BUINT8 *digest, BUINT32 digestLen, BUINT8 *signature, BUINT32 *signatureLen, BUINT8 *Prefix)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (prikey == NULL || signature == NULL || Prefix == NULL)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1)
    {
        result = ecdsa_sign_digest(&secp256k1, // const ecdsa_curve *curve
                                   prikey,     // const uint8_t *priv_key
                                   digest,     // const uint8_t *digest
                                   signature,  // uint8_t *sig,
                                   Prefix,     // uint8_t *pby,
                                   NULL        // int (*is_canonical)(uint8_t by, uint8_t sig[64]))
        );
    }
    else if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1)
    {
        result = ecdsa_sign_digest(&nist256p1, // const ecdsa_curve *curve
                                   prikey,     // const uint8_t *priv_key
                                   digest,     // const uint8_t *digest
                                   signature,  // uint8_t *sig,
                                   Prefix,     // uint8_t *pby,
                                   NULL        // int (*is_canonical)(uint8_t by, uint8_t sig[64]))
        );
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "Unkown private key type.");
        return BOAT_ERROR_KEYPAIR_KEY_TYPE_ERR;
    }
    if (result == BOAT_SUCCESS)
    {
        *signatureLen = 64;
    }
    return result;
}

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
BOAT_RESULT BoAT_recover_pubkey_soft(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format, BUINT8 *pubkey, const BUINT8 *digest, const BUINT8 *signature, BUINT8 Prefix)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (pubkey == NULL || digest == NULL || signature == NULL)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1)
    {
        result = ecdsa_recover_pub_from_sig(&secp256k1, // const ecdsa_curve *curve
                                            pubkey,     // uint8_t *pub_key
                                            signature,  // uint8_t *sig,
                                            digest,     // const uint8_t *digest
                                            Prefix      // uint8_t *pby,
        );
    }
    else if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1)
    {
        result = ecdsa_recover_pub_from_sig(&nist256p1, // const ecdsa_curve *curve
                                            pubkey,     // uint8_t *pub_key
                                            signature,  // const uint8_t *sig
                                            digest,     // const uint8_t *digest,
                                            Prefix      // uint8_t *pby
        );
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "Unkown private key type.");
        return BOAT_ERROR_KEYPAIR_KEY_TYPE_ERR;
    }
    return result;
}

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
BOAT_RESULT BoAT_GetPrikeyByIndex_soft(BUINT8 index, BoatKeypairExtraData *prikey)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 prikeyNum = 0, prikeyIndex = 0;
    BUINT32 offset = 0;
    BUINT32 paramLength = 0, paramLengthLen = 0;
    BUINT8 lengthBytes[3] = {0};
    BUINT8 prikeynumBytes[4] = {0};
    BUINT8 ciphertext[BOAT_KEYSTORE_PRIKEY_LEN] = {0};
    BoatStoreType storetype;
    if (prikey == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "prikey must not be NULL");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if (index == 0)
    { // onetime keypair
        storetype = BOAT_STORE_TYPE_RAM;
    }
    else
    {
        storetype = BOAT_STORE_TYPE_FLASH;
    }
    result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, prikeynumBytes, sizeof(prikeynumBytes), storetype);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }
    result = utility_check_NumBytes(prikeynumBytes, &prikeyNum);
    BoatLog(BOAT_LOG_NORMAL, "prikey num = %d", prikeyNum);
    /* if read Nvram failed , no keypair */
    if (result != BOAT_SUCCESS || prikeyNum == 0)
    {
        return BOAT_ERROR;
    }
    offset += sizeof(prikeynumBytes);
    for (BUINT8 i = 0; i < prikeyNum; i++)
    {
        /* prikey index */
        result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), storetype);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "get prikey[%d] index length fail ", i);
            return result;
        }
        paramLength = UtilityGetLVData_L(lengthBytes);
        if (paramLength != 1)
        {
            BoatLog(BOAT_LOG_NORMAL, "prikey index length err ");
            return BOAT_ERROR;
        }
        paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
        offset += paramLengthLen;
        result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, &prikeyIndex, sizeof(prikeyIndex), storetype);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "get prikey[%d] index fail ", i);
            return result;
        }
        BoatLog(BOAT_LOG_NORMAL, "prikeyindex = %d , index = %d", prikeyIndex, index);
        offset += paramLength;
        if (prikeyIndex == index)
        {
            /* begin read prikey chiptext */
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), storetype);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "get prikey[%d] data length fail ", i);
                return result;
            }
            paramLength = UtilityGetLVData_L(lengthBytes);
            if (paramLength > BOAT_KEYSTORE_PRIKEY_LEN)
            {
                BoatLog(BOAT_LOG_NORMAL, "prikey data length err ");
                return BOAT_ERROR;
            }
            paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            offset += paramLengthLen;
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, ciphertext, paramLength, storetype);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "get prikey[%d] data fail ", i);
                return result;
            }
            /*  dec */
            result = BoATSoftRotNvramDec(ciphertext, paramLength, &(prikey->value[0]), &(prikey->value_len));
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "dec keypair[%d] prikey fail ", i);
                return result;
            }
            return result;
        }
        else
        {
            /* prikey data */
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), storetype);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "get prikey[%d] index length fail ", i);
                return result;
            }
            paramLength = UtilityGetLVData_L(lengthBytes);
            if (paramLength < 1)
            {
                BoatLog(BOAT_LOG_NORMAL, "prikey index length err ");
                return BOAT_ERROR;
            }
            paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            offset += (paramLengthLen + paramLength);
        }
    }
    return BOAT_ERROR;
}

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
BOAT_RESULT BoAT_Keystore_store_prikey_soft(BUINT8 keypairIndex, BUINT8 *prikey, BUINT32 prikeyLen)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 lengthBytes[3] = {0};
    BUINT8 prikeynumBytes[4] = {0};
    BUINT8 storebuf[4 + BOAT_KEYSTORE_PRIKEY_LEN] = {0};
    BUINT8 index = 0, prikeyNum = 0;
    BUINT32 offset = 0, storeOffset = 0;
    BUINT32 paramLength = 0, paramLengthLen = 0, prikeyChipLen = 0;
    BoatStoreType storeType;
    if (keypairIndex == 0)
    {
        storeType = BOAT_STORE_TYPE_RAM;
    }
    else
    {
        storeType = BOAT_STORE_TYPE_FLASH;
    }
    result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, prikeynumBytes, sizeof(prikeynumBytes), storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "read prikey num fail,ret = %d ", result);
        // return result;
        prikeyNum = 0;
    }
    else
    {
        result = utility_check_NumBytes(prikeynumBytes, &prikeyNum);
        if (result != BOAT_SUCCESS)
        {
            // BoatLog(BOAT_LOG_NORMAL,"check prikey num fail");
            // return result;
            prikeyNum = 0;
            result = BOAT_SUCCESS;
        }
    }
    BoatLog(BOAT_LOG_NORMAL, "read prikey num  = %d ", prikeyNum);
    if (prikeyNum >= BOAT_MAX_KEYPAIR_NUM)
    {
        return BOAT_ERROR;
    }
    if (BOAT_STORE_TYPE_RAM == storeType)
    {
        prikeyNum = 0;
    }
    offset += sizeof(prikeynumBytes);
    for (size_t i = 0; i < prikeyNum; i++)
    {
        /* index */
        result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), storeType);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "read prikey index lengthbytes fail ,ret = %d ", result);
            return result;
        }
        paramLength = UtilityGetLVData_L(lengthBytes);
        if (paramLength != 1)
        {
            BoatLog(BOAT_LOG_NORMAL, "prikey index length err ");
            return BOAT_ERROR;
        }
        paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
        // offset += paramLengthLen;
        result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset + paramLengthLen, &index, sizeof(index), storeType);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "read prikey index fail,ret = %d ", result);
            return result;
        }
        BoatLog(BOAT_LOG_NORMAL, "prikey index = %d , keypairIndex = %d ", index, keypairIndex);
        if (index == keypairIndex)
        {
            BoatLog(BOAT_LOG_NORMAL, "keypair index already exist ");
            return BOAT_ERROR;
        }
        offset += (paramLengthLen + paramLength);
        /* prikey data */
        result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), storeType);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "read prikey lengthbytes fail,ret = %d ", result);
            return result;
        }
        paramLength = UtilityGetLVData_L(lengthBytes);
        if (paramLength < 0)
        {
            BoatLog(BOAT_LOG_NORMAL, "prikey data length err ");
            return BOAT_ERROR;
        }
        paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
        offset += (paramLengthLen + paramLength);
    }
    storebuf[storeOffset] = keypairIndex;
    result = add_L_withOffset(storebuf, &storeOffset, 1);
    if (result < BOAT_SUCCESS)
    {
        return result;
    }
    result = BoATSoftRotNvramEnc(prikey, prikeyLen, storebuf + storeOffset, &prikeyChipLen);
    if (result != BOAT_SUCCESS || prikeyChipLen > BOAT_KEYSTORE_PRIKEY_LEN)
    {
        return BOAT_ERROR;
    }
    result = add_L_withOffset(storebuf, &storeOffset, prikeyChipLen);
    if (result < BOAT_SUCCESS)
    {
        return result;
    }
    result = BoATStoreSoftRotNvram(BOAT_STORE_PRIKEY, offset, storebuf, storeOffset, storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "store prikey data err , ret = %d ", result);
        return result;
    }
    prikeyNum++;
    offset = 0;
    utility_get_NumBytes(prikeyNum, prikeynumBytes);
    result = BoATStoreSoftRotNvram(BOAT_STORE_PRIKEY, offset, prikeynumBytes, sizeof(prikeynumBytes), storeType);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "store prikey data err ");
    }
    return result;
}

/**
 * @description:
 *  This cunction delete prikey by index.
 * @param {BUINT8} index
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 */
BOAT_RESULT BoAT_DeletePrikeyByIndex_soft(BUINT8 index)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 i = 0;
    BUINT8 prikeyNum = 0, prikeyNumNew = 0, prikeyIndex = 0;
    BUINT32 offset = 0, offset_moveFrom = 0, offset_moveTo = 0;
    BUINT32 prikeyLength = 0, paramLength = 0, paramLengthLen = 0;
    BUINT8 lengthBytes[3] = {0};
    BUINT8 prikeynumBytes[4] = {0};
    BUINT8 *prikeyData = NULL;
    boat_try_declare;
    if (index > BOAT_MAX_KEYPAIR_NUM)
    {
        return BOAT_ERROR;
    }
    /* onetime prikey
       index of onetime prikey must be 0
    */
    if (index == 0)
    {
        /* set prikey num of onetime prikey to 0 */
        memset(prikeynumBytes, 0x00, sizeof(prikeynumBytes));
        result = BoATStoreSoftRotNvram(BOAT_STORE_PRIKEY, 0, prikeynumBytes, sizeof(prikeynumBytes), BOAT_STORE_TYPE_RAM);
        return result;
    }
    else
    { // persistent prikey
        result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, prikeynumBytes, sizeof(prikeynumBytes), BOAT_STORE_TYPE_FLASH);
        /* if read Nvram failed , no keypair */
        if (result != BOAT_SUCCESS)
        {
            return result;
        }
        result = utility_check_NumBytes(prikeynumBytes, &prikeyNum);
        if (result != BOAT_SUCCESS || prikeyNum == 0)
        {
            return result;
        }
        offset += sizeof(prikeynumBytes);
        for (i = 0; i < prikeyNum; i++)
        {
            /* index */
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "read prikey index lengthbytes fail ,ret = %d ", result);
                return result;
            }
            paramLength = UtilityGetLVData_L(lengthBytes);
            if (paramLength != 1)
            {
                BoatLog(BOAT_LOG_NORMAL, "prikey index length err ");
                return BOAT_ERROR;
            }
            paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            // offset += keypairLengthLen;
            /* keypair index  */
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset + paramLengthLen, &prikeyIndex, sizeof(prikeyIndex), BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "read keypair index err ");
                return result;
            }
            if (prikeyIndex == index)
            {
                break;
            }
            offset += (paramLengthLen + paramLength);
            /* prikey data */
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "read prikey lengthbytes fail,ret = %d ", result);
                return result;
            }
            paramLength = UtilityGetLVData_L(lengthBytes);
            if (paramLength < 0)
            {
                BoatLog(BOAT_LOG_NORMAL, "prikey data length err ");
                return BOAT_ERROR;
            }
            paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            offset += (paramLengthLen + paramLength);
        }
        BoatLog(BOAT_LOG_NORMAL, "i= %d ", i);
        if (i >= prikeyNum)
        {
            BoatLog(BOAT_LOG_NORMAL, "not find the prikey ");
            return BOAT_ERROR;
        }
        prikeyNumNew = prikeyNum - 1;
        utility_get_NumBytes(prikeyNumNew, prikeynumBytes);
        result = BoATStoreSoftRotNvram(BOAT_STORE_PRIKEY, 0, prikeynumBytes, sizeof(prikeynumBytes), BOAT_STORE_TYPE_FLASH); // only need to reset keypair length bytes
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "delete keypair fail ");
            return result;
        }

        if (i == (prikeyNum - 1))
        { /* last one keypair*/
            memset(lengthBytes, 0x00, sizeof(lengthBytes));
            result = BoATStoreSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH); // only need to reset keypair length bytes
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "delete keypair fail");
                boat_throw(BOAT_ERROR, updataPrikeyData_exception);
            }
        }
        else
        {
            offset_moveTo = offset;
            // the next keypair
            offset += (paramLengthLen + paramLength);
            /* prikey content*/
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                BoatLog(BOAT_LOG_NORMAL, "read prikey lengthbytes fail,ret = %d ", result);
                boat_throw(result, updataPrikeyData_exception);
            }
            paramLength = UtilityGetLVData_L(lengthBytes);
            if (paramLength < 0)
            {
                BoatLog(BOAT_LOG_NORMAL, "prikey data length err ");
                boat_throw(BOAT_ERROR, updataPrikeyData_exception);
            }
            paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
            offset += (paramLengthLen + paramLength);
            offset_moveFrom = offset;
            i++;
            prikeyLength = 0;
            for (; i < prikeyNum; i++)
            {
                /* index */
                result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH);
                if (result != BOAT_SUCCESS)
                {
                    boat_throw(result, updataPrikeyData_exception);
                }
                paramLength = UtilityGetLVData_L(lengthBytes);
                if (paramLength < 0)
                {
                    BoatLog(BOAT_LOG_NORMAL, "keypair data length err ");
                    boat_throw(BOAT_ERROR, updataPrikeyData_exception);
                }
                paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
                offset += (paramLengthLen + paramLength);
                prikeyLength += (paramLengthLen + paramLength);
                result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset, lengthBytes, sizeof(lengthBytes), BOAT_STORE_TYPE_FLASH);
                if (result != BOAT_SUCCESS)
                {
                    boat_throw(result, updataPrikeyData_exception);
                }
                paramLength = UtilityGetLVData_L(lengthBytes);
                if (paramLength < 0)
                {
                    BoatLog(BOAT_LOG_NORMAL, "keypair data length err ");
                    boat_throw(BOAT_ERROR, updataPrikeyData_exception);
                }
                paramLengthLen = UtilityGetTLV_LL_from_len(paramLength);
                offset += (paramLengthLen + paramLength);
                prikeyLength += (paramLengthLen + paramLength);
            }
            // all the keypairs'length after index keypair
            prikeyData = BoatMalloc(prikeyLength);
            if (NULL == prikeyData)
            {
                BoatLog(BOAT_LOG_NORMAL, "fail to malloc memory ");
                boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, updataPrikeyData_exception);
            }
            result = BoatReadSoftRotNvram(BOAT_STORE_PRIKEY, offset_moveFrom, prikeyData, prikeyLength, BOAT_STORE_TYPE_FLASH);
            if (result != BOAT_SUCCESS)
            {
                BoatFree(prikeyData);
                BoatLog(BOAT_LOG_NORMAL, "read keypair data fail ");
                boat_throw(BOAT_ERROR_STORAGE_FILE_READ_FAIL, updataPrikeyData_exception);
            }
            result = BoATStoreSoftRotNvram(BOAT_STORE_PRIKEY, offset_moveTo, prikeyData, prikeyLength, BOAT_STORE_TYPE_FLASH);
            BoatFree(prikeyData);
            if (result != BOAT_SUCCESS)
            {
                /* recover keypairNum */
                boat_throw(BOAT_ERROR_STORAGE_FILE_WRITE_FAIL, updataPrikeyData_exception);
            }
        }
    }

    //! boat catch handle
    boat_catch(updataPrikeyData_exception)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
        /* recover prikeyNum */
        utility_get_NumBytes(prikeyNum, prikeynumBytes);
        offset = 0;
        result = BoATStoreSoftRotNvram(BOAT_STORE_PRIKEY, offset, prikeynumBytes, sizeof(prikeynumBytes), BOAT_STORE_TYPE_FLASH);
        if (result != BOAT_SUCCESS)
        {
            BoatLog(BOAT_LOG_NORMAL, "delete keypair fail ");
            return result;
        }
    }
    return BOAT_SUCCESS;
}
