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
#include "boat_keystore_se.h"
#include "boatkeypair.h"
#include "persiststore.h"
#include "se05x_tlv.h"
#include "se_process.h"
#include "se05x_APDU_impl.h"

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
BOAT_RESULT BoAT_Keystore_Write_Binary_se(BUINT8 *data, BUINT32 datalen, BUINT32 offset)
{
    BOAT_RESULT ret = BOAT_SUCCESS;
    smStatus_t sw = SM_OK;
    BUINT8 apdu_select[] = {0x00, 0xA4, 0x04, 0x00, 0x10, 0xA0, 0x00, 0x00, 0x03, 0x96, 0x54, 0x53, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00};
    BUINT8 res[64] = {0};
    BUINT32 reslen = sizeof(res);
    Se05xPolicy_t policy;
    boat_try_declare;
    policy.value = NULL;
    policy.value_len = 0;
    ret = boat_open_se();
    if (ret != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "boat_open_se fail  %d\n", ret);
        return ret;
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "boat_open_se  OK \n");
    }

    ret = boat_send_apdu(apdu_select, sizeof(apdu_select), res, &reslen, &sw);
    if (ret != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "boat_send_apdu fail  %d\n", ret);
        boat_throw(BOAT_ERROR, binary_write_exception);
    }// LCOV_EXCL_STOP
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "boat_send_apdu  OK \n");
        BoatLog_hexdump(BOAT_LOG_NORMAL, " select res : ", res, reslen);
        BoatLog(BOAT_LOG_NORMAL, "select return sw : %x \n", sw);
    }

    sw = Se05x_API_WriteBinary(&policy, OBJECT_BINARY_ID, offset, 0, data, datalen);
    if (sw != SM_OK)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "write binary data fail, sw : %x \n", sw);
        boat_throw(BOAT_ERROR, binary_write_exception);
    }// LCOV_EXCL_STOP
    boat_catch(binary_write_exception)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        ret = boat_exception;
    }
    boat_se_close();
    return ret;
}

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
BOAT_RESULT BoAT_Keystore_read_Binary_se(BUINT8 *data, BUINT32 *datalen, BUINT32 offset)
{
    BOAT_RESULT ret = BOAT_SUCCESS;
    smStatus_t sw = SM_OK;
    BUINT8 apdu_select[] = {0x00, 0xA4, 0x04, 0x00, 0x10, 0xA0, 0x00, 0x00, 0x03, 0x96, 0x54, 0x53, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00};
    BUINT8 res[64] = {0};
    BUINT32 reslen = sizeof(res);
    boat_try_declare;
    ret = boat_open_se();
    if (ret != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "boat_open_se fail  %d\n", ret);
        return ret;
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "boat_open_se  OK \n");
    }

    ret = boat_send_apdu(apdu_select, sizeof(apdu_select), res, &reslen, &sw);
    if (ret != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "boat_send_apdu fail  %d\n", ret);
        boat_throw(BOAT_ERROR, binary_write_exception);
    }// LCOV_EXCL_STOP
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "boat_send_apdu  OK \n");
        BoatLog_hexdump(BOAT_LOG_NORMAL, " select res : ", res, reslen);
        BoatLog(BOAT_LOG_NORMAL, "select return sw : %x \n", sw);
    }

    sw = Se05x_API_ReadObject(OBJECT_BINARY_ID, offset, 0, data, datalen);
    if (sw != SM_OK)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "write binary data fail, sw : %x \n", sw);
        boat_throw(BOAT_ERROR, binary_write_exception);
    }// LCOV_EXCL_STOP
    // sw = Se05x_API_DeleteSecureObject(OBJECT_SECP256K1_ID);
    // BoatLog(BOAT_LOG_NORMAL, "boat delete ECkey  sw : %x \n", sw);
    boat_catch(binary_write_exception)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        ret = boat_exception;
    }
    boat_se_close();
    return ret;
}

