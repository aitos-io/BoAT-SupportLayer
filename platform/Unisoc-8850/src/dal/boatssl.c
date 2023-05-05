//! self header include

#include "boattypes.h"
#include "boatlog.h"

#include "rand.h"
#include "aes/aes.h"
#include "sha3.h"
#include "secp256k1.h"
#include "nist256p1.h"
#include "bignum.h"
#include "persiststore.h"
#include "boatkeystore.h"
#include "boatdal.h"
#include "boatplatformosal.h"

/* net releated include */
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h>
// #include <arpa/inet.h>
// #include <string.h>
// // #include <sys/time.h>
//  #include "http2intf.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

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

#if 0
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/ssl.h>

#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/x509v3.h>

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
	x509Cert = buffer2x509((const uint8_t *)cert, certlen);
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
    int                connectfd = -1;
    char               ip[64];
    char               port[8];
    char               *ptr = NULL;
    struct hostent     *he; 
    struct sockaddr_in server;
    struct sockaddr    localaddr;
    struct sockaddr_in *localaddr_ptr;
    socklen_t          addrlen = sizeof(struct sockaddr);

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

    if ((he = gethostbyname(ip)) == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "gethostbyname() error");
        return -1;
    }

    if ((connectfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "socket() error");
        return -1;
    }

    struct timeval timeout = {0, 500*1000};
    setsockopt(connectfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
    setsockopt(connectfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(port));
    server.sin_addr = *((struct in_addr *)(he->h_addr_list[0])); 

    if (connect(connectfd, (struct sockaddr *)&server,sizeof(struct sockaddr)) < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "connect() error");
        close(connectfd);
        return -1;
    }
    if (0 != getsockname(connectfd, &localaddr, &addrlen))
    {
        BoatLog(BOAT_LOG_CRITICAL, "getsockname() error");
        close(connectfd);
        return -1;
    }
    else
    {
        localaddr_ptr = (struct sockaddr_in*)&localaddr;
        BoatLog(BOAT_LOG_VERBOSE, "localIP: %s:%d.", 
        inet_ntoa(localaddr_ptr->sin_addr), htons(localaddr_ptr->sin_port));
    }

    BoatLog(BOAT_LOG_VERBOSE, "%s:%s[%d] connected!", ip, port, connectfd);

    return connectfd;
}

#if (BOAT_TLS_SUPPORT == 1)
/////BOAT_RESULT BoatTlsInit(const BCHAR *address, const BCHAR *hostName, const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey,
/////						const BoatFieldVariable clientCert, BSINT32 *socketfd, boatSSlCtx **tlsContext, void *rsvd);
BOAT_RESULT BoatTlsInit(const BCHAR *address, const BCHAR *hostName, const BoatFieldVariable caChain, const BoatFieldVariable clientPrikey, 
						const BoatFieldVariable clientCert, BSINT32 *socketfd, boatSSlCtx **tlsContext, void *rsvd)

