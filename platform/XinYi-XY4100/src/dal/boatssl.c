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

//#include "qcloud_iot_import.h"
#include "xy_ssl_api.h"
#include "xy_tcpip_api.h"

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
    int                 connectfd;
    char                ip[64];
    char                port[8];
    char                *ptr = NULL;
    struct hostent      *he; 
    struct sockaddr_in  server;
    struct sockaddr     localaddr;
    struct sockaddr_in  *localaddr_ptr;
    socklen_t           addrlen = sizeof(struct sockaddr);

    (void)rsvd;

    ptr = strchr(address, ':');
    if (NULL == ptr)
    {
        BoatLog(BOAT_LOG_CRITICAL, "invalid address:%s.", address);
        return -1;
    }

    memset(ip  , 0      , sizeof(ip));
    memset(port, 0      , sizeof(port));
    memcpy(ip  , address, (int)(ptr - address));
    memcpy(port, ptr + 1, strlen(address) - (int)(ptr - address));


	/* Create TCP socket */
	if ((connectfd = xy_socket_by_host(ip, Sock_IPv46, IPPROTO_TCP, 0, atoi(port), NULL)) == -1) {
		BoatLog(BOAT_LOG_CRITICAL, "failed to connect with TCP server: %s:%d", ip, atoi(port));
		return -1;
	}
	else{
		return connectfd;
	}

}


#if (BOAT_TLS_SUPPORT == 1)
#include "xy_fs.h"
#include "xy_ssl_api.h"
#include "at_ssl_config.h"

#define BOAT_TLS_KEYS_PATH "D:/tempKeys"
#define BOAT_TLS_CA_KEY "D:/tempKeys/caKey.txt"
#define BOAT_TLS_CLIENT_KEY "D:/tempKeys/clientKey.txt"
#define BOAT_TLS_CLIENT_CERT "D:/tempKeys/clientCert.txt"

