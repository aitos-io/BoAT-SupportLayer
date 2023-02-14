/**
****************************************************************************************
* @FilePath: se05x_tlv.c
* @Author: aitos
* @Date: 2022-11-21 16:56:40
* @LastEditors:
* @LastEditTime: 2022-11-21 16:56:41
* @Descripttion:
****************************************************************************************
*/
/*
 *
 * Copyright 2019-2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "se05x_tlv.h"
#include <string.h> // memcpy
#include "boatlog.h"
#include "boaterrcode.h"
#include "se_process.h"
#include "sm_types.h"

#ifdef FLOW_VERBOSE
#define VERBOSE_APDU_LOGS 1
#else
#define VERBOSE_APDU_LOGS 0
#endif

int tlvSet_U8(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, BUINT8 value)
{
    BUINT8 *pBuf = *buf;
    const size_t size_of_tlv = 1 + 1 + 1;
    if (((*bufLen) + size_of_tlv) > SE05X_TLV_BUF_SIZE_CMD)
    {
        return 1;
    }
    *pBuf++ = (BUINT8)tag;
    *pBuf++ = 1;
    *pBuf++ = value;
    *buf = pBuf;
    *bufLen += size_of_tlv;
    return 0;
}

int tlvSet_U16Optional(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, BUINT16 value)
{
    if (value == 0)
    {
        return 0;
    }
    else
    {
        return tlvSet_U16(buf, bufLen, tag, value);
    }
}

int tlvSet_U16(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, BUINT16 value)
{
    const size_t size_of_tlv = 1 + 1 + 2;
    BUINT8 *pBuf = *buf;
    if (((*bufLen) + size_of_tlv) > SE05X_TLV_BUF_SIZE_CMD)
    {
        return 1;
    }
    *pBuf++ = (BUINT8)tag;
    *pBuf++ = 2;
    *pBuf++ = (BUINT8)((value >> 1 * 8) & 0xFF);
    *pBuf++ = (BUINT8)((value >> 0 * 8) & 0xFF);
    *buf = pBuf;
    *bufLen += size_of_tlv;
    return 0;
}

int tlvSet_U32(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, BUINT32 value)
{
    const size_t size_of_tlv = 1 + 1 + 4;
    BUINT8 *pBuf = *buf;
    if (((*bufLen) + size_of_tlv) > SE05X_TLV_BUF_SIZE_CMD)
    {
        return 1;
    }
    *pBuf++ = (BUINT8)tag;
    *pBuf++ = 4;
    *pBuf++ = (BUINT8)((value >> 3 * 8) & 0xFF);
    *pBuf++ = (BUINT8)((value >> 2 * 8) & 0xFF);
    *pBuf++ = (BUINT8)((value >> 1 * 8) & 0xFF);
    *pBuf++ = (BUINT8)((value >> 0 * 8) & 0xFF);
    *buf = pBuf;
    *bufLen += size_of_tlv;
    return 0;
}

int tlvSet_U64_size(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, BUINT64 value, BUINT16 size)
{
    int8_t pos = 0;
    pos = (BUINT8)size;
    const size_t size_of_tlv = 1 + 1 + size;
    BUINT8 *pBuf = *buf;
    if (((*bufLen) + size_of_tlv) > SE05X_TLV_BUF_SIZE_CMD)
    {
        return 1;
    }
    *pBuf++ = (BUINT8)tag;
    *pBuf++ = pos;
    pos--;
    for (; pos >= 0; pos--)
    {
        *pBuf++ = (BUINT8)((value >> pos * 8) & 0xFF);
    }
    *buf = pBuf;
    *bufLen += size_of_tlv;
    return 0;
}

int tlvSet_Se05xPolicy(const char *description, BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, Se05xPolicy_t *policy)
{
    AX_UNUSED_ARG(description);
    int tlvRet = 0;
    if ((policy != NULL) && (policy->value != NULL))
    {
        tlvRet = tlvSet_u8buf(buf, bufLen, tag, policy->value, policy->value_len);
        return tlvRet;
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "Policy is NULL");
    }

    return tlvRet;
}

int tlvSet_ECCurve(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, SE05x_ECCurve_t value)
{
    int retVal = 0;
    if (value != kSE05x_ECCurve_NA)
    {
        retVal = tlvSet_U8(buf, bufLen, tag, (BUINT8)value);
    }
    return retVal;
}

int tlvSet_u8bufOptional(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, const BUINT8 *cmd, size_t cmdLen)
{
    if (cmdLen == 0)
    {
        return 0;
    }
    else
    {
        return tlvSet_u8buf(buf, bufLen, tag, cmd, cmdLen);
    }
}

int tlvSet_u8bufOptional_ByteShift(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, const BUINT8 *cmd, size_t cmdLen)
{
    int ret = 1;
    if (cmdLen == 0)
    {
        ret = 0;
    }
    else if (0 == (cmdLen & 1))
    {
        /* LSB is 0 */
        ret = tlvSet_u8buf(buf, bufLen, tag, cmd, cmdLen);
    }
    else
    {
        BUINT8 localBuff[SE05X_MAX_BUF_SIZE_CMD];
        ENSURE_OR_GO_CLEANUP((cmdLen + 1) < sizeof(localBuff));
        localBuff[0] = '\0';
        memcpy(localBuff + 1, cmd, cmdLen);
        ret = tlvSet_u8buf(buf, bufLen, tag, localBuff, cmdLen + 1);
    }