{

	//! @todo BoatTlsInit implementation in crypto default.
	// return BOAT_ERROR;
	BOAT_RESULT retval = BOAT_SUCCESS;

	boatSSlCtx *platform_ssl_ctx = *tlsContext;

	mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

	mbedtls_net_context *ssl_net = NULL;
	mbedtls_ssl_context *ssl_ctx = NULL;
	mbedtls_ssl_config *ssl_cfg = NULL;
	mbedtls_x509_crt *ssl_crt = NULL;


	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;

	platform_ssl_ctx->ssl_net = BoatMalloc(sizeof(mbedtls_net_context));
	if(platform_ssl_ctx->ssl_net == NULL)
	{
		result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		BoatLog(BOAT_LOG_NORMAL, "Malloc ssl_net failed. ");
		boat_throw(result, BoatTlsInit_exception);
	}


	platform_ssl_ctx->ssl_ctx = BoatMalloc(sizeof(mbedtls_ssl_context));
	if(platform_ssl_ctx->ssl_ctx == NULL)
	{
		result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		BoatLog(BOAT_LOG_NORMAL, "Malloc ssl_ctx failed. ");
		boat_throw(result, BoatTlsInit_exception);
	}

	platform_ssl_ctx->ssl_cfg = BoatMalloc(sizeof(mbedtls_ssl_config));
	if(platform_ssl_ctx->ssl_cfg == NULL)
	{
		result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		BoatLog(BOAT_LOG_NORMAL, "Malloc ssl_cfg failed. ");
		boat_throw(result, BoatTlsInit_exception);
	}
	
	platform_ssl_ctx->ssl_crt = BoatMalloc(sizeof(mbedtls_ssl_config));
	if(platform_ssl_ctx->ssl_crt == NULL)
	{
		result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
		BoatLog(BOAT_LOG_NORMAL, "Malloc ssl_crt failed. ");
		boat_throw(result, BoatTlsInit_exception);
	}


	retval = BoatConnect(address, NULL);
	if (retval < BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_NORMAL, "socket connect fail ");
		boat_throw(result, BoatTlsInit_exception);
	}
	*socketfd = retval;


	mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

	mbedtls_net_init(platform_ssl_ctx->ssl_net);
	mbedtls_ssl_init(platform_ssl_ctx->ssl_ctx);
	mbedtls_ssl_config_init(platform_ssl_ctx->ssl_cfg);
	mbedtls_x509_crt_init(platform_ssl_ctx->ssl_crt);

	result = mbedtls_ctr_drbg_seed(&ctr_drbg,mbedtls_entropy_func,&entropy,NULL,0);
	if(result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Failed to execute ctr_drbg_seed.");
        boat_throw(result, BoatTlsInit_exception);
	}

	result = mbedtls_ssl_config_defaults(platform_ssl_ctx->ssl_cfg, MBEDTLS_SSL_IS_CLIENT,
										 MBEDTLS_SSL_TRANSPORT_STREAM,
										 MBEDTLS_SSL_PRESET_DEFAULT);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute ssl_config_defaults.\n");
        boat_throw(result, BoatTlsInit_exception);
    }

	mbedtls_ssl_conf_rng(platform_ssl_ctx->ssl_cfg, mbedtls_ctr_drbg_random, &ctr_drbg);

	result += mbedtls_x509_crt_parse(platform_ssl_ctx->ssl_crt, caChain.field_ptr, caChain.field_len);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute x509_crt_parse: -%x\n", -result);
        boat_throw(result, BoatTlsInit_exception);
    }

	mbedtls_ssl_conf_ca_chain(platform_ssl_ctx->ssl_cfg, platform_ssl_ctx->ssl_crt, NULL);
	mbedtls_ssl_conf_authmode(platform_ssl_ctx->ssl_cfg, MBEDTLS_SSL_VERIFY_REQUIRED);

	result = mbedtls_ssl_setup(platform_ssl_ctx->ssl_ctx, platform_ssl_ctx->ssl_cfg);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute ssl_setup.\n");
        boat_throw(result, BoatTlsInit_exception);
    }

	result = mbedtls_ssl_set_hostname(platform_ssl_ctx->ssl_ctx, hostName);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute hostname_set.\n");
        boat_throw(result, BoatTlsInit_exception);
    }

	ssl_net->fd = *socketfd;
	mbedtls_ssl_set_bio(platform_ssl_ctx->ssl_ctx, platform_ssl_ctx->ssl_net, mbedtls_net_send, mbedtls_net_recv, NULL);
	result = mbedtls_ssl_handshake(platform_ssl_ctx->ssl_ctx);
	if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Failed to execute ssl_handshake: -%x\n", -result);
        boat_throw(result, BoatTlsInit_exception);
    }
	BoatLog(BOAT_LOG_NORMAL, "ret = ssl_handshake SUCCESSED!");

	// (mbedtls_net_context *)(platform_ssl_ctx->ssl_net) = ssl_net;
	// (mbedtls_ssl_context *)(platform_ssl_ctx->ssl_ctx) = ssl_ctx;
	// (mbedtls_ssl_config *)(platform_ssl_ctx->ssl_cfg) = ssl_cfg;
	// (mbedtls_x509_crt *)(platform_ssl_ctx->ssl_crt) = ssl_crt;

	/* boat catch handle */
	boat_catch(BoatTlsInit_exception)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;

		
		if(platform_ssl_ctx->ssl_net != NULL)
		{
			mbedtls_net_free(platform_ssl_ctx->ssl_net);
			BoatFree(platform_ssl_ctx->ssl_net);
			platform_ssl_ctx->ssl_net = NULL;
		}

		if(ssl_ctx != NULL)
		{
			mbedtls_ssl_free(platform_ssl_ctx->ssl_ctx);
			BoatFree(platform_ssl_ctx->ssl_ctx);
			platform_ssl_ctx->ssl_ctx = NULL;
		}

		if(ssl_cfg != NULL)
		{
			mbedtls_ssl_config_free(platform_ssl_ctx->ssl_cfg);
			BoatFree(platform_ssl_ctx->ssl_cfg);
			platform_ssl_ctx->ssl_cfg = NULL;
		}

		if(ssl_crt != NULL)
		{
			mbedtls_x509_crt_free(platform_ssl_ctx->ssl_crt);
			BoatFree(platform_ssl_ctx->ssl_crt);
			platform_ssl_ctx->ssl_crt = NULL;
		}
		
	}
	
	mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);

	return result;

}
#endif