BOAT_RESULT writeTlsKeys2FS(const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey,const BoatFieldVariable clientCert)
{
	int32_t ret = 0;
	//remove all tls key files first
	xy_fs_remove_dirlist(BOAT_TLS_KEYS_PATH);

	ret = xy_fmkdir(BOAT_TLS_KEYS_PATH);
	//if(ret != 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "xy_fmkdir %s err(%d)",BOAT_TLS_KEYS_PATH,ret);
	//	return ret;
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

/* mbedTLS header include */
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#include "mbedtls/asn1.h"

#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/bignum.h"
#include "mbedtls/asn1write.h"
#include "mbedtls/error.h"

static mbedtls_ssl_context *g_ssl_ctx = NULL;
static mbedtls_ssl_config *g_ssl_cfg = NULL;
static mbedtls_x509_crt *g_ssl_ca_crt = NULL;
static mbedtls_net_context *g_ssl_net = NULL;

#if (BOAT_TLS_IDENTIFY_CLIENT == 1)
static mbedtls_x509_crt *g_ssl_client_crt = NULL;
static mbedtls_pk_context *g_ssl_client_key = NULL;
#endif


BOAT_RESULT BoatTlsInit(const BCHAR *address, const BCHAR *hostName, const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey,
                        const BoatFieldVariable clientCert, BSINT32 *socketfd, boatSSlCtx **tlsContext, void *rsvd)
{

    BOAT_RESULT result = BOAT_SUCCESS;
	mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

	boat_try_declare;

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

	//connect first
	int ret = BoatConnect(address, NULL);
    if (ret == -1)
    {
        BoatLog(BOAT_LOG_NORMAL, "socket connect fail ");
        return BOAT_ERROR;
    }
	BoatLog(BOAT_LOG_NORMAL,"socket connect success ");
    *socketfd = ret;


	g_ssl_ctx = BoatMalloc(sizeof(mbedtls_ssl_context));
	if (g_ssl_ctx == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate ssl_context.");
		result =  BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		boat_throw(result, BoatTlsInit_exception);
	}

	g_ssl_cfg = BoatMalloc(sizeof(mbedtls_ssl_config));
	if (g_ssl_cfg == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate ssl_config.");
		result =  BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		boat_throw(result, BoatTlsInit_exception);
	}
	g_ssl_ca_crt = BoatMalloc(sizeof(mbedtls_x509_crt));
	if (g_ssl_ca_crt == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate x509_crt.");
		result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		boat_throw(result, BoatTlsInit_exception);
	}
	g_ssl_net = BoatMalloc(sizeof(mbedtls_net_context));
	if (g_ssl_net == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate net_context.");
		result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		boat_throw(result, BoatTlsInit_exception);
	}

#if (BOAT_TLS_IDENTIFY_CLIENT == 1)

	g_ssl_client_crt= BoatMalloc(sizeof(mbedtls_x509_crt));
	if (g_ssl_client_crt == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate x509_crt.");
		result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		boat_throw(result, BoatTlsInit_exception);
	}

	g_ssl_client_key = BoatMalloc(sizeof(mbedtls_pk_context));
	if (g_ssl_client_key == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to allocate pk.");
		result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		boat_throw(result, BoatTlsInit_exception);
	}
#endif

	mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_net_init(g_ssl_net);
	mbedtls_ssl_init(g_ssl_ctx);
	mbedtls_ssl_config_init(g_ssl_cfg);
	mbedtls_x509_crt_init(g_ssl_ca_crt);

#if (BOAT_TLS_IDENTIFY_CLIENT == 1)
	mbedtls_x509_crt_init(g_ssl_client_crt);
	mbedtls_pk_init(g_ssl_client_key);
#endif
	result = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
    if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to execute ctr_drbg_seed.");
        boat_throw(result, BoatTlsInit_exception);
    }

	result = mbedtls_ssl_config_defaults(g_ssl_cfg, MBEDTLS_SSL_IS_CLIENT,
										 MBEDTLS_SSL_TRANSPORT_STREAM,
										 MBEDTLS_SSL_PRESET_DEFAULT);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute ssl_config_defaults.\n");
        boat_throw(result, BoatTlsInit_exception);
    }

	mbedtls_ssl_conf_rng(g_ssl_cfg, mbedtls_ctr_drbg_random, &ctr_drbg);

	result += mbedtls_x509_crt_parse(g_ssl_ca_crt, caChain.field_ptr, caChain.field_len);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute x509_crt_parse: -%x\n", -result);
        boat_throw(result, BoatTlsInit_exception);
    }

	mbedtls_ssl_conf_ca_chain(g_ssl_cfg, g_ssl_ca_crt, NULL);
#if (BOAT_TLS_IDENTIFY_CLIENT == 1)
	result += mbedtls_x509_crt_parse(g_ssl_client_crt, clientCert.field_ptr, clientCert.field_len);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute x509_crt_parse: -%x\n", -result);
        boat_throw(result, BoatTlsInit_exception);
    }

	result += mbedtls_pk_parse_key(g_ssl_client_key,clientPrikey.field_ptr,clientPrikey.field_len,NULL,0,NULL,NULL);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute pk_parse: -%x\n", -result);
        boat_throw(result, BoatTlsInit_exception);
    }

	result += mbedtls_ssl_conf_own_cert(g_ssl_cfg,g_ssl_client_crt,g_ssl_client_key);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute conf_own_cert: -%x\n", -result);
        boat_throw(result, BoatTlsInit_exception);
    }