cleanup:
    return ret;
}

int tlvSet_u8buf(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, const BUINT8 *cmd, size_t cmdLen)
{
    BUINT8 *pBuf = *buf;

    /* if < 0x7F
     *    len = 1 byte
     * elif if < 0xFF
     *    '0x81' + len == 2 Bytes
     * elif if < 0xFFFF
     *    '0x82' + len_msb + len_lsb == 3 Bytes
     */
    const size_t size_of_length = (cmdLen <= 0x7f ? 1 : (cmdLen <= 0xFf ? 2 : 3));
    const size_t size_of_tlv = 1 + size_of_length + cmdLen;

    if (((*bufLen) + size_of_tlv) > SE05X_TLV_BUF_SIZE_CMD)
    {
        BoatLog(BOAT_LOG_NORMAL, "Not enough buffer");
        return 1;
    }
    *pBuf++ = (BUINT8)tag;

    if (cmdLen <= 0x7Fu)
    {
        *pBuf++ = (BUINT8)cmdLen;
    }
    else if (cmdLen <= 0xFFu)
    {
        *pBuf++ = (BUINT8)(0x80 /* Extended */ | 0x01 /* Additional Length */);
        *pBuf++ = (BUINT8)((cmdLen >> 0 * 8) & 0xFF);
    }
    else if (cmdLen <= 0xFFFFu)
    {
        *pBuf++ = (BUINT8)(0x80 /* Extended */ | 0x02 /* Additional Length */);
        *pBuf++ = (BUINT8)((cmdLen >> 1 * 8) & 0xFF);
        *pBuf++ = (BUINT8)((cmdLen >> 0 * 8) & 0xFF);
    }
    else
    {
        return 1;
    }
    if ((cmdLen > 0) && (cmd != NULL))
    {
        while (cmdLen-- > 0)
        {
            *pBuf++ = *cmd++;
        }
    }

    *bufLen += size_of_tlv;
    *buf = pBuf;

    return 0;
}

int tlvGet_U8(BUINT8 *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag, BUINT8 *pRsp)
{
    int retVal = 1;
    BUINT8 *pBuf = buf + (*pBufIndex);
    BUINT8 got_tag = *pBuf++;
    size_t rspLen;

    if ((*pBufIndex) > bufLen)
    {
        goto cleanup;
    }

    if (got_tag != tag)
    {
        goto cleanup;
    }
    rspLen = *pBuf++;
    if (rspLen > 1)
    {
        goto cleanup;
    }
    *pRsp = *pBuf;
    *pBufIndex += (1 + 1 + (rspLen));
    retVal = 0;
cleanup:
    return retVal;
}

int tlvSet_KeyID(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, BUINT32 keyID)
{
    int retVal = 0;
    if (keyID != 0)
    {
        retVal = tlvSet_U32(buf, bufLen, tag, keyID);
    }
    return retVal;
}

int tlvSet_MaxAttemps(BUINT8 **buf, size_t *bufLen, SE05x_TAG_t tag, BUINT16 maxAttemps)
{
    int retVal = 0;
    if (maxAttemps != 0)
    {
        retVal = tlvSet_U16(buf, bufLen, tag, maxAttemps);
    }
    return retVal;
}

