/**
****************************************************************************************
* @FilePath: se05x_APDU_impl.h
* @Author: aitos
* @Date: 2022-11-22 17:27:00
* @LastEditors:
* @LastEditTime: 2022-11-22 17:27:00
* @Descripttion:
****************************************************************************************
*/
/*
 *
 * Copyright 2019-2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "se05x_APDU_impl.h"

smStatus_t Se05x_API_CreateSession(BUINT32 authObjectID, BUINT8 *sessionId, BUINT32 *psessionIdLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_SESSION_CREATE}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_U32("auth", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, authObjectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, sessionId, psessionIdLen); /*  */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_ExchangeSessionData(pSe05xPolicy_t policy)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_SESSION_POLICY}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    //    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_Se05xPolicy("Policy", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, policy);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_RefreshSession(pSe05xPolicy_t policy)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_SESSION_REFRESH}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;

    tlvRet = TLVSET_Se05xPolicy("policy", &pCmdbuf, &cmdbufLen, kSE05x_TAG_POLICY, policy);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

// smStatus_t Se05x_API_CloseSession(pSe05xSession_t session_ctx)
// {
//     smStatus_t retStatus = SM_NOT_OK;
//     tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_SESSION_CLOSE}};
//     BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
//     BUINT32 cmdbufLen = 0;
//     BUINT8 iCnt = 0;

// #if VERBOSE_APDU_LOGS
//     NEWLINE();
//     nLog("APDU", NX_LEVEL_DEBUG, "CloseSession []");
// #endif /* VERBOSE_APDU_LOGS */
//     if (((session_ctx->value[0] || session_ctx->value[1] || session_ctx->value[2] || session_ctx->value[3] ||
//           session_ctx->value[4] || session_ctx->value[5] || session_ctx->value[6] || session_ctx->value[7])) &&
//         (session_ctx->hasSession == 1))
//     {
//         retStatus = DoAPDUTx_s_Case3(session_ctx, &hdr, cmdbuf, cmdbufLen);
//         if (retStatus == SM_OK)
//         {
//             for (iCnt = 0; iCnt < 8; iCnt++)
//             {
//                 session_ctx->value[iCnt] = 0;
//             }
//             session_ctx->hasSession = 0;
//         }
//     }
//     else
//     {
//         LOG_D("CloseSession command is sent only if valid Session exists!!!");
//     }
//     return retStatus;
// }

