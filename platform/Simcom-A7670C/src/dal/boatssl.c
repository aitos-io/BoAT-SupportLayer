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

#include "simcom_ssl.h"
#include "simcom_tcpip.h"
#include "simcom_tcpip_old.h"
#include "simcom_debug.h"
#include "simcom_os.h"
#include "simcom_common.h"
#include "scfw_inet.h"

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
	INT32 sockfd = -1;
	SChostent *host_entry = NULL;
	SCsockAddrIn server;

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

	// if (-1 == sAPI_TcpipPdpActive(1, 1))
	// {
	// 	BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipPdpActive() error");
	// 	return -1;
	// }

	sockfd = sAPI_TcpipSocket(SC_AF_INET, SC_SOCK_STREAM, SC_IPPROTO_TCP);
	BoatLog(BOAT_LOG_CRITICAL, "BoatConnect sockfd[%d]", sockfd);
	if (sockfd < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipSocket() error");
		// if (-1 == sAPI_TcpipPdpDeactive(1, 1))
		// {
		// 	BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipPdpDeactive() error");
		// }
		return -1;
	}

	host_entry = sAPI_TcpipGethostbyname(ip);
	// host_entry = sAPI_TcpipGethostbyname((INT8 *)"www.baidu.com");
	if (host_entry == NULL)
	{
		sAPI_SslClose(0);
		sAPI_TcpipClose(sockfd);
		BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipGethostbyname() error");
		// if (-1 == sAPI_TcpipPdpDeactive(1, 1))
		// {
		// 	BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipPdpDeactive() error");
		// }
		return -1;
	}

	server.sin_family = SC_AF_INET;
	server.sin_port = sAPI_TcpipHtons(atoi(port));
	// server.sin_port = sAPI_TcpipHtons(443);
	server.sin_addr.s_addr = *(UINT32 *)host_entry->h_addr_list[0];

	BoatLog(BOAT_LOG_CRITICAL, "start connect!!!");
	if (sAPI_TcpipConnect(sockfd, (SCsockAddr *)&server, sizeof(SCsockAddr)) != 0)
	{
		sAPI_SslClose(0);
		sAPI_TcpipClose(sockfd);
		BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipConnect() error");
		// if (-1 == sAPI_TcpipPdpDeactive(1, 1))
		// {
		// 	BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipPdpDeactive() error");
		// }
		return -1;
	}

	return sockfd;
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

    BOAT_RESULT result = BOAT_SUCCESS;
	BSINT32 sockfd;
	// SCSslCtx_t *tlsContext_ptr = BoatMalloc(sizeof(SCSslCtx_t));
	boat_try_declare;

	sockfd = BoatConnect(address, NULL);
	if (sockfd < BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_NORMAL, "socket connect fail ");
		boat_throw(sockfd, BoatTlsInit_exception);
	}
	else
	{
		BoatLog(BOAT_LOG_NORMAL, "socket connect success!!! ");
	}
	BoatLog(BOAT_LOG_NORMAL, "boat hostName = %s ", hostName);
	// memset(tlsContext_ptr,0,sizeof(tlsContext_ptr));
	SCSslCtx_t sslCtx;

	memset(&sslCtx, 0, sizeof(SCSslCtx_t));
	sslCtx.ClientId = 0;
	sslCtx.fd = sockfd;
	sslCtx.ciphersuitesetflg = 0;
	// sslCtx.ciphersuite[0] = 0x2C;
	sslCtx.ssl_version = SC_SSL_CFG_VERSION_TLS12;
	// sslCtx.enable_SNI = 0;
	sslCtx.enable_SNI = 1;
	sslCtx.ipstr = (INT8 *)hostName;
	sslCtx.root_ca = (INT8 *)caChain.field_ptr;
	sslCtx.root_ca_len = caChain.field_len;
	sslCtx.ignore_local_time = 1;
	// sslCtx.auth_mode = SC_SSL_CFG_VERIFY_MODE_REQUIRED;
	// sslCtx.auth_mode = SC_SSL_CFG_VERIFY_MODE_CLIENT;
	sslCtx.auth_mode = SC_SSL_CFG_VERIFY_MODE_REQUIRED;
#if (BOAT_TLS_IDENTIFY_CLIENT == 1)
	sslCtx.auth_mode = SC_SSL_CFG_VERIFY_MODE_SERVER_CLIENT;
	sslCtx.client_cert = (INT8 *)clientCert.field_ptr;
	sslCtx.client_cert_len = clientCert.field_len;
	sslCtx.client_key = (INT8 *)clientPrikey.field_ptr;
	sslCtx.client_key_len = clientPrikey.field_len;
#endif

	result = sAPI_SslHandShake(&sslCtx);
	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_NORMAL, "sAPI_SslHandShake fail[%d]", result);
		boat_throw(result, BoatTlsInit_exception);
	}
	else
	{
		BoatLog(BOAT_LOG_NORMAL, "sAPI_SslHandShake success!!!");
	}

	boat_catch(BoatTlsInit_exception)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
		result = boat_exception;
		sAPI_SslClose(0);
		sAPI_TcpipClose(sockfd);
		if (-1 == sAPI_TcpipPdpDeactive(1, 1))
		{
			BoatLog(BOAT_LOG_CRITICAL, "sAPI_TcpipPdpDeactive() error");
		}
		// BoatFree(tlsContext_ptr);
		// tlsContext_ptr = NULL;
	}
	*tlsContext = &sslCtx;
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

	return sAPI_SslSend(0,(INT8 *)buf,len);

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
	return sAPI_SslRead(0, (INT8 *)buf, len);
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
    
    sAPI_SslClose(0);

	return;
#else
    close(sockfd);
#endif
}
// #endif /* #if (PROTOCOL_USE_HLFABRIC == 1) */