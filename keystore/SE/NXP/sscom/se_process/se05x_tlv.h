/**
****************************************************************************************
* @FilePath: se05x_tlv.h
* @Author: aitos
* @Date: 2022-11-21 16:56:52
* @LastEditors:
* @LastEditTime: 2022-11-21 16:56:52
* @Descripttion:
****************************************************************************************
*/
/*
 *
 * Copyright 2019,2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SE05X_TLV_H_INC
#define SE05X_TLV_H_INC

#include <stdio.h>
#include <stdlib.h>
#include "se05x_enums.h"
#include "boattypes.h"
// #define VERBOSE_APDU_LOGS 1
#define SE05X_MAX_BUF_SIZE_CMD (892)
#define SE05X_MAX_BUF_SIZE_RSP (892)

#define SE05X_TLV_BUF_SIZE_CMD SE05X_MAX_BUF_SIZE_CMD
#define SE05X_TLV_BUF_SIZE_RSP SE05X_MAX_BUF_SIZE_RSP
#define kSE05x_CLA 0x80

typedef struct
{
    BUINT8 hdr[0   /* For Indentation */
               + 1 /* CLA */
               + 1 /* INS */
               + 1 /* P1 */
               + 1 /* P2 */
    ];
} tlvHeader_t;

typedef enum
{
    kSSS_AuthType_None = 0,
    /** Global platform SCP03 */
    kSSS_AuthType_SCP03 = 1,
    /** (e.g. SE05X) UserID based connection */
    kSSS_AuthType_ID = 2,

    /** (e.g. SE05X) Use AESKey for user authentication
     *
     *  Earlier this was called  kSSS_AuthType_AppletSCP03
     */
    kSSS_AuthType_AESKey = 3,
    /** (e.g. SE05X) Use ECKey for user authentication
     *
     *  Earlier this was called  kSSS_AuthType_FastSCP
     */
    kSSS_AuthType_ECKey = 4,

    /* ================ Internal ======================= */
    /* Not to be selected by end user... directly */

    /**
     * Used internally, not to be set/used by user.
     *
     * For the versions of the applet where we have to add
     * the a counter during KDF.
     */
    kSSS_AuthType_INT_ECKey_Counter = 0x14,

    kSSS_SIZE = 0x7FFFFFFF,
} SE_AuthType_t;

typedef enum
{
    SM_NOT_OK = 0xFFFF,                       // Error
    SM_OK = 0x9000,                           // No Error
    SM_ERR_WRONG_LENGTH = 0x6700,             // Wrong length (e.g. C-APDU does not fit into APDU buffer)
    SM_ERR_CONDITIONS_NOT_SATISFIED = 0x6985, // Conditions not satisfied
    SM_ERR_COMMAND_NOT_ALLOWED = 0x6986,      // Command not allowed - access denied based on object policy
    SM_ERR_SECURITY_STATUS = 0x6982,          // Security status not satisfied
    SM_ERR_WRONG_DATA = 0x6A80,               // Wrong data provided
    SM_ERR_DATA_INVALID = 0x6984,             // Data invalid - policy set invalid for the given object
    SM_ERR_FILE_FULL = 0x6A84,                // Not enough memory space available (either transient or persistent memory)
} smStatus_t;

typedef enum
{
    CRED_DEFAULT = 0x00,
    CRED_EC = 0x01,
    CRED_RSA = 0x02,
    CRED_AES = 0x03,
    CRED_DES = 0x04,
    CRED_BINARY = 0x05,
    CRED_PIN = 0x06,
    CRED_COUNTER = 0x07,
    CRED_PCR = 0x08,
    CRED_OBJECT = 0x09,

    CRED_PUB_EC,
    CRED_PUB_RSA
} eSE05xType_t;

/** @addtogroup se05x_types
 *
 * @{ */

