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

#include "qcloud_iot_import.h"
#include "xy_ssl_api.h"

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
	char ip[64];
	char port[8];
	char *ptr = NULL;

	(void)rsvd;

	ptr = strchr(address, ':');
	if (NULL == ptr)
	{
		BoatLog(BOAT_LOG_CRITICAL, "invalid address:%s.", address);
		return -1;
	}

	memset(ip, 0, sizeof(ip));
	memset(port, 0, sizeof(port));
	memcpy(ip, address, (int)(ptr - address));
	memcpy(port, ptr + 1, strlen(address) - (int)(ptr - address));

	return HAL_TCP_Connect(ip,atoi(port));

}


#if (BOAT_TLS_SUPPORT == 1)
#include "xy_fs.h"
#include "xy_ssl_api.h"
#include "at_ssl_config.h"

#define BOAT_TLS_KEYS_PATH "U:/user_fsdisk/boat/tempKeys"
#define BOAT_TLS_CA_KEY "U:/user_fsdisk/boat/tempKeys/caKey.txt"
#define BOAT_TLS_CLIENT_KEY "U:/user_fsdisk/boat/tempKeys/clientKey.txt"
#define BOAT_TLS_CLIENT_CERT "U:/user_fsdisk/boat/tempKeys/clientCert.txt"

BOAT_RESULT writeTlsKeys2FS(const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey,const BoatFieldVariable clientCert)
{
	int32_t ret = 0;
	//remove all tls key files first
	xy_fs_remove_dirlist(BOAT_TLS_KEYS_PATH);

	ret = xy_fmkdir(BOAT_TLS_KEYS_PATH);
	if(ret != 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "xy_fmkdir %s err(%d)",BOAT_TLS_KEYS_PATH,ret);
		return ret;
	}

	/* 1.save caChain to fs */
	XY_FILE  *caKeyFile = xy_fopen(BOAT_TLS_CA_KEY,"w+");
	if(caKeyFile == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "fopen %s err",BOAT_TLS_CA_KEY);
		return -1;
	}

	int32_t len = xy_fwrite(caChain.field_ptr,sizeof(BUINT8),caChain.field_len,caKeyFile);
	if(len != caChain.field_len)
	{
		BoatLog(BOAT_LOG_CRITICAL, "fwrite %s err",BOAT_TLS_CA_KEY);
		ret = -1;
		goto end1;
	}

	/* 2.save clientPrikey to fs */
	XY_FILE *clientKeyFile = xy_fopen(BOAT_TLS_CLIENT_KEY,"w+");
	if(clientKeyFile == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "fopen %s err",BOAT_TLS_CLIENT_KEY);
		ret = -1;
		goto end1;
	}

	len = xy_fwrite(clientPrikey.field_ptr,sizeof(BUINT8),clientPrikey.field_len,clientKeyFile);
	if(len != clientPrikey.field_len)
	{
		BoatLog(BOAT_LOG_CRITICAL, "fwrite %s err",BOAT_TLS_CLIENT_KEY);
		ret = -1;
		goto end2;
	}

	/* 3.save clientCert to fs */
	XY_FILE *clientCertFile = xy_fopen(BOAT_TLS_CLIENT_CERT,"w+");
	if(clientCertFile == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "fopen %s err",BOAT_TLS_CLIENT_CERT);
		ret = -1;
		goto end2;
	}

	len = xy_fwrite(clientCert.field_ptr,sizeof(BUINT8),clientCert.field_len,clientCertFile);
	if(len != clientCert.field_len)
	{
		BoatLog(BOAT_LOG_CRITICAL, "fwrite %s err",BOAT_TLS_CLIENT_CERT);
		ret = -1;
		goto end3;
	}

	ret = 0;

end3:
	xy_fclose(clientCertFile);
end2:
	xy_fclose(clientKeyFile);
end1:
	xy_fclose(caKeyFile);
	
	return ret;
}
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

    BOAT_RESULT result = BOAT_SUCCESS;
	BSINT32 sockfd;
	// SCSslCtx_t *tlsContext_ptr = BoatMalloc(sizeof(SCSslCtx_t));
	boat_try_declare;

	// //write ssl keys to fileSystem
	// result = writeTlsKeys2FS(caChain,clientPrikey,clientCert);
	// if(result != BOAT_SUCCESS)
	// {
	// 	BoatLog(BOAT_LOG_NORMAL, "writeTlsKeys2FS fail ");
	// 	return result;
	// }
	sockfd = BoatConnect(address, NULL);
	if (sockfd == 0)
	{
		BoatLog(BOAT_LOG_NORMAL, "socket connect fail ");
		boat_throw(sockfd, BoatTlsInit_exception);
	}
	else
	{
		BoatLog(BOAT_LOG_NORMAL, "socket connect success!!! ");
	}
	BoatLog(BOAT_LOG_NORMAL, "boat hostName = %s ", hostName);

	char ip[64];
	char port[8];
	char *ptr = NULL;

	(void)rsvd;

	ptr = strchr(address, ':');
	if (NULL == ptr)
	{
		BoatLog(BOAT_LOG_CRITICAL, "invalid address:%s.", address);
		return -1;
	}

	memset(ip, 0, sizeof(ip));
	memset(port, 0, sizeof(port));
	memcpy(ip, address, (int)(ptr - address));
	memcpy(port, ptr + 1, strlen(address) - (int)(ptr - address));

	ssl_config_t *sslConf = xy_malloc(sizeof(ssl_config_t));

	sslConf->protocol = 0; //TLS
	sslConf->vsn = SSL_VERSION_DEFAULT; 
	sslConf->cipher_suites = SSL_CIPHERSUITE_DEFAULT;
	sslConf->ignore_local_time = SSL_IGNORELOCALTIME_DEFAULT;
	sslConf->negotiate_time = SSL_NEGOTIATETIME_DEFAULT;
	sslConf->session_cache_enable = SSL_SESSION_CACHE_DEFAULT;

	sslConf->cert.from = SSL_CERT_FROM_BUF;
	sslConf->cert.path.rootCA = caChain.field_ptr ;

#if (BOAT_TLS_IDENTIFY_CLIENT == 1)
	sslConf->cert.path.clientKey = clientPrikey.field_ptr;
	sslConf->cert.path.clientCert = clientCert.field_ptr;
#endif

	void *sslCtx = ssl_init(sslConf);
	if(sslCtx == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "ssl init fail ");
		boat_throw(-1, BoatTlsInit_exception);
	}

	int ret = ssl_handshake(sslCtx,1,hostName,atoi(port),1000);
	if(ret != 0)
	{
		BoatLog(BOAT_LOG_NORMAL, "ssl handshake fail ");
		boat_throw(ret, BoatTlsInit_exception);
	}

	boat_catch(BoatTlsInit_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
		xy_fs_remove_dirlist(BOAT_TLS_KEYS_PATH);
		ssl_deinit(&sslCtx);
		xy_free(sslConf);
		sslConf = NULL;
		sslCtx = NULL;
	}

	*socketfd = ssl_get_socket_fd(sslCtx);

	*tlsContext = sslCtx;

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

	return ssl_write(tlsContext,buf,len,1000);

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
	return ssl_read(tlsContext,buf,len,1000);
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
    ssl_shutdown(*tlsContext,500);
    ssl_deinit(tlsContext);
	*tlsContext = NULL;
	xy_fs_remove_dirlist(BOAT_TLS_KEYS_PATH);
	return;
#else
    close(sockfd);
#endif
}
// #endif /* #if (PROTOCOL_USE_HLFABRIC == 1) */