smStatus_t Se05x_API_WriteECKey(pSe05xPolicy_t policy,
                                SE05x_MaxAttemps_t maxAttempt,
                                BUINT32 objectID,
                                SE05x_ECCurve_t curveID,
                                const BUINT8 *privKey,
                                BUINT32 privKeyLen,
                                const BUINT8 *pubKey,
                                BUINT32 pubKeyLen,
                                const SE05x_INS_t ins_type,
                                const SE05x_KeyPart_t key_part)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_WRITE | ins_type, kSE05x_P1_EC | key_part, kSE05x_P2_DEFAULT}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;

    tlvRet = TLVSET_Se05xPolicy("policy", &pCmdbuf, &cmdbufLen, kSE05x_TAG_POLICY, policy);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_MaxAttemps("maxAttempt", &pCmdbuf, &cmdbufLen, kSE05x_TAG_MAX_ATTEMPTS, maxAttempt);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_ECCurve("curveID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, curveID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("privKey", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, privKey, privKeyLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("pubKey", &pCmdbuf, &cmdbufLen, kSE05x_TAG_4, pubKey, pubKeyLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_WriteSymmKey(pSe05xPolicy_t policy,
                                  SE05x_MaxAttemps_t maxAttempt,
                                  BUINT32 objectID,
                                  SE05x_KeyID_t kekID,
                                  const BUINT8 *keyValue,
                                  BUINT32 keyValueLen,
                                  const SE05x_INS_t ins_type,
                                  const SE05x_SymmKeyType_t type)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_WRITE | ins_type, type, kSE05x_P2_DEFAULT}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;

    tlvRet = TLVSET_Se05xPolicy("policy", &pCmdbuf, &cmdbufLen, kSE05x_TAG_POLICY, policy);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_MaxAttemps("maxAttempt", &pCmdbuf, &cmdbufLen, kSE05x_TAG_MAX_ATTEMPTS, maxAttempt);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_KeyID("KEK id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, kekID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("key value", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, keyValue, keyValueLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_WriteBinary(pSe05xPolicy_t policy,
                                 BUINT32 objectID,
                                 BUINT16 offset,
                                 BUINT16 length,
                                 const BUINT8 *inputData,
                                 BUINT32 inputDataLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_WRITE, kSE05x_P1_BINARY, kSE05x_P2_DEFAULT}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;

    tlvRet = TLVSET_Se05xPolicy("policy", &pCmdbuf, &cmdbufLen, kSE05x_TAG_POLICY, policy);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_U16Optional("offset", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, offset);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_U16Optional("length", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, length);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("input data", &pCmdbuf, &cmdbufLen, kSE05x_TAG_4, inputData, inputDataLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_WriteUserID(pSe05xPolicy_t policy,
                                 SE05x_MaxAttemps_t maxAttempt,
                                 BUINT32 objectID,
                                 const BUINT8 *userId,
                                 BUINT32 userIdLen,
                                 const SE05x_AttestationType_t attestation_type)
{
    smStatus_t retStatus = SM_NOT_OK;

    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_WRITE | attestation_type, kSE05x_P1_UserID, kSE05x_P2_DEFAULT}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;

    tlvRet = TLVSET_Se05xPolicy("policy", &pCmdbuf, &cmdbufLen, kSE05x_TAG_POLICY, policy);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_MaxAttemps("maxAttempt", &pCmdbuf, &cmdbufLen, kSE05x_TAG_MAX_ATTEMPTS, maxAttempt);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("userId", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, userId, userIdLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_ReadType(BUINT32 objectID,
                              SE05x_SecureObjectType_t *ptype,
                              BUINT8 *pisTransient,
                              const SE05x_AttestationType_t attestation_type)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_READ | attestation_type, kSE05x_P1_DEFAULT, kSE05x_P2_TYPE}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_SecureObjectType(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, ptype); /* - */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        tlvRet = tlvGet_U8(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_2, pisTransient); /* - */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_ReadSize(BUINT32 objectID, BUINT16 *psize)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_READ, kSE05x_P1_DEFAULT, kSE05x_P2_SIZE}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_U16(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, psize); /* - */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_ReadIDList(BUINT16 outputOffset,
                                BUINT8 filter,
                                BUINT8 *pmore,
                                BUINT8 *idlist,
                                BUINT32 *pidlistLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_READ, kSE05x_P1_DEFAULT, kSE05x_P2_LIST}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_U16("output offset", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, outputOffset);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_U8("filter", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, filter);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4_ext(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_U8(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, pmore); /* - */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        tlvRet = tlvGet_u8buf(pRspbuf,
                              &rspIndex,
                              rspbufLen,
                              kSE05x_TAG_2,
                              idlist,
                              pidlistLen); /* Byte array containing 4-byte identifiers */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_CheckObjectExists(BUINT32 objectID, SE05x_Result_t *presult)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_EXIST}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_Result(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, presult); /* - */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_DeleteSecureObject(BUINT32 objectID)
{
    smStatus_t retStatus = SM_NOT_OK;

    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_DELETE_OBJECT}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_CreateECCurve(SE05x_ECCurve_t curveID)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_WRITE, kSE05x_P1_CURVE, kSE05x_P2_CREATE}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    tlvRet = TLVSET_ECCurve("curve id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, curveID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_SetECCurveParam(SE05x_ECCurve_t curveID,
                                     SE05x_ECCurveParam_t ecCurveParam,
                                     const BUINT8 *inputData,
                                     BUINT32 inputDataLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_WRITE, kSE05x_P1_CURVE, kSE05x_P2_PARAM}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;

    tlvRet = TLVSET_ECCurve("curve id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, curveID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_ECCurveParam("ecCurveParam", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, ecCurveParam);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_GetECCurveId(BUINT32 objectID, BUINT8 *pcurveId)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_READ, kSE05x_P1_CURVE, kSE05x_P2_ID}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_U8(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, pcurveId); /* - */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_ReadECCurveList(BUINT8 *curveList, BUINT32 *pcurveListLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_READ, kSE05x_P1_CURVE, kSE05x_P2_LIST}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    retStatus = DoAPDUTxRx_s_Case4(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, curveList, pcurveListLen); /*  */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_DeleteECCurve(SE05x_ECCurve_t curveID)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_CURVE, kSE05x_P2_DELETE_OBJECT}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;

    tlvRet = TLVSET_ECCurve("curve id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, curveID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_CreateCryptoObject(SE05x_CryptoObjectID_t cryptoObjectID,
                                        SE05x_CryptoContext_t cryptoContext,
                                        SE05x_CryptoModeSubType_t subtype)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_WRITE, kSE05x_P1_CRYPTO_OBJ, kSE05x_P2_DEFAULT}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;

    tlvRet = TLVSET_CryptoObjectID("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, cryptoObjectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_CryptoContext("cryptoContext", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cryptoContext);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_CryptoModeSubType(
        "1-byte Crypto Object subtype, either from DigestMode, CipherMode or MACAlgo (depending on TAG_2).",
        &pCmdbuf,
        &cmdbufLen,
        kSE05x_TAG_3,
        subtype);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_ReadObject(BUINT32 objectID, BUINT16 offset, BUINT16 length, BUINT8 *data, BUINT32 *pdataLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_READ, kSE05x_P1_DEFAULT, kSE05x_P2_DEFAULT}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);
    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_U16Optional("offset", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, offset);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_U16Optional("length", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, length);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4_ext(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, data, pdataLen); /*  */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

    if (retStatus == SM_ERR_COMMAND_NOT_ALLOWED)
    {
        BoatLog(BOAT_LOG_NORMAL, "Denied to read object %08X bases on policy.", objectID);
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_ReadCryptoObjectList(BUINT8 *idlist, BUINT32 *pidlistLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_READ, kSE05x_P1_CRYPTO_OBJ, kSE05x_P2_LIST}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    retStatus = DoAPDUTxRx_s_Case2(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet =
            tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, idlist, pidlistLen); /* If more ids are present */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_DeleteCryptoObject(SE05x_CryptoObjectID_t cryptoObjectID)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_CRYPTO_OBJ, kSE05x_P2_DELETE_OBJECT}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;

    tlvRet = TLVSET_CryptoObjectID("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, cryptoObjectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_ECDSASign(BUINT32 objectID,
                               SE05x_ECSignatureAlgo_t ecSignAlgo,
                               const BUINT8 *inputData,
                               BUINT32 inputDataLen,
                               BUINT8 *signature,
                               BUINT32 *psignatureLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_SIGNATURE, kSE05x_P2_SIGN}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);
    BUINT8 signresult[74] = {0};
    BUINT32 signresultLen = sizeof(signresult);
    BUINT8 offset = 0;

    tlvRet = TLVSET_U32("objectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_ECSignatureAlgo("ecSignAlgo", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, ecSignAlgo);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, signresult, &signresultLen); /*  */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
        offset = 3;
        if (signresult[offset] == 0x21)
        {
            offset += 2;
            memcpy(signature, signresult + offset, 0x20);
        }
        else
        {
            offset += 1;
            memcpy(signature, signresult + offset, 0x20);
        }
        offset += 0x20;
        offset++;
        if (signresult[offset] == 0x21)
        {
            offset += 2;
            memcpy(signature + 0x20, signresult + offset, 0x20);
        }
        else
        {
            offset += 1;
            memcpy(signature + 0x20, signresult + offset, 0x20);
        }
        *psignatureLen = 64;
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_EdDSASign(BUINT32 objectID,
                               SE05x_EDSignatureAlgo_t edSignAlgo,
                               const BUINT8 *inputData,
                               BUINT32 inputDataLen,
                               BUINT8 *signature,
                               BUINT32 *psignatureLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_SIGNATURE, kSE05x_P2_SIGN}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_U32("objectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_EDSignatureAlgo("edSignAlgo", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, edSignAlgo);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, signature, psignatureLen); /*  */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_ECDSAVerify(BUINT32 objectID,
                                 SE05x_ECSignatureAlgo_t ecSignAlgo,
                                 const BUINT8 *inputData,
                                 BUINT32 inputDataLen,
                                 const BUINT8 *signature,
                                 BUINT32 signatureLen,
                                 SE05x_Result_t *presult)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_SIGNATURE, kSE05x_P2_VERIFY}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_U32("objectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_ECSignatureAlgo("ecSignAlgo", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, ecSignAlgo);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("signature", &pCmdbuf, &cmdbufLen, kSE05x_TAG_5, signature, signatureLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_Result(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, presult); /* - */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_EdDSAVerify(BUINT32 objectID,
                                 SE05x_EDSignatureAlgo_t edSignAlgo,
                                 const BUINT8 *inputData,
                                 BUINT32 inputDataLen,
                                 const BUINT8 *signature,
                                 BUINT32 signatureLen,
                                 SE05x_Result_t *presult)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_SIGNATURE, kSE05x_P2_VERIFY}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_U32("objectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_EDSignatureAlgo("edSignAlgo", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, edSignAlgo);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("signature", &pCmdbuf, &cmdbufLen, kSE05x_TAG_5, signature, signatureLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_Result(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, presult); /* - */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_CipherInit(BUINT32 objectID,
                                SE05x_CryptoObjectID_t cryptoObjectID,
                                BUINT8 *IV,
                                BUINT32 IVLen,
                                const SE05x_Cipher_Oper_t operation)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_CIPHER, operation}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);
    BUINT16 ivlen16 = (BUINT16)IVLen;
    BUINT32 ivlen32 = IVLen;

    tlvRet = TLVSET_U32("objectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_CryptoObjectID("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cryptoObjectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    // if AES_CTR_INT_IV and Encrypt then TAG 4 is IVLen
    if ((cryptoObjectID == kSE05x_CryptoObject_AES_CTR_INT_IV) && (operation == kSE05x_Cipher_Oper_Encrypt))
    {
        tlvRet = TLVSET_U16("IVLen", &pCmdbuf, &cmdbufLen, kSE05x_TAG_4, ivlen16);
        if (0 != tlvRet)
        {
            goto cleanup;
        }
    }
    else
    {
        tlvRet = TLVSET_u8bufOptional("IV", &pCmdbuf, &cmdbufLen, kSE05x_TAG_4, IV, IVLen);
        if (0 != tlvRet)
        {
            goto cleanup;
        }
    }

    if ((cryptoObjectID == kSE05x_CryptoObject_AES_CTR_INT_IV) && (operation == kSE05x_Cipher_Oper_Encrypt))
    {
        retStatus = DoAPDUTxRx_s_Case4_ext(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
        if (retStatus == SM_OK)
        {
            retStatus = SM_NOT_OK;
            BUINT32 rspIndex = 0;

            // Return IV when doing encrypt and using internal IV.
            tlvRet = tlvGet_u8buf(pRspbuf,
                                  &rspIndex,
                                  rspbufLen,
                                  kSE05x_TAG_3,
                                  IV,
                                  &ivlen32); /* Byte array containing initialization vector */
            if ((0 != tlvRet) || (ivlen32 != 16))
            {
                // initialization vector can only be 16 bytes.
                goto cleanup;
            }
            if ((rspIndex + 2) == rspbufLen)
            {
                retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
            }
        }
    }
    else
    {
        retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_CipherUpdate(SE05x_CryptoObjectID_t cryptoObjectID,
                                  const BUINT8 *inputData,
                                  BUINT32 inputDataLen,
                                  BUINT8 *outputData,
                                  BUINT32 *poutputDataLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_CIPHER, kSE05x_P2_UPDATE}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_CryptoObjectID("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cryptoObjectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, outputData, poutputDataLen); /*  */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_CipherFinal(SE05x_CryptoObjectID_t cryptoObjectID,
                                 const BUINT8 *inputData,
                                 BUINT32 inputDataLen,
                                 BUINT8 *outputData,
                                 BUINT32 *poutputDataLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_CIPHER, kSE05x_P2_FINAL}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_CryptoObjectID("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cryptoObjectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8buf("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4_ext(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, outputData, poutputDataLen); /*  */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_MACInit(BUINT32 objectID,
                             SE05x_CryptoObjectID_t cryptoObjectID,
                             const SE05x_Mac_Oper_t mac_oper)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_MAC, mac_oper}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;

    tlvRet = TLVSET_U32("objectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_CryptoObjectID("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cryptoObjectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_MACUpdate(
    const BUINT8 *inputData, BUINT32 inputDataLen, SE05x_CryptoObjectID_t cryptoObjectID)
{
    smStatus_t retStatus = SM_NOT_OK;

    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_MAC, kSE05x_P2_UPDATE}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;

    tlvRet = TLVSET_u8bufOptional("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, inputData, inputDataLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_CryptoObjectID("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cryptoObjectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_MACFinal(const BUINT8 *inputData,
                              BUINT32 inputDataLen,
                              SE05x_CryptoObjectID_t cryptoObjectID,
                              const BUINT8 *macValidateData,
                              BUINT32 macValidateDataLen,
                              BUINT8 *macValue,
                              BUINT32 *pmacValueLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_MAC, kSE05x_P2_FINAL}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_u8buf("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, inputData, inputDataLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_CryptoObjectID("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cryptoObjectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional(
        "macValidateData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, macValidateData, macValidateDataLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4_ext(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, macValue, pmacValueLen); /*  */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_DigestInit(SE05x_CryptoObjectID_t cryptoObjectID)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_DEFAULT, kSE05x_P2_INIT}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;

    tlvRet = TLVSET_CryptoObjectID("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cryptoObjectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_DigestUpdate(
    SE05x_CryptoObjectID_t cryptoObjectID, const BUINT8 *inputData, BUINT32 inputDataLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_DEFAULT, kSE05x_P2_UPDATE}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;

    tlvRet = TLVSET_CryptoObjectID("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cryptoObjectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8buf("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_DigestFinal(SE05x_CryptoObjectID_t cryptoObjectID,
                                 const BUINT8 *inputData,
                                 BUINT32 inputDataLen,
                                 BUINT8 *cmacValue,
                                 BUINT32 *pcmacValueLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_DEFAULT, kSE05x_P2_FINAL}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_CryptoObjectID("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cryptoObjectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8buf("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, cmacValue, pcmacValueLen); /*  */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_GetVersion(BUINT8 *pappletVersion, BUINT32 *appletVersionLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_VERSION}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    retStatus = DoAPDUTxRx_s_Case2(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, pappletVersion, appletVersionLen); /* - */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_GetTimestamp(SE05x_TimeStamp_t *ptimeStamp)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_TIME}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    retStatus = DoAPDUTxRx_s_Case2(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_TimeStamp(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, ptimeStamp); /* - */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_GetFreeMemory(SE05x_MemoryType_t memoryType, BUINT16 *pfreeMem)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_MEMORY}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);

    tlvRet = TLVSET_MemoryType("memoryType", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, memoryType);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        tlvRet = tlvGet_U16(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, pfreeMem); /* - */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_GetRandom(BUINT16 size, BUINT8 *randomData, BUINT32 *prandomDataLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_RANDOM}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    BUINT32 rspbufLen = ARRAY_SIZE(rspbuf);
    tlvRet = TLVSET_U16("size", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, size);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4_ext(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        BUINT32 rspIndex = 0;
        BoatLog_hexdump(BOAT_LOG_NORMAL, "pRspbuf : ", pRspbuf, rspbufLen);
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, randomData, prandomDataLen); /*  */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

// LCOV_EXCL_START
smStatus_t Se05x_API_DeleteAll(void)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_DELETE_ALL}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    BUINT32 cmdbufLen = 0;

    retStatus = DoAPDUTx_s_Case3(&hdr, cmdbuf, cmdbufLen);
    return retStatus;
}
// LCOV_EXCL_STOP

smStatus_t Se05x_API_ReadObject_W_Attst(BUINT32 objectID,
                                        BUINT16 offset,
                                        BUINT16 length,
                                        BUINT32 attestID,
                                        SE05x_AttestationAlgo_t attestAlgo,
                                        const BUINT8 *random,
                                        size_t randomLen,
                                        BUINT8 *data,
                                        size_t *pdataLen,
                                        BUINT8 *attribute,
                                        size_t *pattributeLen,
                                        SE05x_TimeStamp_t *ptimeStamp,
                                        BUINT8 *outrandom,
                                        size_t *poutrandomLen,
                                        BUINT8 *chipId,
                                        size_t *pchipIdLen,
                                        BUINT8 *signature,
                                        size_t *psignatureLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr = {{kSE05x_CLA, kSE05x_INS_READ_With_Attestation, kSE05x_P1_DEFAULT, kSE05x_P2_DEFAULT}};
    BUINT8 cmdbuf[SE05X_MAX_BUF_SIZE_CMD];
    size_t cmdbufLen = 0;
    BUINT8 *pCmdbuf = &cmdbuf[0];
    int tlvRet = 0;
    BUINT8 rspbuf[SE05X_MAX_BUF_SIZE_RSP] = {0};
    BUINT8 *pRspbuf = &rspbuf[0];
    size_t rspbufLen = ARRAY_SIZE(rspbuf);
#if VERBOSE_APDU_LOGS
    NEWLINE();
    nLog("APDU", NX_LEVEL_DEBUG, "ReadObject_W_Attst []");
#endif /* VERBOSE_APDU_LOGS */
    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_U16Optional("offset", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, offset);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_U16Optional("length", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, length);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_U32("attestID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_5, attestID);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_AttestationAlgo("attestAlgo", &pCmdbuf, &cmdbufLen, kSE05x_TAG_6, attestAlgo);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("random", &pCmdbuf, &cmdbufLen, kSE05x_TAG_7, random, randomLen);
    if (0 != tlvRet)
    {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx_s_Case4_ext(&hdr, cmdbuf, cmdbufLen, rspbuf, &rspbufLen);
    if (retStatus == SM_OK)
    {
        retStatus = SM_NOT_OK;
        size_t rspIndex = 0;
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, data, pdataLen); /*  */
        if (0 != tlvRet)
        {
            /* Keys with no read policy will not return TAG1 */
            // goto cleanup;
        }
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_2, attribute, pattributeLen); /*  */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        tlvRet = tlvGet_TimeStamp(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_3, ptimeStamp); /* - */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_4, outrandom, poutrandomLen); /*  */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_5, chipId, pchipIdLen); /*  */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_6, signature, psignatureLen); /*  */
        if (0 != tlvRet)
        {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen)
        {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}