/**
 * @description:
 *  This function gen keypair through software
 * @param[in] {BoatKeypairPriKeyType} type
 * @param[out] {BoatKeypairKeypair} *keypair
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoAT_Keystore_Gen_Keypair_se(BoatKeypairPriKeyType type, BoatKeypairKeypair *keypair)
{
    // return BoAT_Keypair_generation(type, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE, keypair);
    BOAT_RESULT ret = BOAT_SUCCESS;
    smStatus_t sw = SM_OK;
    BUINT8 apdu_select[] = {0x00, 0xA4, 0x04, 0x00, 0x10, 0xA0, 0x00, 0x00, 0x03, 0x96, 0x54, 0x53, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00};
    BUINT8 res[64] = {0};
    BUINT8 keypairGenFlag[1] = {0x55};
    BUINT32 reslen = sizeof(res);
    BUINT32 i = 0;
    Se05xPolicy_t policy;
    boat_try_declare;
    policy.value = NULL;
    policy.value_len = 0;
    ret = boat_open_se();
    if (ret != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "boat_open_se fail  %d\n", ret);
        return ret;
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "boat_open_se  OK \n");
    }

    ret = boat_send_apdu(apdu_select, sizeof(apdu_select), res, &reslen, &sw);
    if (ret != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "boat_send_apdu fail  %d\n", ret);
        boat_throw(BOAT_ERROR, keypair_gen_exception);
    }// LCOV_EXCL_STOP
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "boat_send_apdu  OK \n");
        BoatLog_hexdump(BOAT_LOG_NORMAL, " select res : ", res, reslen);
        BoatLog(BOAT_LOG_NORMAL, "select return sw : %x \n", sw);
    }
    /* check ECCurve of secp256K1 is existed or not*/
    BUINT8 ECCurveList[24] = {0};
    BUINT32 ECCurveListLen = sizeof(ECCurveList);
    sw = Se05x_API_ReadECCurveList(ECCurveList, &ECCurveListLen);
    if (sw != SM_OK)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "read ECCurveList  fail %x\n", sw);
        boat_throw(BOAT_ERROR, keypair_gen_exception);
    }// LCOV_EXCL_STOP
    for (i = 0; i < ECCurveListLen; i++)
    {
        /* ECCurve ID of secp256k1 equal 0x10 */
        if (ECCurveList[i] == 0x10)
        {
            break;
        }
    }
    /* have no secp256k1 in the se*/
    if (i > ECCurveListLen)
    {
        /* creat ECCurve of secp256k1 */
        const BUINT8 ecc_prime[] = {EC_PARAM_secp256k1_prime};
        const BUINT8 ecc_a[] = {EC_PARAM_secp256k1_a};
        const BUINT8 ecc_b[] = {EC_PARAM_secp256k1_b};
        const BUINT8 ecc_G[] = {0x04, EC_PARAM_secp256k1_x, EC_PARAM_secp256k1_y};
        const BUINT8 ecc_ordern[] = {EC_PARAM_secp256k1_order};
        sw = Se05x_API_CreateECCurve(kSE05x_ECCurve_Secp256k1);
        if (sw != SM_OK)
        {// LCOV_EXCL_START
            BoatLog(BOAT_LOG_NORMAL, "CreateECCurve fail, sw : %x \n", sw);
            boat_throw(BOAT_ERROR, keypair_gen_exception);
        }// LCOV_EXCL_STOP

        sw = Se05x_API_SetECCurveParam(kSE05x_ECCurve_Secp256k1, kSE05x_ECCurveParam_PARAM_A, ecc_a, sizeof(ecc_a));
        if (sw != SM_OK)
        {// LCOV_EXCL_START
            BoatLog(BOAT_LOG_NORMAL, "SetECCurveParam PARAM_A fail, sw : %x \n", sw);
            boat_throw(BOAT_ERROR, keypair_gen_exception);
        }// LCOV_EXCL_STOP

        sw = Se05x_API_SetECCurveParam(kSE05x_ECCurve_Secp256k1, kSE05x_ECCurveParam_PARAM_B, ecc_b, sizeof(ecc_b));
        if (sw != SM_OK)
        {// LCOV_EXCL_START
            BoatLog(BOAT_LOG_NORMAL, "SetECCurveParam PARAM_B fail, sw : %x \n", sw);
            boat_throw(BOAT_ERROR, keypair_gen_exception);
        }// LCOV_EXCL_STOP
        sw = Se05x_API_SetECCurveParam(kSE05x_ECCurve_Secp256k1, kSE05x_ECCurveParam_PARAM_G, ecc_G, sizeof(ecc_G));
        if (sw != SM_OK)
        {// LCOV_EXCL_START
            BoatLog(BOAT_LOG_NORMAL, "SetECCurveParam PARAM_G fail, sw : %x \n", sw);
            boat_throw(BOAT_ERROR, keypair_gen_exception);
        }// LCOV_EXCL_STOP
        sw = Se05x_API_SetECCurveParam(kSE05x_ECCurve_Secp256k1, kSE05x_ECCurveParam_PARAM_N, ecc_ordern, sizeof(ecc_ordern));
        if (sw != SM_OK)
        {// LCOV_EXCL_START
            BoatLog(BOAT_LOG_NORMAL, "SetECCurveParam PARAM_N fail, sw : %x \n", sw);
            boat_throw(BOAT_ERROR, keypair_gen_exception);
        }// LCOV_EXCL_STOP
        sw = Se05x_API_SetECCurveParam(kSE05x_ECCurve_Secp256k1, kSE05x_ECCurveParam_PARAM_PRIME, ecc_prime, sizeof(ecc_prime));
        if (sw != SM_OK)
        {// LCOV_EXCL_START
            BoatLog(BOAT_LOG_NORMAL, "SetECCurveParam PARAM_PRIME fail, sw : %x \n", sw);
            boat_throw(BOAT_ERROR, keypair_gen_exception);
        }// LCOV_EXCL_STOP
    }

    sw = Se05x_API_WriteECKey(&policy, 0, OBJECT_SECP256K1_ID, kSE05x_ECCurve_Secp256k1, NULL, 0, NULL, 0, kSE05x_INS_NA, kSE05x_KeyPart_Pair);
    if (sw != SM_OK)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Write ECkey fail, sw : %x \n", sw);
        boat_throw(BOAT_ERROR, keypair_gen_exception);
    }// LCOV_EXCL_STOP
    keypair->pubkey.value_len = sizeof(keypair->pubkey.value);
    sw = Se05x_API_ReadObject(OBJECT_SECP256K1_ID, 0, 0, keypair->pubkey.value, &keypair->pubkey.value_len);
    if (sw != SM_OK)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "read ECkey fail, sw : %x \n", sw);
        boat_throw(BOAT_ERROR, keypair_gen_exception);
    }// LCOV_EXCL_STOP
    // sw = Se05x_API_WriteBinary(&policy, OBJECT_BINARY_ID, OFFSET_KEYPAIR_GEN_FLAG, 0, keypairGenFlag, sizeof(keypairGenFlag));
    // if (sw != SM_OK)
    // {
    //     BoatLog(BOAT_LOG_NORMAL, "write binary data fail, sw : %x \n", sw);
    //     boat_throw(BOAT_ERROR, keypair_gen_exception);
    // }
    boat_catch(keypair_gen_exception)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        ret = boat_exception;
    }
    boat_se_close();
    return ret;
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
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoAT_Keystore_Sign_se(BoatKeypairPriKeyType type, BUINT8 prikeyIndex, const BUINT8 *digest, BUINT32 digestLen, BUINT8 *signature, BUINT32 *signatureLen, BUINT8 *Prefix)
{
    BOAT_RESULT ret = BOAT_SUCCESS;
    smStatus_t sw = SM_OK;
    BUINT8 apdu_select[] = {0x00, 0xA4, 0x04, 0x00, 0x10, 0xA0, 0x00, 0x00, 0x03, 0x96, 0x54, 0x53, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00};
    BUINT8 res[64] = {0};
    BUINT32 reslen = sizeof(res);
    boat_try_declare;
    ret = boat_open_se();
    if (ret != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "boat_open_se fail  %d\n", ret);
        return ret;
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "boat_open_se  OK \n");
    }

    ret = boat_send_apdu(apdu_select, sizeof(apdu_select), res, &reslen, &sw);
    if (ret != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "boat_send_apdu fail  %d\n", ret);
        boat_throw(BOAT_ERROR, keypair_sign_exception);
    }// LCOV_EXCL_STOP
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "boat_send_apdu  OK \n");
    }
    sw = Se05x_API_ECDSASign(OBJECT_SECP256K1_ID, kSE05x_ECSignatureAlgo_SHA_256, digest, digestLen, signature, signatureLen);
    if (sw != SM_OK)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "ECDSA sign fail, sw : %x \n", sw);
        boat_throw(BOAT_ERROR, keypair_sign_exception);
    }// LCOV_EXCL_STOP
    BoatLog_hexdump(BOAT_LOG_NORMAL, "signresult : ", signature, *signatureLen);
    *Prefix = 0x03;
    boat_catch(keypair_sign_exception)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        ret = boat_exception;
    }
    boat_se_close();
    return ret;
}
