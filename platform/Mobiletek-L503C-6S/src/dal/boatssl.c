/**
****************************************************************************************
* @FilePath: boat_ssl.c
* @Author: aitos
* @Date: 2023-02-15 11:49:33
* @LastEditors:
* @LastEditTime: 2023-02-15 11:49:33
* @Descripttion:
****************************************************************************************
*/

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"
#include "boatdal.h"
#include "boatplatformdal.h"
#include "boatplatformosal.h"

#include "ol_flash_fs.h"
#include "mbtk_ssl_hal.h"
#include "mbtk_socket_api.h"

// #if (PROTOCOL_USE_HLFABRIC == 1 || PROTOCOL_USE_CHAINMAKER_V1 == 1 || PROTOCOL_USE_CHAINMAKER_V2 == 1 || BOAT_PROTOCOL_USE_HWBCS == 1)
/******************************************************************************
                              BOAT SOCKET WARPPER
                            THIS ONLY USED BY FABRIC OR CHAINMAKER
*******************************************************************************/
#if (BOAT_PROTOCOL_USE_HWBCS == 1)
static X509 *buffer2x509(const uint8_t *cert, size_t len)
{
    /*read the cert and decode it*/
    BIO *b = BIO_new_mem_buf((void *)cert, len);
    if (NULL == b)
    {
        BoatLog(BOAT_LOG_NORMAL, "read cert data fail");
        return NULL;
    }
    X509 *x509 = PEM_read_bio_X509(b, NULL, NULL, NULL);
    if (NULL == x509)
    {
        BoatLog(BOAT_LOG_NORMAL, "PEM_read_bio_X509 fail");
        BIO_free(b);
        return NULL;
    }
    BIO_free(b);
    return x509;
}

static EVP_PKEY *buffer2evpkey(const uint8_t *key, size_t key_len)
{
    BIO *b = BIO_new_mem_buf((void *)key, key_len);
    if (NULL == b)
    {
        return NULL;
    }

    // see openssl's PEM_read_bio_PrivateKey interface
    EVP_PKEY *evpkey = PEM_read_bio_PrivateKey(b, NULL, NULL, NULL);
    if (NULL == evpkey)
    {
        BIO_free(b);
        return NULL;
    }
    BIO_free(b);
    return evpkey;
}

BOAT_RESULT boat_find_subject_common_name(const BCHAR *cert, const BUINT32 certlen, BCHAR *value, size_t value_length)
{
    BOAT_RESULT retval = BOAT_SUCCESS;
    X509 *x509Cert = NULL;
    X509_NAME *subject = NULL;
    // retval = boat_get_x509cert(cert, certlen, &x509Cert);
    x509Cert = buffer2x509(cert, certlen);
    if (x509Cert == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "read x509cert fail");
        return BOAT_ERROR;
    }
    // Version = X509_get_version(x509Cert);
    // BoatLog(BOAT_LOG_NORMAL, "X509 Version:%d\n", Version);
    subject = X509_get_subject_name(x509Cert);
    retval = X509_NAME_get_text_by_NID(subject, NID_commonName, value, value_length);
    X509_free(x509Cert);

    return retval;
}
#endif
/**
****************************************************************************************
* @brief:
*  This function connect to address.
* @param[in] *address
*  URL of the network wanted to connect.
* @param[in] *rsvd
*  unused untill now
* @return
*  This function will return socketID if executed successfully.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BSINT32 BoatConnect(const BCHAR *address, void *rsvd)
{

    return BOAT_TRUE;
}

#if (BOAT_TLS_SUPPORT == 1)
/**
****************************************************************************************
* @brief:
* This function initialize TLS connection. This initialization will set hostname and CA cert of
* server's TLS cert or client's TLS cert or key.
* This function will connect to server and complete HandShake process.
* @param[in] *address
* URL of server network
* @param[in] *hostName
* CN of server's TLS cert
* @param[in] caChain
* CA cert of server's TLS cert
* @param[in] clientPrikey
* client's tls prikey
* @param[in] clientCert
* client's tls cert
* @param[out] *socketfd
* socketfd of the connection between client and server
* @param[out] **tlsContext
* output of tls context
* @param[in] *rsvd
* unused untill now
* @return
*  This function will return BOAT_SUCCESS if executed successfully.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT BoatTlsInit(const BCHAR *address, const BCHAR *hostName, const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey,
                        const BoatFieldVariable clientCert, BSINT32 *socketfd, boatSSlCtx **tlsContext, void *rsvd)
{

	SSLCtx *tlsContext_ptr = BoatMalloc(sizeof(SSLCtx));

    if(tlsContext_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "malloc failed." );
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }

	SSLConfig conf_ssl ={0};
	char *ptr = NULL;
	char ip[64];
    char port[8];
	
	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	ptr = strchr(address, ':');
    if (NULL == ptr)
    {
        BoatLog(BOAT_LOG_CRITICAL, "invalid address:%s.", address);
        BoatFree(tlsContext_ptr);
        return BOAT_ERROR;
    }
	BoatLog(BOAT_LOG_CRITICAL, "address:%s", address);

	memset(ip  , 0      , sizeof(ip));
    memset(port, 0      , sizeof(port));
    memcpy(ip  , address, (int)(ptr - address));
    memcpy(port, ptr + 1, strlen(address) - (int)(ptr - address));

	conf_ssl.profileIdx = 0;
	conf_ssl.dbgLevel = 3;
	conf_ssl.protocol = 0;
	conf_ssl.serverName = ip;
	conf_ssl.serverPort = atoi(port);
	conf_ssl.verify = SSL_VERIFY_MODE_NONE;
	conf_ssl.vsn = SSL_VSN_ALL;
	conf_ssl.cert.from = SSL_CERT_FROM_BUF;
	conf_ssl.cert.path.rootCA = caChain.field_ptr;
	conf_ssl.cert.path.clientCert = clientCert.field_ptr;
	conf_ssl.cert.path.clientKey = clientPrikey.field_ptr;
	ol_ssl_set_config(tlsContext_ptr, &conf_ssl);

	result = ol_ssl_ctx_init(tlsContext_ptr);
	BoatLog(BOAT_LOG_CRITICAL, "ol_ssl_ctx_init result:%d",result);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to ol_ssl_ctx_init.");
        boat_throw(result, BoatTlsInit_exception);
    }

	result = ol_ssl_handshake(tlsContext_ptr, 60*1000);
	BoatLog(BOAT_LOG_CRITICAL, "ol_ssl_handshake result:%d",result);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to ol_ssl_handshake.");
        boat_throw(result, BoatTlsInit_exception);
    }
	/* boat catch handle */
	boat_catch(BoatTlsInit_exception)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
		ol_ssl_shutdown(tlsContext_ptr);
		ol_ssl_ctx_deinit(tlsContext_ptr);
		BoatFree(tlsContext_ptr);
	}
	*tlsContext = tlsContext_ptr;
	return result;
}
#endif