#endif
	mbedtls_ssl_conf_authmode(g_ssl_cfg, MBEDTLS_SSL_VERIFY_NONE); //2
	mbedtls_ssl_conf_ciphersuites(g_ssl_cfg, mbedtls_ssl_list_ciphersuites());

	result = mbedtls_ssl_setup(g_ssl_ctx,g_ssl_cfg);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute ssl_setup: -%x\n", -result);
        boat_throw(result, BoatTlsInit_exception);
    }

	result = mbedtls_ssl_set_hostname(g_ssl_ctx, ip);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute hostname_set.\n");
        boat_throw(result, BoatTlsInit_exception);
    }

	g_ssl_net->fd = ret;

	mbedtls_ssl_set_bio(g_ssl_ctx,g_ssl_net, mbedtls_net_send, mbedtls_net_recv, NULL);

	result = mbedtls_ssl_handshake(g_ssl_ctx);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute ssl_handshake: -%x\n", -result);
        boat_throw(result, BoatTlsInit_exception);
    }
	BoatLog(BOAT_LOG_NORMAL, "ret = ssl_handshake SUCCESSED!");

	(*tlsContext)->sslCtx = g_ssl_ctx;

	/* boat catch handle */
	boat_catch(BoatTlsInit_exception)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
		if(g_ssl_ctx)
		{
			mbedtls_ssl_free(g_ssl_ctx);
			BoatFree(g_ssl_ctx);
			g_ssl_ctx = NULL;
		}
			
		if(g_ssl_net)
		{
			mbedtls_net_free(g_ssl_net);
			BoatFree(g_ssl_net);
			g_ssl_net = NULL;
		}
			
		if(g_ssl_cfg)
		{
			mbedtls_ssl_config_free(g_ssl_cfg);
			BoatFree(g_ssl_cfg);
			g_ssl_cfg = NULL;
		}
			
		if(g_ssl_ca_crt)
		{
			mbedtls_x509_crt_free(g_ssl_ca_crt);
			BoatFree(g_ssl_ca_crt);
			g_ssl_ca_crt = NULL;
		}	
#if (BOAT_TLS_IDENTIFY_CLIENT == 1)
		if(g_ssl_client_crt)
		{
			mbedtls_x509_crt_free(g_ssl_client_crt);
			BoatFree(g_ssl_client_crt);
			g_ssl_client_crt = NULL;
		}

		if(g_ssl_client_key)
		{
			mbedtls_pk_free(g_ssl_client_key);
			BoatFree(g_ssl_client_key);
			g_ssl_client_key = NULL;
		}
#endif

	}
	mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);

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

	return mbedtls_ssl_write(tlsContext->sslCtx,buf,len);

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
	return mbedtls_ssl_read(tlsContext->sslCtx,buf,len);
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
    if (*tlsContext != NULL)
	{
		BoatLog(BOAT_LOG_NORMAL,"run BoatClose...");
		close(sockfd);

		if(g_ssl_ctx)
		{
			mbedtls_ssl_free(g_ssl_ctx);
			BoatFree(g_ssl_ctx);
			g_ssl_ctx = NULL;
		}
		(*tlsContext)->sslCtx = NULL;
		if(g_ssl_net)
		{
			mbedtls_net_free(g_ssl_net);
			BoatFree(g_ssl_net);
			g_ssl_net = NULL;
		}
			
		if(g_ssl_cfg)
		{
			mbedtls_ssl_config_free(g_ssl_cfg);
			BoatFree(g_ssl_cfg);
			g_ssl_cfg = NULL;
		}
			
		if(g_ssl_ca_crt)
		{
			mbedtls_x509_crt_free(g_ssl_ca_crt);
			BoatFree(g_ssl_ca_crt);
			g_ssl_ca_crt = NULL;
		}	
#if (BOAT_TLS_IDENTIFY_CLIENT == 1)
		if(g_ssl_client_crt)
		{
			mbedtls_x509_crt_free(g_ssl_client_crt);
			BoatFree(g_ssl_client_crt);
			g_ssl_client_crt = NULL;
		}

		if(g_ssl_client_key)
		{
			mbedtls_pk_free(g_ssl_client_key);
			BoatFree(g_ssl_client_key);
			g_ssl_client_key = NULL;
		}
#endif
	}
#else
    close(sockfd);
#endif
}
// #endif /* #if (PROTOCOL_USE_HLFABRIC == 1) */