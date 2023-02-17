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

#if (PROTOCOL_USE_HLFABRIC == 1 || PROTOCOL_USE_CHAINMAKER_V1 == 1 || PROTOCOL_USE_CHAINMAKER_V2 == 1 || BOAT_PROTOCOL_USE_HWBCS == 1)
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
BSINT32 BoatConnect(const BCHAR *address, void *rsvd)
{
    int connectfd;
    char ip[64];
    char port[8];
    char *ptr = NULL;
    struct hostent *he;
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

#if (BOAT_TLS_SUPPORT == 1)
    connectfd = fibo_ssl_sock_create();
    if (connectfd == -1)
    {
        BoatLog(BOAT_LOG_CRITICAL, "socket() error");
        return -1;
    }
    if (fibo_ssl_sock_connect(connectfd, ip, atoi(port)) < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "connect() error ,code = %d", fibo_get_ssl_errcode());
        fibo_ssl_sock_close(connectfd);
        return -1;
    }
    /// get ssl socket fd
    int fd = fibo_ssl_sock_get_fd(connectfd);

    // set sock fd to unblock
    fibo_sock_lwip_fcntl(fd, F_SETFL, fibo_sock_lwip_fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
    BoatLog(BOAT_LOG_VERBOSE, "%s:%s[%d] connected!", ip, port, connectfd);
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

    if ((connectfd = fibo_sock_create(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "socket() error");
        return -1;
    }

    if (fibo_sock_connect(connectfd, &addr) == 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "connect() error");
        fibo_ssl_sock_close(connectfd);
        return -1;
    }
#endif

    return connectfd;
}

#if (BOAT_TLS_SUPPORT == 1)
BOAT_RESULT BoatTlsInit(const BCHAR *address, const BCHAR *hostName, const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey,
                        const BoatFieldVariable clientCert, BSINT32 *socketfd, void **tlsContext, void *rsvd)
{

    int ret = 0;

    fibo_set_ssl_chkmode(1);
    ret = fibo_write_ssl_file("TRUSTFILE", caChain.field_ptr, strlen(caChain.field_ptr));
    if (BOAT_TLS_IDENTIFY_CLIENT == 1)
    {
        ret = fibo_write_ssl_file("CAFILE", clientCert.field_ptr, clientCert.field_len);
        ret = fibo_write_ssl_file("CAKEY", clientPrikey.field_ptr, clientPrikey.field_len);
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

BSINT32 BoatSend(BSINT32 sockfd, void *tlsContext, const void *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1)
    //! @todo BOAT_TLS_SUPPORT implementation in crypto default.
    //! @todo BOAT_HLFABRIC_TLS_SUPPORT implementation in crypto default.
    BSINT32 ret = fibo_ssl_sock_send(sockfd, buf, len);
    // BoatLog(BOAT_LOG_VERBOSE, "write ssl send = %d ", ret);
    return ret;
#else
    return send(sockfd, buf, len, 0);
#endif
}

static int ssl_recv_unblock(INT32 sock, void *buf, INT32 size, INT32 timeout)
{
    struct timeval tm = {0};
    fd_set rset;
    BUINT8 *temp = buf;

    int fd = fibo_ssl_sock_get_fd(sock);

    int ret = fibo_ssl_sock_recv(sock, buf, size);
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
    FD_SET(fd, &rset);
    tm.tv_sec = timeout / 1000;
    tm.tv_usec = (timeout % 1000) * 1000;

    ret = select(fd + 1, &rset, NULL, NULL, timeout > 0 ? &tm : NULL);
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
        ret = fibo_ssl_sock_recv(sock, buf, size);
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
}

BSINT32 BoatRecv(BSINT32 sockfd, void *tlsContext, void *buf, size_t len, void *rsvd)
{
#if (BOAT_TLS_SUPPORT == 1)
    //! @todo BOAT_TLS_SUPPORT implementation in crypto default.
    BSINT32 ret = ssl_recv_unblock(sockfd, buf, len, 10 * 1000);
    return ret;
#else
    return recv(sockfd, buf, len, 0);
#endif
}

void BoatClose(BSINT32 sockfd, void **tlsContext, void *rsvd)
{
    close(sockfd);
#if (BOAT_TLS_SUPPORT == 1)
    // free tls releated
    //! @todo BOAT_TLS_SUPPORT implementation in crypto default.
    fibo_ssl_sock_close(sockfd);
#else
    close(sockfd);
#endif
}
#endif /* #if (PROTOCOL_USE_HLFABRIC == 1) */