int tlvGet_SecureObjectType(BUINT8 *buf, size_t *pBufIndex, size_t bufLen, SE05x_TAG_t tag, SE05x_SecObjTyp_t *pType)
{
    BUINT8 uType = 0;
    int retVal = tlvGet_U8(buf, pBufIndex, bufLen, tag, &uType);
    *pType = (SE05x_SecObjTyp_t)uType;
    return retVal;
}

int tlvGet_Result(BUINT8 *buf, size_t *pBufIndex, size_t bufLen, SE05x_TAG_t tag, SE05x_Result_t *presult)
{
    BUINT8 uType = 0;
    size_t uTypeLen = 1;
    int retVal = tlvGet_u8buf(buf, pBufIndex, bufLen, tag, &uType, &uTypeLen);
    *presult = (SE05x_Result_t)uType;
    return retVal;
}

int tlvGet_U16(BUINT8 *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag, BUINT16 *pRsp)
{
    int retVal = 1;
    BUINT8 *pBuf = buf + (*pBufIndex);
    BUINT8 got_tag = *pBuf++;
    size_t rspLen;

    if ((*pBufIndex) > bufLen)
    {
        goto cleanup;
    }

    if (got_tag != tag)
    {
        goto cleanup;
    }
    rspLen = *pBuf++;
    if (rspLen > 2)
    {
        goto cleanup;
    }
    *pRsp = (*pBuf++) << 8;
    *pRsp |= *pBuf++;
    *pBufIndex += (1 + 1 + (rspLen));
    retVal = 0;
cleanup:
    return retVal;
}

// ISO 7816-4 Annex D.
int tlvGet_u8buf(BUINT8 *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag, BUINT8 *rsp, size_t *pRspLen)
{
    int retVal = 1;
    BUINT8 *pBuf = buf + (*pBufIndex);
    BUINT8 got_tag = *pBuf++;
    size_t extendedLen;
    size_t rspLen;
    // size_t len;

    if (rsp == NULL)
    {
        goto cleanup;
    }

    if (pRspLen == NULL)
    {
        goto cleanup;
    }

    if ((*pBufIndex) > bufLen)
    {
        goto cleanup;
    }

    if (got_tag != tag)
    {
        goto cleanup;
    }
    rspLen = *pBuf++;

    if (rspLen <= 0x7FU)
    {
        extendedLen = rspLen;
        *pBufIndex += (1 + 1);
    }
    else if (rspLen == 0x81)
    {
        extendedLen = *pBuf++;
        *pBufIndex += (1 + 1 + 1);
    }
    else if (rspLen == 0x82)
    {
        extendedLen = *pBuf++;
        extendedLen = (extendedLen << 8) | *pBuf++;
        *pBufIndex += (1 + 1 + 2);
    }
    else
    {
        goto cleanup;
    }

    if (extendedLen > *pRspLen)
    {
        goto cleanup;
    }
    if (extendedLen > bufLen)
    {
        goto cleanup;
    }

    *pRspLen = extendedLen;
    *pBufIndex += extendedLen;
    while (extendedLen-- > 0)
    {
        *rsp++ = *pBuf++;
    }
    retVal = 0;
cleanup:
    if (retVal != 0)
    {
        if (pRspLen != NULL)
        {
            *pRspLen = 0;
        }
    }
    return retVal;
}

int tlvGet_ValueIndex(BUINT8 *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag)
{
    int retVal = 1;
    BUINT8 *pBuf = buf + (*pBufIndex);
    BUINT8 got_tag = *pBuf++;
    size_t extendedLen;
    size_t rspLen;

    if ((*pBufIndex) > bufLen)
    {
        goto cleanup;
    }

    if (got_tag != tag)
    {
        goto cleanup;
    }
    rspLen = *pBuf++;

    if (rspLen <= 0x7FU)
    {
        extendedLen = rspLen;
        *pBufIndex += (1 + 1);
    }
    else if (rspLen == 0x81)
    {
        extendedLen = *pBuf++;
        *pBufIndex += (1 + 1 + 1);
    }
    else if (rspLen == 0x82)
    {
        extendedLen = *pBuf++;
        extendedLen = (extendedLen << 8) | *pBuf++;
        *pBufIndex += (1 + 1 + 2);
    }
    else
    {
        goto cleanup;
    }

    if (extendedLen > bufLen)
    {
        goto cleanup;
    }

    retVal = 0;
cleanup:
    return retVal;
}

