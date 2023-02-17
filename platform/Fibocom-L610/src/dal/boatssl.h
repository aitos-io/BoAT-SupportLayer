/**
****************************************************************************************
* @FilePath: boat_ssl.h
* @Author: aitos
* @Date: 2023-02-15 11:49:42
* @LastEditors:
* @LastEditTime: 2023-02-15 11:49:42
* @Descripttion:
****************************************************************************************
*/
#ifndef _BOAT_SSL_H_
#define _BOAT_SSL_H_

#if (PROTOCOL_USE_HLFABRIC == 1 || PROTOCOL_USE_CHAINMAKER_V1 == 1 || PROTOCOL_USE_CHAINMAKER_V2 == 1 || BOAT_PROTOCOL_USE_HWBCS == 1)
/******************************************************************************
                              BOAT SOCKET WARPPER
                            THIS ONLY USED BY FABRIC OR CHAINMAKER
*******************************************************************************/
#if (BOAT_PROTOCOL_USE_HWBCS == 1)
static X509 *buffer2x509(const uint8_t *cert, size_t len);

static EVP_PKEY *buffer2evpkey(const uint8_t *key, size_t key_len);

BOAT_RESULT boat_find_subject_common_name(const BCHAR *cert, const BUINT32 certlen, BCHAR *value, size_t value_length);
#endif
BSINT32 BoatConnect(const BCHAR *address, void *rsvd);

#if (BOAT_TLS_SUPPORT == 1)
BOAT_RESULT BoatTlsInit(const BCHAR *address, const BCHAR *hostName, const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey,
                        const BoatFieldVariable clientCert, BSINT32 *socketfd, void **tlsContext, void *rsvd);
#endif

BSINT32 BoatSend(BSINT32 sockfd, void *tlsContext, const void *buf, size_t len, void *rsvd);

BSINT32 BoatRecv(BSINT32 sockfd, void *tlsContext, void *buf, size_t len, void *rsvd);

void BoatClose(BSINT32 sockfd, void **tlsContext, void *rsvd);
#endif /* #if (PROTOCOL_USE_HLFABRIC == 1) */

#endif