/////BSINT32 BoatSend(BSINT32 sockfd, void *tlsContext, const void *buf, size_t len, void *rsvd)
BSINT32 BoatSend(BSINT32 sockfd, boatSSlCtx *tlsContext, const BUINT8 *buf, size_t len, void *rsvd)

{
#if (BOAT_TLS_SUPPORT == 1)
	//! @todo BOAT_TLS_SUPPORT implementation in crypto default.
	if (tlsContext == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "tlsContext  can't be NULL.");
		return -1;
	}
	//return SSL_write((SSL *)tlsContext, buf, len);
	return mbedtls_ssl_write((mbedtls_ssl_context *)(tlsContext->ssl_ctx),buf,len);
#else
	return send(sockfd, buf, len, 0);
#endif
}

/////BSINT32 BoatRecv(BSINT32 sockfd, void *tlsContext, void *buf, size_t len, void *rsvd)
BSINT32 BoatRecv(BSINT32 sockfd, boatSSlCtx *tlsContext, BUINT8 *buf, size_t len, void *rsvd)

{
#if (BOAT_TLS_SUPPORT == 1)
	//! @todo BOAT_TLS_SUPPORT implementation in crypto default.
	if (tlsContext == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "tlsContext  can't be NULL.");
		return -1;
	}
	//return SSL_read((SSL *)tlsContext, buf, len);
	return mbedtls_ssl_read((mbedtls_ssl_context *)(tlsContext->ssl_ctx),buf,len);
#else
	return recv(sockfd, buf, len, 0);
#endif
}

/////void BoatClose(BSINT32 sockfd, void **tlsContext, void *rsvd)
void BoatClose(BSINT32 sockfd, boatSSlCtx **tlsContext, void *rsvd)
{
	close(sockfd);
#if (BOAT_TLS_SUPPORT == 1)
	// free tls releated
	//! @todo BOAT_TLS_SUPPORT implementation in crypto default.
	// SSL_shutdown((SSL *)*tlsContext);
	// SSL_free((SSL *)*tlsContext);
	// *tlsContext = NULL;

	if(*tlsContext != NULL)
	{
		mbedtls_net_free((mbedtls_net_context *)((*tlsContext)->ssl_net));
		BoatFree((*tlsContext)->ssl_net);
		(*tlsContext)->ssl_net = NULL;

		mbedtls_ssl_free((mbedtls_ssl_context *)((*tlsContext)->ssl_ctx));
		BoatFree((*tlsContext)->ssl_ctx);
		(*tlsContext)->ssl_ctx = NULL;

		mbedtls_ssl_config_free((mbedtls_ssl_config *)((*tlsContext)->ssl_cfg));
		BoatFree((*tlsContext)->ssl_cfg);
		(*tlsContext)->ssl_cfg = NULL;

		mbedtls_x509_crt_free((mbedtls_x509_crt *)((*tlsContext)->ssl_crt));
		BoatFree((*tlsContext)->ssl_crt);
		(*tlsContext)->ssl_crt = NULL;
	}

#endif
}

