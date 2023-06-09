/**
****************************************************************************************
* @FilePath: boat_keystore_intf.h
* @Author: aitos
* @Date: 2023-02-15 16:17:42
* @LastEditors:
* @LastEditTime: 2023-02-15 16:17:42
* @Descripttion:
****************************************************************************************
*/
#ifndef __BOAT_KEYSTORE_INTF_H__
#define __BOAT_KEYSTORE_INTF_H__

#include "boattypes.h"
#include "boatkeypair.h"

#define BOAT_CRYPTO_USE_SE 0

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
BOAT_RESULT BoAT_Keystore_Gen_Keypair(BoatKeypairPriKeyType type, BoatKeypairKeypair *keypair);

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
BOAT_RESULT BoAT_Keystore_Sign(BoatKeypairPriKeyType type, BUINT8 prikeyIndex, const BUINT8 *digest, BUINT32 digestLen, BUINT8 *signature, BUINT32 *signatureLen, BUINT8 *Prefix);

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
BOAT_RESULT BoatHash(const BoatHashAlgType type, const BUINT8 *input, BUINT32 inputLen, BUINT8 *hashed, BUINT8 *hashedLen, void *rsvd);
/*!****************************************************************************
 * @brief 
 *   elliptic curve signature function.
 * 
 * @details
 *   elliptic curve signature function.
 *
 * @param[in] prikeyCtx 
 *   Private key context.
 *
 * @param[in] digest
 *   pointer to disgest message.
 *
 * @param[in] digestLen 
 *   the length of digiest message.
 *
 * @param[out] signatureResult 
 *   The buffer to stored signature result.
 *
 * @param rsvd 
 *   Reserved for futrue. 
 *
 * @return 
 *   Return \c BOAT_SUCCESS if generate success; otherwise return a negative error code.
 ******************************************************************************/
BOAT_RESULT BoatSignature(BoatKeypairPriKeyCtx prikeyCtx, 
						  const BUINT8 *digest, BUINT32 digestLen, 
						  BoatSignatureResult *signatureResult, void *rsvd);
// /**
//  * @description:
//  * 	This function get pubkey from prikey;
//  * @param {BoatKeypairPriKeyType} type
//  * 	now only support ecdsa and will support other alg such as SM
//  * @param {BUINT8} *prikey
//  * 	prikey inut
//  * @param {BUINT32} prikeyLen
//  * 	length of prikey
//  * @param {BUINT8} *pubkey
//  * 	pubkey output
//  * @param {BUINT32} *pubkeyLen
//  * 	length of pubkey
//  * @return {*}
//  *  This function returns BoAT_SUCCESS if successfully executed.
//  *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
//  *  for details.
//  * @author: aitos
//  */
BOAT_RESULT BoAT_getPubkey(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format, BUINT8 *prikey, BUINT32 prikeyLen, BUINT8 *pubkey, BUINT32 *pubkeyLen);

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
BOAT_RESULT BoAT_recover_pubkey(BoatKeypairPriKeyType type, BoatKeypairPriKeyFormat format, BUINT8 *pubkey, const BUINT8 *digest, const BUINT8 *signature, BUINT8 Prefix);

/**
 * @description:
 *  This cunction delete prikey by index.
 * @param {BUINT8} index
 * @return {*}
 *  This function returns BoAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 */
BOAT_RESULT BoAT_DeletePrikeyByIndex(BUINT8 index);

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
BOAT_RESULT BoAT_Keystore_Write_Binary_secure(BUINT8 *data, BUINT32 datalen, BUINT32 offset);

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
BOAT_RESULT BoAT_Keystore_read_Binary_secure(BUINT8 *data, BUINT32 *datalen, BUINT32 offset);

BOAT_RESULT BoatPort_keyCreate(const BoatKeypairPriKeyCtx_config *config, BoatKeypairDataCtx *pkCtx);

/******************************************************************************
                              BOAT AES WARPPER
*******************************************************************************/
BOAT_RESULT BoatAesEncrypt(BUINT8 iv[16], const BUINT8 *key, const BUINT8 *input, size_t length, BUINT8 *output);

BOAT_RESULT BoatAesDecrypt(BUINT8 iv[16], const BUINT8 *key, const BUINT8 *input, size_t length, BUINT8 *output);

#endif