int tlvGet_TimeStamp(BUINT8 *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag, SE05x_TimeStamp_t *pTs)
{
    int retVal = 1;
    if (pTs == NULL)
    {
        return retVal;
    }
    size_t rspBufSize = sizeof(pTs->ts);
    return tlvGet_u8buf(buf, pBufIndex, bufLen, tag, pTs->ts, &rspBufSize);
}

static smStatus_t sss_se05x_channel_txnRaw(const tlvHeader_t *hdr,
                                           BUINT8 *cmdBuf,
                                           size_t cmdBufLen,
                                           BUINT8 *rsp,
                                           size_t *rspLen,
                                           BUINT8 hasle)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 txBuf[SE05X_MAX_BUF_SIZE_CMD] = {0};
    size_t i = 0;
    memcpy(&txBuf[i], hdr, sizeof(*hdr));
    smStatus_t ret = SM_NOT_OK;
    i += sizeof(*hdr);
    if (cmdBufLen > 0)
    {
        // The Lc field must be extended in case the length does not fit
        // into a single byte (Note, while the standard would allow to
        // encode 0x100 as 0x00 in the Lc field, nobody who is sane in his mind
        // would actually do that).
        if ((cmdBufLen < 0xFF) && !hasle)
        {
            txBuf[i++] = (BUINT8)cmdBufLen;
        }
        else
        {
            txBuf[i++] = 0x00;
            txBuf[i++] = 0xFFu & (cmdBufLen >> 8);
            txBuf[i++] = 0xFFu & (cmdBufLen);
        }
        memcpy(&txBuf[i], cmdBuf, cmdBufLen);
        i += cmdBufLen;
    }
    else
    {
        if (cmdBufLen == 0)
        {
            txBuf[i++] = 0x00;
        }
    }

    if (hasle)
    {
        txBuf[i++] = 0x00;
        txBuf[i++] = 0x00;
    }

    BUINT32 U32rspLen = (BUINT32)*rspLen;
    result = boat_send_apdu(txBuf, (BUINT16)i, rsp, &U32rspLen, &ret);
    BoatLog(BOAT_LOG_NORMAL, "sss_se05x_channel_txnRaw result = %d , ret = %x ", result, ret);
    if (result != BOAT_SUCCESS)
    {
        return SM_NOT_OK;
    }
    // ret = (smStatus_t)smCom_TransceiveRaw(conn_ctx, txBuf, (U16)i, rsp, &U32rspLen);
    *rspLen = U32rspLen;
    return ret;
}

smStatus_t DoAPDUTx_s_Case3(const tlvHeader_t *hdr, BUINT8 *cmdBuf, size_t cmdBufLen)
{
    BUINT8 rxBuf[SE05X_TLV_BUF_SIZE_RSP + 2] = {0};
    size_t rxBufLen = sizeof(rxBuf);
    smStatus_t apduStatus = SM_NOT_OK;

    apduStatus = sss_se05x_channel_txnRaw(hdr, cmdBuf, cmdBufLen, rxBuf, &rxBufLen, 0);

    return apduStatus;
}

smStatus_t DoAPDUTxRx_s_Case2(const tlvHeader_t *hdr,
                              BUINT8 *cmdBuf,
                              size_t cmdBufLen,
                              BUINT8 *rspBuf,
                              size_t *pRspBufLen)
{
    smStatus_t apduStatus;

    apduStatus = sss_se05x_channel_txnRaw(hdr, cmdBuf, cmdBufLen, rspBuf, pRspBufLen, 0);

    return apduStatus;
}

smStatus_t DoAPDUTxRx_s_Case4(const tlvHeader_t *hdr,
                              BUINT8 *cmdBuf,
                              size_t cmdBufLen,
                              BUINT8 *rspBuf,
                              size_t *pRspBufLen)
{
    smStatus_t apduStatus;

    apduStatus = sss_se05x_channel_txnRaw(hdr, cmdBuf, cmdBufLen, rspBuf, pRspBufLen, 0);

    return apduStatus;
}

smStatus_t DoAPDUTxRx_s_Case4_ext(const tlvHeader_t *hdr,
                                  BUINT8 *cmdBuf,
                                  size_t cmdBufLen,
                                  BUINT8 *rspBuf,
                                  size_t *pRspBufLen)
{
    smStatus_t apduStatus = SM_NOT_OK;

    apduStatus = sss_se05x_channel_txnRaw(hdr, cmdBuf, cmdBufLen, rspBuf, pRspBufLen, 0);

    return apduStatus;
}