typedef struct
{
    BUINT8 *se05xTxBuf;
    size_t se05xTxBufLen;
    size_t ws_LC;                    // With Session LC
    size_t ws_LCW;                   // With Session LC Width 1 or 3 bytes
    BUINT8 *wsSe05x_cmd;             // WithSession SE05X command
    size_t wsSe05x_cmdLen;           // WithSession SE05X command Length
    size_t wsSe05x_tag1Len;          // WithSession SE05X Tag1 len
    size_t wsSe05x_tag1W;            // WithSession SE05X Tag1 Width
    BUINT8 *wsSe05x_tag1Cmd;         // WithSession SE05X Tag1 Command Data
    size_t wsSe05x_tag1CmdLen;       // WithSession SE05X Tag1 Command Data Len
    const tlvHeader_t *se05xCmd_hdr; // SE05x Command Header
    size_t se05xCmdLC;               // SE05x Command LC
    size_t se05xCmdLCW;              // SE05x Command LC width
    BUINT8 *se05xCmd;                // SE05x Command
    size_t se05xCmdLen;              // SE05x Command Length
    BUINT8 *dataToMac;
    size_t dataToMacLen;
} Se05xApdu_t;

typedef struct
{
    BUINT8 *value;
    size_t value_len;
} Se05xPolicy_t;

typedef struct
{
    BUINT8 ts[12];
} SE05x_TimeStamp_t;

typedef Se05xPolicy_t *pSe05xPolicy_t;

#define TLVSET_Se05xSession(DESCRIPTION, PBUF, PBUFLEN, TAG, SESSIONID) \
    TLVSET_u8buf(DESCRIPTION, PBUF, PBUFLEN, TAG, SESSIONID->value, sizeof(SESSIONID->value))

#define TLVSET_Se05xPolicy(DESCRIPTION, PBUF, PBUFLEN, TAG, POLICY) \
    tlvSet_Se05xPolicy(DESCRIPTION, PBUF, PBUFLEN, TAG, POLICY)

#define TLVSET_U8(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    tlvSet_U8(PBUF, PBUFLEN, TAG, VALUE);

#define TLVSET_U16(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    tlvSet_U16(PBUF, PBUFLEN, TAG, VALUE);

#define TLVSET_U16Optional(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    tlvSet_U16Optional(PBUF, PBUFLEN, TAG, VALUE);

#define TLVSET_U32(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    tlvSet_U32(PBUF, PBUFLEN, TAG, VALUE);

#define TLVSET_U64_SIZE(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE, SIZE) \
    tlvSet_U64_size(PBUF, PBUFLEN, TAG, VALUE, SIZE);

#define TLVSET_KeyID(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    tlvSet_KeyID(PBUF, PBUFLEN, TAG, VALUE);

#define TLVSET_MaxAttemps(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    tlvSet_MaxAttemps(PBUF, PBUFLEN, TAG, VALUE);

#define TLVSET_AttestationAlgo TLVSET_U8
#define TLVSET_CipherMode TLVSET_U8

#define TLVSET_ECCurve(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    tlvSet_ECCurve(PBUF, PBUFLEN, TAG, VALUE);

#define TLVSET_ECCurveParam TLVSET_U8
#define TLVSET_ECDAASignatureAlgo TLVSET_U8
#define TLVSET_ECSignatureAlgo TLVSET_U8
#define TLVSET_EDSignatureAlgo TLVSET_U8
#define TLVSET_MacOperation TLVSET_U8
#define TLVSET_RSAEncryptionAlgo TLVSET_U8
#define TLVSET_RSAKeyComponent TLVSET_U8
#define TLVSET_RSASignatureAlgo TLVSET_U8
#define TLVSET_DigestMode TLVSET_U8
#define TLVSET_Variant tlvSet_u8buf_features
#define TLVSET_RSAPubKeyComp TLVSET_U8
#define TLVSET_PlatformSCPRequest TLVSET_U8
#define TLVSET_MemoryType TLVSET_U8

#define TLVSET_CryptoContext TLVSET_U8
#define TLVSET_CryptoModeSubType(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    TLVSET_U8(DESCRIPTION, PBUF, PBUFLEN, TAG, ((VALUE).union_8bit))

#define TLVSET_CryptoObjectID TLVSET_U16

// #define TLVSET_pVoid(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) (0)
// #define tlvGet_pVoid(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) (0)

#define TLVSET_u8buf(DESCRIPTION, PBUF, PBUFLEN, TAG, CMD, CMDLEN) \
    tlvSet_u8buf(PBUF, PBUFLEN, TAG, CMD, CMDLEN);

