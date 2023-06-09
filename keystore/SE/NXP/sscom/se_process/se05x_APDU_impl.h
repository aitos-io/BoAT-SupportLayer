/**
****************************************************************************************
* @FilePath: se05x_APDU_impl copy.c
* @Author: aitos
* @Date: 2022-12-01 15:24:19
* @LastEditors:
* @LastEditTime: 2022-12-01 15:24:19
* @Descripttion:
****************************************************************************************
*/
/*
 *
 * Copyright 2019-2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef SE05x_APDU_IMPL_H
#define SE05x_APDU_IMPL_H

#include <string.h>
#include "se05x_enums.h"
#include "se05x_tlv.h"
#include "boatlog.h"

/*! @brief Computes the number of elements in an array. */
#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

smStatus_t Se05x_API_CreateSession(BUINT32 authObjectID, BUINT8 *sessionId, BUINT32 *psessionIdLen);

smStatus_t Se05x_API_ExchangeSessionData(pSe05xPolicy_t policy);

smStatus_t Se05x_API_RefreshSession(pSe05xPolicy_t policy);

smStatus_t Se05x_API_WriteECKey(pSe05xPolicy_t policy,
                                SE05x_MaxAttemps_t maxAttempt,
                                BUINT32 objectID,
                                SE05x_ECCurve_t curveID,
                                const BUINT8 *privKey,
                                BUINT32 privKeyLen,
                                const BUINT8 *pubKey,
                                BUINT32 pubKeyLen,
                                const SE05x_INS_t ins_type,
                                const SE05x_KeyPart_t key_part);

smStatus_t Se05x_API_WriteSymmKey(pSe05xPolicy_t policy,
                                  SE05x_MaxAttemps_t maxAttempt,
                                  BUINT32 objectID,
                                  SE05x_KeyID_t kekID,
                                  const BUINT8 *keyValue,
                                  BUINT32 keyValueLen,
                                  const SE05x_INS_t ins_type,
                                  const SE05x_SymmKeyType_t type);

smStatus_t Se05x_API_WriteBinary(pSe05xPolicy_t policy,
                                 BUINT32 objectID,
                                 BUINT16 offset,
                                 BUINT16 length,
                                 const BUINT8 *inputData,
                                 BUINT32 inputDataLen);

smStatus_t Se05x_API_WriteUserID(pSe05xPolicy_t policy,
                                 SE05x_MaxAttemps_t maxAttempt,
                                 BUINT32 objectID,
                                 const BUINT8 *userId,
                                 BUINT32 userIdLen,
                                 const SE05x_AttestationType_t attestation_type);

smStatus_t Se05x_API_ReadType(BUINT32 objectID,
                              SE05x_SecureObjectType_t *ptype,
                              BUINT8 *pisTransient,
                              const SE05x_AttestationType_t attestation_type);

smStatus_t Se05x_API_ReadSize(BUINT32 objectID, BUINT16 *psize);

smStatus_t Se05x_API_ReadIDList(BUINT16 outputOffset,
                                BUINT8 filter,
                                BUINT8 *pmore,
                                BUINT8 *idlist,
                                BUINT32 *pidlistLen);

smStatus_t Se05x_API_CheckObjectExists(BUINT32 objectID, SE05x_Result_t *presult);

smStatus_t Se05x_API_DeleteSecureObject(BUINT32 objectID);

smStatus_t Se05x_API_CreateECCurve(SE05x_ECCurve_t curveID);

smStatus_t Se05x_API_SetECCurveParam(SE05x_ECCurve_t curveID,
                                     SE05x_ECCurveParam_t ecCurveParam,
                                     const BUINT8 *inputData,
                                     BUINT32 inputDataLen);

smStatus_t Se05x_API_GetECCurveId(BUINT32 objectID, BUINT8 *pcurveId);

smStatus_t Se05x_API_ReadECCurveList(BUINT8 *curveList, BUINT32 *pcurveListLen);

smStatus_t Se05x_API_DeleteECCurve(SE05x_ECCurve_t curveID);

smStatus_t Se05x_API_CreateCryptoObject(SE05x_CryptoObjectID_t cryptoObjectID,
                                        SE05x_CryptoContext_t cryptoContext,
                                        SE05x_CryptoModeSubType_t subtype);

smStatus_t Se05x_API_ReadObject(BUINT32 objectID, BUINT16 offset, BUINT16 length, BUINT8 *data, BUINT32 *pdataLen);

