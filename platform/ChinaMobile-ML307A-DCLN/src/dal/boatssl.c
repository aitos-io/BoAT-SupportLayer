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
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"
#include "boatdal.h"
//#include "httpclient.h"
#include "boatplatformdal.h"

#include "cm_os.h"
#include "cm_ssl.h"
#include "sockets.h"
#include "cm_asocket.h"
#include "stdlib.h"

#define SSL_ID  0

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
	int connectfd;
	char ip[64];
	char port[8];
	char *ptr = NULL;
	// struct hostent *he;
	struct sockaddr_in server;
	struct sockaddr localaddr;
	struct sockaddr_in *localaddr_ptr;
	socklen_t addrlen = sizeof(struct sockaddr);

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

	// if ((he = gethostbyname(ip)) == NULL)
	// {
	// 	BoatLog(BOAT_LOG_CRITICAL, "gethostbyname() error");
	// 	return -1;
	// }

	if ((connectfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "socket() error");
		return -1;
	}

	struct timeval timeout = {0, 500 * 1000};
	setsockopt(connectfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
	setsockopt(connectfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(port));
	server.sin_addr.s_addr = inet_addr(ip);

	if (connect(connectfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "connect() error");
		close(connectfd);
		return -1;
	}
	if (getsockname(connectfd, &localaddr, &addrlen) < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "getsockname() error");
		close(connectfd);
		return -1;
	}
	else
	{
		localaddr_ptr = (struct sockaddr_in *)&localaddr;
		BoatLog(BOAT_LOG_VERBOSE, "localIP: %s:%d.",
				inet_ntoa(localaddr_ptr->sin_addr), htons(localaddr_ptr->sin_port));
	}

	BoatLog(BOAT_LOG_VERBOSE, "%s:%s[%d] connected!", ip, port, connectfd);

	return connectfd;
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

    int ret = 0;
    void *sslCtx = NULL;

    int verify_mode = 1;
    int timeOut = 60;

    BoatLog(BOAT_LOG_NORMAL, "CA Cert len = %d",caChain.field_len);
    BoatLog(BOAT_LOG_NORMAL, "CA Cert=>%s",caChain.field_ptr+(caChain.field_len - 20));

    
    // BoatSleepMs(200);

    ret = BoatConnect(address, NULL);
    if (ret < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "socket connect fail ");
        return BOAT_ERROR;
    }
    *socketfd = ret;


    BoatSleepMs(200);

    if(BOAT_TLS_IDENTIFY_CLIENT == 1)
    {
        verify_mode = 2;
    }

    cm_ssl_setopt(SSL_ID,CM_SSL_PARAM_CA_CERT_DELETE,NULL);
    cm_ssl_setopt(SSL_ID,CM_SSL_PARAM_CLI_CERT_DELETE,NULL);
    cm_ssl_setopt(SSL_ID,CM_SSL_PARAM_CLI_KEY_DELETE,NULL);

    cm_ssl_setopt(SSL_ID,CM_SSL_PARAM_VERIFY,&verify_mode);
    cm_ssl_setopt(SSL_ID,CM_SSL_PARAM_NEGOTIME,&timeOut);
    cm_ssl_setopt(SSL_ID,CM_SSL_PARAM_CA_CERT,caChain.field_ptr);

    if (BOAT_TLS_IDENTIFY_CLIENT == 1)
    {
        cm_ssl_setopt(SSL_ID,CM_SSL_PARAM_CLI_CERT,clientCert.field_ptr);
        cm_ssl_setopt(SSL_ID,CM_SSL_PARAM_CLI_KEY,clientPrikey.field_ptr);
    }

    ret = cm_ssl_conn(&sslCtx,SSL_ID,*socketfd,5000);
    if(ret != 0)
    {
        BoatLog(BOAT_LOG_NORMAL, " BoatTlsInit Failed,ret = %d",ret);
        return BOAT_ERROR;
    }
    *tlsContext = sslCtx;
    // fibo_taskSleep(10000);
    BoatLog(BOAT_LOG_NORMAL, " BoatTlsInit OK");
    return BOAT_SUCCESS;
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

    int ret = cm_ssl_write(tlsContext,buf,len);
    BoatLog(BOAT_LOG_VERBOSE, "write ssl send = %d ", ret);
    return ret;
#else
    return cm_asocket_send(sockfd, buf, len, 0);
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

    int ret = cm_ssl_read(tlsContext,buf,len);
    BoatLog(BOAT_LOG_VERBOSE, "write ssl recv = %d ", ret);
    return ret;
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


    cm_ssl_close(tlsContext);
    close(sockfd);
#else
    close(sockfd);
#endif
}
// #endif /* #if (PROTOCOL_USE_HLFABRIC == 1) */