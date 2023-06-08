/**
****************************************************************************************
* @FilePath: boat_keystore_intf.c
* @Author: aitos
* @Date: 2023-02-15 16:17:29
* @LastEditors:
* @LastEditTime: 2023-02-15 16:17:29
* @Descripttion:
****************************************************************************************
*/
#include "boattypes.h"
#include "boatkeypair.h"
#include "boaterrcode.h"
#include "boatlog.h"
#include "sha2.h"
#include "sha3.h"
#include "aes/aes.h"

#include "secp256k1.h"
#include "nist256p1.h"
#include "bignum.h"
#include "boatutility.h"
#include "boat_keystore_intf.h"
#include "boat_keystore_soft.h"
#if (BOAT_CRYPTO_USE_SE == 1)
#include "boat_keystore_se.h"
#endif
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
BOAT_RESULT BoAT_Keystore_Gen_Keypair(BoatKeypairPriKeyType type, BoatKeypairKeypair *keypair)
{
#if (BOAT_CRYPTO_USE_SE == 1)
    return BoAT_Keystore_Gen_Keypair_se(type, keypair);
#else
    return BoAT_Keystore_Gen_Keypair_soft(type, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE, keypair);
#endif
}

/**
 * @description:
 * 	This function gen signature by digest, no need to know private key, only the index of private key.
 * @param[in] {BoatKeypairPriKeyType} type
 * 	support ecdsa now.
 * @param[in] {BUINT8} prikeyIndex
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
BOAT_RESULT BoAT_Keystore_Sign(BoatKeypairPriKeyType type, BUINT8 prikeyIndex, const BUINT8 *digest, BUINT32 digestLen, BUINT8 *signature, BUINT32 *signatureLen, BUINT8 *Prefix)
{
#if (BOAT_CRYPTO_USE_SE == 1)
    return BoAT_Keystore_Sign_se(type, prikeyIndex, digest, digestLen, signature, signatureLen, Prefix);
#else
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairExtraData prikey;
    result = BoAT_GetPrikeyByIndex_soft(prikeyIndex, &prikey);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, "get prikey by index fail");
        return result;
    }
    result = BoAT_sign_soft(type, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE, prikey.value, prikey.value_len, digest, digestLen, signature, signatureLen, Prefix);
    return result;
#endif
}

/**
****************************************************************************************
* @brief:
*   This function cal digest from input data using crypto-default library.
* @param[in] type
*   digest alg type: keccak256 or sha256
* @param[in] *input
*   input data of digest.
* @param[in] inputLen
*   length of input data
* @param[in] *hashed
*   digest result
* @param[in] *hashedLen
*   length of digest
* @param[in] *rsvd
*   no used until now
* @return
*  This function returns BoAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT BoatHash(const BoatHashAlgType type, const BUINT8 *input, BUINT32 inputLen, BUINT8 *hashed, BUINT8 *hashedLen, void *rsvd)
{
    BOAT_RESULT result = BOAT_SUCCESS;

    /* input param check */
    if (hashed == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "param which 'hashed' can't be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if (type == BOAT_HASH_KECCAK256)
    {
        keccak_256(input, inputLen, hashed);
        if (hashedLen != NULL)
        {
            *hashedLen = 32;
        }
    }
    else if (type == BOAT_HASH_SHA256)
    {
        sha256_Raw(input, inputLen, hashed);
        if (hashedLen != NULL)
        {
            *hashedLen = 32;
        }
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "unknown boat hash algorithm type.");
        result = BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    return result;
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
BOAT_RESULT BoAT_Keystore_store_prikey(BUINT8 keypairIndex, BUINT8 *prikey, BUINT32 prikeyLen)
{
#if (BOAT_CRYPTO_USE_SE != 1)
    return BoAT_Keystore_store_prikey_soft(keypairIndex, prikey, prikeyLen);
#else
    return BOAT_SUCCESS;
#endif
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
BOAT_RESULT BoAT_DeletePrikeyByIndex(BUINT8 index)
{
#if (BOAT_CRYPTO_USE_SE != 1)
    return BoAT_DeletePrikeyByIndex_soft(index);
#else
    return BOAT_SUCCESS;
#endif
}

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
BOAT_RESULT BoAT_Keystore_Write_Binary_secure(BUINT8 *data, BUINT32 datalen, BUINT32 offset)
{
#if (BOAT_CRYPTO_USE_SE == 1)
    return BoAT_Keystore_Write_Binary_se(data, datalen, offset);
#else
    return BOAT_ERROR;
#endif
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
BOAT_RESULT BoAT_Keystore_read_Binary_secure(BUINT8 *data, BUINT32 *datalen, BUINT32 offset)
{
#if (BOAT_CRYPTO_USE_SE == 1)
    return BoAT_Keystore_read_Binary_se(data, datalen, offset);
#else
    return BOAT_ERROR;
#endif
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
BOAT_RESULT BoAT_recover_pubkey(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format, BUINT8 *pubkey, const BUINT8 *digest, const BUINT8 *signature, BUINT8 Prefix)
{
    return BoAT_recover_pubkey_soft(type, format, pubkey, digest, signature, Prefix);
}

/**
 * @description:
 * 	This function get pubkey from prikey;
 * @param {BoatKeypairPriKeyType} type
 * 	now only support ecdsa and will support other alg such as SM
 * @param {BUINT8} *prikey
 * 	prikey inut
 * @param {BUINT32} prikeyLen
 * 	length of prikey
 * @param {BUINT8} *pubkey
 * 	pubkey output
 * @param {BUINT32} *pubkeyLen
 * 	length of pubkey
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoAT_getPubkey(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format, BUINT8 *prikey, BUINT32 prikeyLen, BUINT8 *pubkey, BUINT32 *pubkeyLen)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BUINT8 pubKey65[65] = {0};
    if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1)
    {
        ecdsa_get_public_key65(&secp256k1, prikey, pubKey65);
        memcpy(pubkey, &pubKey65[1], 64);
        *pubkeyLen = 64;
    }
    else if (type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1)
    {
        ecdsa_get_public_key65(&nist256p1, prikey, pubKey65);
        memcpy(pubkey, &pubKey65[1], 64);
        *pubkeyLen = 64;
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "Invalid private key type.");
        result = BOAT_ERROR_KEYPAIR_KEY_TYPE_ERR;
    }
    return result;
}

BOAT_RESULT BoatSignature(BoatKeypairPriKeyCtx prikeyCtx,
                          const BUINT8 *digest, BUINT32 digestLen,
                          BoatSignatureResult *signatureResult, void *rsvd)
{
    BUINT8 signature[64] = {0};
    BUINT8 signatureTmp[139];
    BUINT32 signatureTmpLen = 0;
    BUINT8 ecdsPrefix = 0;
    BUINT32 signatureLen = 0;
    BOAT_RESULT result = BOAT_SUCCESS;

    (void)rsvd;

    /* param check */
    if ((digest == NULL) || (signatureResult == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "parameter can't be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // result = BoAT_sign(prikeyCtx.prikey_type,prikeyCtx.prikey_format, prikey.value,prikey.value_len,digest,digestLen,signatureTmp,&signatureLen,&ecdsPrefix);

    result = BoAT_Keystore_Sign(prikeyCtx.prikey_type, prikeyCtx.keypair_index, digest, digestLen, signature, &signatureLen, &ecdsPrefix);
    if (result != BOAT_SUCCESS)
    {
        return result;
    }

    /* convert r,s to asn.1 */
    result = utility_signature_to_asn1(signature, signatureLen, signatureTmp, &signatureTmpLen);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "signature to asn.1  fail.");
        return BOAT_ERROR;
    }
    // signature result assign
    memset(signatureResult, 0, sizeof(BoatSignatureResult));

    signatureResult->pkcs_format_used = true;
    signatureResult->pkcs_sign_length = signatureTmpLen;
    memcpy(signatureResult->pkcs_sign, signatureTmp, signatureResult->pkcs_sign_length);

    signatureResult->native_format_used = true;
    memcpy(&signatureResult->native_sign[0], signature, 64);

    signatureResult->signPrefix_used = true;
    signatureResult->signPrefix = ecdsPrefix;

    return result;
}

/******************************************************************************
                              BOAT KEY PROCESS WARPPER
*******************************************************************************/
static BOAT_RESULT sBoatPort_keyCreate_internal_generation(const BoatKeypairPriKeyCtx_config *config,
                                                           BoatKeypairDataCtx *pkCtx)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairKeypair keypair;
    // result = BoAT_Keypair_generation(config->prikey_type,config->prikey_format,&keypair);
    result = BoAT_Keystore_Gen_Keypair(config->prikey_type, &keypair);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "generate private key failed.");
        return result;
    }
#if (BOAT_CRYPTO_USE_SE == 1)		///// add for BOAT_CRYPTO_USE_SE , Temporary modification , ph 2023-03-02 19:17:00
    BUINT8 keypairGenFlag[1] = {0};

    keypairGenFlag[0] = 0x55;
    result = BoAT_Keystore_Write_Binary_secure(keypairGenFlag, sizeof(keypairGenFlag), OFFSET_KEYPAIR_GEN_FLAG);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "write keypair gen flag fail .");
        return result;
    }