#define TLVSET_u8bufOptional(DESCRIPTION, PBUF, PBUFLEN, TAG, CMD, CMDLEN) \
    tlvSet_u8bufOptional(PBUF, PBUFLEN, TAG, CMD, CMDLEN);

#define TLVSET_u8bufOptional_ByteShift(DESCRIPTION, PBUF, PBUFLEN, TAG, CMD, CMDLEN) \
    tlvSet_u8bufOptional_ByteShift(PBUF, PBUFLEN, TAG, CMD, CMDLEN);

#define TLVSET_u8buf_I2CM(DESCRIPTION, PBUF, PBUFLEN, TAG, CMD, CMDLEN) \
    tlvSet_u8buf_I2CM(PBUF, PBUFLEN, TAG, CMD, CMDLEN);

int tlvSet_U8(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, BUINT8 value);
int tlvSet_U16(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, BUINT16 value);
int tlvSet_U16Optional(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, BUINT16 value);
int tlvSet_U32(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, BUINT32 value);
int tlvSet_U64_size(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, BUINT64 value, BUINT16 size);
int tlvSet_u8buf(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, const BUINT8 *cmd, size_t cmdLen);
int tlvSet_u8bufOptional(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, const BUINT8 *cmd, size_t cmdLen);
/* Same as tlvSet_u8bufOptional, but some time, Most Significant Byte needs to be shifted and Plus by 1 */
int tlvSet_u8bufOptional_ByteShift(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, const BUINT8 *cmd, size_t cmdLen);
int tlvSet_Se05xPolicy(const char *description, BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, Se05xPolicy_t *policy);
int tlvSet_KeyID(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, BUINT32 keyID);
int tlvSet_MaxAttemps(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, BUINT16 maxAttemps);
int tlvSet_ECCurve(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, SE05x_ECCurve_t value);
// int tlvSet_u8buf_features(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, pSe05xAppletFeatures_t appletVariant);

int tlvGet_U8(BUINT8 *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag, BUINT8 *pRsp);
int tlvGet_U16(BUINT8 *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag, BUINT16 *pRsp);
int tlvGet_U32(BUINT8 *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag, BUINT32 *pRsp);

int tlvGet_u8buf(BUINT8 *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag, BUINT8 *rsp, size_t *pRspLen);
int tlvGet_ValueIndex(BUINT8 *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag);
// int tlvGet_Se05xSession(
//     BUINT8 *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag, pSe05xSession_t *pSessionId);
int tlvGet_TimeStamp(BUINT8 *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag, SE05x_TimeStamp_t *pTs);

int tlvSet_u8buf_I2CM(BUINT8 **buf, size_t *bufLen, SE05x_I2CM_TAG_t tag, const BUINT8 *cmd, size_t cmdLen);

int tlvGet_SecureObjectType(BUINT8 *buf, size_t *pBufIndex, size_t bufLen, SE05x_TAG_t tag, SE05x_SecObjTyp_t *pType);

int tlvGet_Result(BUINT8 *buf, size_t *pBufIndex, size_t bufLen, SE05x_TAG_t tag, SE05x_Result_t *presult);
smStatus_t DoAPDUTx_s_Case3(const tlvHeader_t *hdr, BUINT8 *cmdBuf, size_t cmdBufLen);
smStatus_t DoAPDUTxRx_s_Case2(const tlvHeader_t *hdr,
                              BUINT8 *cmdBuf,
                              size_t cmdBufLen,
                              BUINT8 *rspBuf,
                              size_t *pRspBufLen);
smStatus_t DoAPDUTxRx_s_Case4(const tlvHeader_t *hdr,
                              BUINT8 *cmdBuf,
                              size_t cmdBufLen,
                              BUINT8 *rspBuf,
                              size_t *pRspBufLen);
smStatus_t DoAPDUTxRx_s_Case4_ext(const tlvHeader_t *hdr,
                                  BUINT8 *cmdBuf,
                                  size_t cmdBufLen,
                                  BUINT8 *rspBuf,
                                  size_t *pRspBufLen);

#endif // !SE05X_TLV_H_INC