/**
****************************************************************************************
* @brief:
* This function send data with length equal to len to server.
* @param[in] sockfd
* socketfd of this connection.
* @param[in] *tlsContext
* tls context between client and server
* @param[in] *buf
* point to the data wanted to send
* @param[in] len
* length of the data wanted to send
* @param[in] *rsvd
* unused untill now
* @return
*  This function will return length of sent data if executed successfully.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BSINT32 BoatSend(BSINT32 sockfd, boatSSlCtx *tlsContext, const BUINT8 *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1)
    //! @todo BOAT_TLS_SUPPORT implementation in crypto default.
    //! @todo BOAT_HLFABRIC_TLS_SUPPORT implementation in crypto default.
    if ((SSLCtx *)tlsContext == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "tlsContext can't be NULL.");
		return -1;
	}

	return ol_ssl_write((SSLCtx *)tlsContext, buf, len);

#else
    return send(sockfd, buf, len, 0);
#endif
}


/**
****************************************************************************************
* @brief:
* This function receive data from server
* @param[in] sockfd
* socketfd of this connection.
* @param[in] *tlsContext
* tls context between client and server
* @param[in] *buf
* point to the data wanted to receive
* @param[in] len
* length of the buf to store receiving data
* @param[in] *rsvd
* unused untill now
* @return
*  This function will return length of received data if executed successfully.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BSINT32 BoatRecv(BSINT32 sockfd, boatSSlCtx *tlsContext, BUINT8 *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1)
    //! @todo BOAT_TLS_SUPPORT implementation in crypto default.
    if ((SSLCtx *)tlsContext == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "tlsContext can't be NULL.");
		return -1;
	}

	return ol_ssl_read((SSLCtx *)tlsContext, buf, len);
#else
    return recv(sockfd, buf, len, 0);
#endif
}
/**
****************************************************************************************
* @brief:
* This function close the connection between client and server.
* This function must release the tls context.
* @param[in] sockfd
* socketfd of this connection.
* @param[in] **tlsContext
* tls context between client and server
* @param[in] *rsvd
* unused untill now
* @return
* This function has no returned value.
****************************************************************************************
*/
void BoatClose(BSINT32 sockfd, boatSSlCtx **tlsContext, void *rsvd)
{
    // close(sockfd);
#if (BOAT_TLS_SUPPORT == 1)
    // free tls releated
    //! @todo BOAT_TLS_SUPPORT implementation in crypto default.
    if ((SSLCtx *)*tlsContext == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "tlsContext can't be NULL.");
		return ;
	}
	
    ol_ssl_shutdown((SSLCtx *)*tlsContext);
	ol_ssl_ctx_deinit((SSLCtx *)*tlsContext);
    if(*tlsContext != NULL)
    {
        BoatFree(*tlsContext);
    }
	return;
#else
    close(sockfd);
#endif
}
// #endif /* #if (PROTOCOL_USE_HLFABRIC == 1) */