#endif
    BoatLog(BOAT_LOG_CRITICAL, "generate private key OK .");
    // 2- update private key format
    pkCtx->prikeyCtx.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;

    // 3- update private key type
    pkCtx->prikeyCtx.prikey_type = config->prikey_type;
    pkCtx->extraData.value_len = keypair.prikey.value_len;
    memcpy(pkCtx->extraData.value, keypair.prikey.value, keypair.prikey.value_len);
    memcpy(pkCtx->prikeyCtx.pubkey_content, keypair.pubkey.value, 64);

    return result;
}

static BOAT_RESULT sBoatPort_keyCreate_external_injection_native(const BoatKeypairPriKeyCtx_config *config,
                                                                 BoatKeypairDataCtx *pkCtx)
{
    BUINT32 len = 0;
    BOAT_RESULT result = BOAT_SUCCESS;

    // 0- check input parameter
    if ((config == NULL) || (config->prikey_content.field_ptr == NULL) || (pkCtx == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "input parameter can not be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // 1- update private key
    if (config->prikey_content.field_len > sizeof(pkCtx->extraData.value))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Error: length of injection key is too long.");
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }

    memcpy(pkCtx->extraData.value, config->prikey_content.field_ptr, config->prikey_content.field_len);
    pkCtx->extraData.value_len = config->prikey_content.field_len;

    // 2- update private key format
    pkCtx->prikeyCtx.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;

    // 3- update private key type
    pkCtx->prikeyCtx.prikey_type = config->prikey_type;

    // 4- update private key index

    // 5- update public key
    result = BoAT_getPubkey(config->prikey_type, config->prikey_format, pkCtx->extraData.value, pkCtx->extraData.value_len, pkCtx->prikeyCtx.pubkey_content, &len);

    return result;
}

static BOAT_RESULT sBoatPort_keyCreate_external_injection_pkcs(const BoatKeypairPriKeyCtx_config *config,
                                                               BoatKeypairDataCtx *pkCtx)
{
    // BUINT8       pubKey65[65] = {0};
    BOAT_RESULT result = BOAT_SUCCESS;
    KeypairNative keypair;

    // 0- check input parameter
    if ((config == NULL) || (config->prikey_content.field_ptr == NULL) || (pkCtx == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "input parameter can not be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    // 1- update private key
    if (config->prikey_content.field_len > sizeof(pkCtx->extraData.value))
    {
        BoatLog(BOAT_LOG_CRITICAL, "Error: length of injection key is too long.");
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }

    result = UtilityPKCS2Native((BCHAR *)(config->prikey_content.field_ptr), &keypair);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> UtilityPKCS2Native err.");
        UtilityFreeKeypair(keypair);
        return result;
    }

    // memcpy(pkCtx->extra_data.value, config->prikey_content.field_ptr, config->prikey_content.field_len);
    // pkCtx->extra_data.value_len = config->prikey_content.field_len;
    memcpy(pkCtx->extraData.value, keypair.prikey, keypair.prikeylen);
    pkCtx->extraData.value_len = keypair.prikeylen;

    // 2- update private key format
    pkCtx->prikeyCtx.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;

    // 3- update private key type
    pkCtx->prikeyCtx.prikey_type = config->prikey_type;

    // 4- update private key index
    // This field should update by 'key secure storage'(such as TE/SE).
    // When algorithms are implemented by software, this field is default to 0, means
    // that ignore this field.

    // 5- update public key

    // if (config->prikey_type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1)
    // {
    // 	ecdsa_get_public_key65(&secp256k1, pkCtx->extra_data.value, pubKey65);
    // 	memcpy(pkCtx->pubkey_content, &pubKey65[1], 64);
    // }
    // else if (config->prikey_type == BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1)
    // {
    // 	ecdsa_get_public_key65(&nist256p1, pkCtx->extra_data.value, pubKey65);
    // 	memcpy(pkCtx->pubkey_content, &pubKey65[1], 64);
    // }
    // else
    // {
    // 	BoatLog( BOAT_LOG_CRITICAL, "Invalid private key type." );
    // 	result = BOAT_ERROR_WALLET_KEY_TYPE_ERR;
    // }
    memcpy(pkCtx->prikeyCtx.pubkey_content, keypair.pubkey, keypair.pubkeylen);
    UtilityFreeKeypair(keypair);
    return result;
}

BOAT_RESULT BoatPort_keyCreate(const BoatKeypairPriKeyCtx_config *config, BoatKeypairDataCtx *pkCtx)
{
    BOAT_RESULT result = BOAT_SUCCESS;

    if ((config == NULL) || (pkCtx == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL, "parameter can't be NULL.");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    BoatLog(BOAT_LOG_NORMAL, "prikey_genMode = %d .", config->prikey_genMode);
    if (config->prikey_genMode == BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION)
    {
        BoatLog(BOAT_LOG_VERBOSE, "The private key is generated internally...");
        result = sBoatPort_keyCreate_internal_generation(config, pkCtx);
    }
    else if (config->prikey_genMode == BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION)
    {
        BoatLog(BOAT_LOG_NORMAL, "prikey_format = %d .", config->prikey_format);
        switch (config->prikey_format)
        {
        case BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS:
            BoatLog(BOAT_LOG_NORMAL, "keypair private key[pkcs] set...");
            // result = BOAT_ERROR_kEYPAIR_KEY_FORMAT_ERR;
            result = sBoatPort_keyCreate_external_injection_pkcs(config, pkCtx);
            break;
        case BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE:
            BoatLog(BOAT_LOG_VERBOSE, "keypair private key[native] set...");
            result = sBoatPort_keyCreate_external_injection_native(config, pkCtx);
            break;
        case BOAT_KEYPAIR_PRIKEY_FORMAT_MNEMONIC:
            BoatLog(BOAT_LOG_NORMAL, "NOT SUPPORT FORMAT YET.");
            result = BOAT_ERROR_KEYPAIR_KEY_FORMAT_ERR;
            break;
        default:
            BoatLog(BOAT_LOG_CRITICAL, "Invalid private key format.");
            result = BOAT_ERROR_KEYPAIR_KEY_FORMAT_ERR;
            break;
        }
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "Invalid private key format.");
        result = BOAT_ERROR_KEYPAIR_KEY_GENMODE_ERR;
    }

    return result;
}

BOAT_RESULT BoatPort_keyQuery(const BoatKeypairPriKeyCtx_config *config, BoatKeypairPriKeyCtx *pkCtx)
{
    //! @todo
    return BOAT_ERROR;
}

BOAT_RESULT BoatPort_keyDelete(BoatKeypairPriKeyCtx *pkCtx)
{
    //! @todo
    return BOAT_ERROR;
}

/******************************************************************************
                              BOAT AES WARPPER
*******************************************************************************/
BOAT_RESULT BoatAesEncrypt(BUINT8 iv[16], const BUINT8 *key, const BUINT8 *input, size_t length, BUINT8 *output)
{
    aes_encrypt_ctx ctxe;
    BUINT8 saltArrayTmp[16];
    BOAT_RESULT result = BOAT_SUCCESS;

    /* aes init */
    result += aes_encrypt_key128(key, &ctxe);

    /* use saltArrayTmp because function aes_cbc_encrypt(...) will modify this field */
    memcpy(saltArrayTmp, iv, 16);

    /* encrypt process */
    result += aes_cbc_encrypt(input, output, length, saltArrayTmp, &ctxe);

    return result;
}

BOAT_RESULT BoatAesDecrypt(BUINT8 iv[16], const BUINT8 *key, const BUINT8 *input, size_t length, BUINT8 *output)
{
    aes_decrypt_ctx ctxd;
    BOAT_RESULT result = BOAT_SUCCESS;

    /* aes init */
    result += aes_decrypt_key128(key, &ctxd);

    /* decrypt process */
    result += aes_cbc_decrypt(input, output, length, iv, &ctxd);

    return result;
}