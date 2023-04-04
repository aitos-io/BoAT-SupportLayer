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
    int ret;
    int sockFd;
    char ip[64];
    char port[8];
    char *ptr = NULL;
    // struct hostent *he;
    // struct sockaddr_in server;
    // struct sockaddr localaddr;
    // struct sockaddr_in *localaddr_ptr;
    // socklen_t addrlen = sizeof(struct sockaddr);

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

#if (BOAT_TLS_SUPPORT == 1)
   // connectfd = fibo_ssl_sock_create();
    void *sslCtx = NULL;
    sockFd = cm_asocket_open(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL);
   
    ret = cm_ssl_conn(&sslCtx,0,sockFd,5000);
    if (ret != 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "ssl connect() error ,code = %d", ret);
        return -1;
    }
    /// get ssl socket fd
   // int fd = fibo_ssl_sock_get_fd(connectfd);

    // set sock fd to unblock
    lwip_fcntl(sockFd, F_SETFL, lwip_fcntl(sockFd, F_GETFL, 0) | O_NONBLOCK);
    BoatLog(BOAT_LOG_VERBOSE, "%s:%s[%d] connected!", ip, port, sockFd);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(atoi(port));

    

    ret = cm_asocket_connect(sockFd,(const struct sockaddr *)&server_addr,sizeof(server_addr));
    if(ret != 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "asocket connect() error ,code = %d", ret);
        return -1;
    }


#else
    GAPP_TCPIP_ADDR_T addr;
    if (getHostByIpOrName(address, &addr) != 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "getaddrinfo != 0, return HTTPCLIENT_UNRESOLVED_DNS");
        return HTTPCLIENT_UNRESOLVED_DNS;
    }

    addr.sin_port = boat_htons(/*7545*/ client->remote_port);
    if (client->socket < 0)
    {
        return HTTPCLIENT_ERROR_CONN;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("120.27.12.119");
    server_addr.sin_port = htons(2004);

    if ((sockFd = cm_asocket_open(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL)) < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "socket() error");
        return -1;
    }

    if (fibo_sock_connect(sockFd, &addr) == 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "connect() error");
        fibo_ssl_sock_close(sockFd);
        return -1;
    }
#endif


    return sockFd;
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
    
    int verify_mode = 2;
    cm_ssl_setopt(0,CM_SSL_PARAM_VERIFY,&verify_mode);
    cm_ssl_setopt(0,CM_SSL_PARAM_CA_CERT,caChain.field_ptr);
    if (BOAT_TLS_IDENTIFY_CLIENT == 1)
    {
        cm_ssl_setopt(0,CM_SSL_PARAM_CLI_CERT,clientCert.field_ptr);
        cm_ssl_setopt(0,CM_SSL_PARAM_CLI_KEY,clientPrikey.field_ptr);
    }
    

    ret = BoatConnect(address, NULL);
    if (ret == -1)
    {
        BoatLog(BOAT_LOG_NORMAL, "socket connect fail ");
        return BOAT_ERROR;
    }
    *socketfd = ret;
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

    int ret = cm_asocket_send(sockfd, buf, len,0);
    // BoatLog(BOAT_LOG_VERBOSE, "write ssl send = %d ", ret);
    return ret;
#else
    return cm_asocket_send(sockfd, buf, len, 0);
#endif
}

static int ssl_recv_unblock(INT32 sock, void *buf, INT32 size, INT32 timeout)
{
    struct timeval tm = {0};
    fd_set rset;
    // BUINT8 *temp = buf;

    //int fd = fibo_ssl_sock_get_fd(sock);

    //int ret = fibo_ssl_sock_recv(sock, buf, size);
    int ret = cm_asocket_recv(sock, buf, size, 0);
    if (ret > 0)
    {
        // BoatLog(BOAT_LOG_VERBOSE, "recv data size:%d", ret);
        return ret;
    }
    else if (ret < 0)
    {
        // BoatLog(BOAT_LOG_VERBOSE, "recv data fail");
        return ret;
    }

    FD_ZERO(&rset);
    FD_SET(sock, &rset);
    tm.tv_sec = timeout / 1000;
    tm.tv_usec = (timeout % 1000) * 1000;

    ret = select(sock + 1, &rset, NULL, NULL, timeout > 0 ? &tm : NULL);
    if (ret < 0)
    {
        BoatLog(BOAT_LOG_VERBOSE, "select failed:%s", strerror(errno));
        return -1;
    }
    else if (ret == 0)
    {
        BoatLog(BOAT_LOG_VERBOSE, "select timeout");
        return -1;
    }
    else
    {
        ret = cm_asocket_recv(sock, buf, size, 0);
        if (ret >= 0)
        {
            // BoatLog(BOAT_LOG_VERBOSE, "recv data size:%d", ret);
            return ret;
        }
        else if (ret < 0)
        {
            BoatLog(BOAT_LOG_VERBOSE, "recv data fail");
            return ret;
        }
    }
    return -1;
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
    BSINT32 ret = recv(sockfd, buf, len, 0);
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
    // free tls releated
    //! @todo BOAT_TLS_SUPPORT implementation in crypto default.
    close(sockfd);
#else
    close(sockfd);
#endif
}
// #endif /* #if (PROTOCOL_USE_HLFABRIC == 1) */