smStatus_t Se05x_API_ReadCryptoObjectList(BUINT8 *idlist, BUINT32 *pidlistLen);

smStatus_t Se05x_API_DeleteCryptoObject(SE05x_CryptoObjectID_t cryptoObjectID);

smStatus_t Se05x_API_ECDSASign(BUINT32 objectID,
                               SE05x_ECSignatureAlgo_t ecSignAlgo,
                               const BUINT8 *inputData,
                               BUINT32 inputDataLen,
                               BUINT8 *signature,
                               BUINT32 *psignatureLen);

smStatus_t Se05x_API_EdDSASign(BUINT32 objectID,
                               SE05x_EDSignatureAlgo_t edSignAlgo,
                               const BUINT8 *inputData,
                               BUINT32 inputDataLen,
                               BUINT8 *signature,
                               BUINT32 *psignatureLen);

smStatus_t Se05x_API_ECDSAVerify(BUINT32 objectID,
                                 SE05x_ECSignatureAlgo_t ecSignAlgo,
                                 const BUINT8 *inputData,
                                 BUINT32 inputDataLen,
                                 const BUINT8 *signature,
                                 BUINT32 signatureLen,
                                 SE05x_Result_t *presult);

smStatus_t Se05x_API_EdDSAVerify(BUINT32 objectID,
                                 SE05x_EDSignatureAlgo_t edSignAlgo,
                                 const BUINT8 *inputData,
                                 BUINT32 inputDataLen,
                                 const BUINT8 *signature,
                                 BUINT32 signatureLen,
                                 SE05x_Result_t *presult);

smStatus_t Se05x_API_CipherInit(BUINT32 objectID,
                                SE05x_CryptoObjectID_t cryptoObjectID,
                                BUINT8 *IV,
                                BUINT32 IVLen,
                                const SE05x_Cipher_Oper_t operation);

smStatus_t Se05x_API_CipherUpdate(SE05x_CryptoObjectID_t cryptoObjectID,
                                  const BUINT8 *inputData,
                                  BUINT32 inputDataLen,
                                  BUINT8 *outputData,
                                  BUINT32 *poutputDataLen);

smStatus_t Se05x_API_CipherFinal(SE05x_CryptoObjectID_t cryptoObjectID,
                                 const BUINT8 *inputData,
                                 BUINT32 inputDataLen,
                                 BUINT8 *outputData,
                                 BUINT32 *poutputDataLen);

smStatus_t Se05x_API_MACInit(BUINT32 objectID,
                             SE05x_CryptoObjectID_t cryptoObjectID,
                             const SE05x_Mac_Oper_t mac_oper);

smStatus_t Se05x_API_MACUpdate(
    const BUINT8 *inputData, BUINT32 inputDataLen, SE05x_CryptoObjectID_t cryptoObjectID);

smStatus_t Se05x_API_MACFinal(const BUINT8 *inputData,
                              BUINT32 inputDataLen,
                              SE05x_CryptoObjectID_t cryptoObjectID,
                              const BUINT8 *macValidateData,
                              BUINT32 macValidateDataLen,
                              BUINT8 *macValue,
                              BUINT32 *pmacValueLen);

smStatus_t Se05x_API_DigestInit(SE05x_CryptoObjectID_t cryptoObjectID);

smStatus_t Se05x_API_DigestUpdate(
    SE05x_CryptoObjectID_t cryptoObjectID, const BUINT8 *inputData, BUINT32 inputDataLen);

smStatus_t Se05x_API_DigestFinal(SE05x_CryptoObjectID_t cryptoObjectID,
                                 const BUINT8 *inputData,
                                 BUINT32 inputDataLen,
                                 BUINT8 *cmacValue,
                                 BUINT32 *pcmacValueLen);

smStatus_t Se05x_API_GetVersion(BUINT8 *pappletVersion, BUINT32 *appletVersionLen);

smStatus_t Se05x_API_GetTimestamp(SE05x_TimeStamp_t *ptimeStamp);

smStatus_t Se05x_API_GetFreeMemory(SE05x_MemoryType_t memoryType, BUINT16 *pfreeMem);

smStatus_t Se05x_API_GetRandom(BUINT16 size, BUINT8 *randomData, BUINT32 *prandomDataLen);

// LCOV_EXCL_START
smStatus_t Se05x_API_DeleteAll(void);

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
                                        size_t *psignatureLen);

// LCOV_EXCL_STOP
#endif