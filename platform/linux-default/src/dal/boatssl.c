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

/* net releated include */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
// #include "http2intf.h"

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
		// LCOV_EXCL_START 
		BoatLog(BOAT_LOG_NORMAL, "read cert data fail");
		return NULL;
		// LCOV_EXCL_STOP
		
	}
	X509 *x509 = PEM_read_bio_X509(b, NULL, NULL, NULL);
	if (NULL == x509)
	{
		BoatLog(BOAT_LOG_NORMAL, "PEM_read_bio_X509 fail"); // LCOV_EXCL_START
		BIO_free(b);
		return NULL;
		// LCOV_EXCL_STOP
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
		BIO_free(b); // LCOV_EXCL_START
		return NULL; 
		// LCOV_EXCL_STOP
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
		BoatLog(BOAT_LOG_CRITICAL, "invalid address:%s.", address); // LCOV_EXCL_START
		return -1; 
		// LCOV_EXCL_STOP
	}

	memset(ip, 0, sizeof(ip));
	memset(port, 0, sizeof(port));
	memcpy(ip, address, (int)(ptr - address));
	memcpy(port, ptr + 1, strlen(address) - (int)(ptr - address));

	if ((he = gethostbyname(ip)) == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "gethostbyname() error"); // LCOV_EXCL_START
		return -1; 
		// LCOV_EXCL_STOP
	}

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
	server.sin_addr = *((struct in_addr *)(he->h_addr_list[0]));

	if (connect(connectfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "connect() error");
		close(connectfd);
		return -1;
	}
	if (getsockname(connectfd, &localaddr, &addrlen) < 0)
	{
		BoatLog(BOAT_LOG_CRITICAL, "getsockname() error"); // LCOV_EXCL_START
		close(connectfd);
		return -1; 
		// LCOV_EXCL_STOP
	}
	else
	{
		localaddr_ptr = (struct sockaddr_in *)&localaddr;
		BoatLog(BOAT_LOG_VERBOSE, "localIP: %s:%d.",
				inet_ntoa(localaddr_ptr->sin_addr), htons(localaddr_ptr->sin_port));
	}

	BoatLog(BOAT_LOG_VERBOSE, "%s:%s[%d] connected!", ip, port, connectfd);
	(void)localaddr_ptr;
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
	SSL_CTX *ctx;
	X509 *cert = NULL;
	X509 *clientTlsCert = NULL;
	EVP_PKEY *clientTlsprikey = NULL;
	retval = BoatConnect(address, NULL);
	if (retval < BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_NORMAL, "socket connect fail ");
		return BOAT_ERROR;
	}
	*socketfd = retval;
	/* SSL library init, ssl-server.c  */
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	ctx = SSL_CTX_new(SSLv23_client_method());
	if (ctx == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "creat SSL CTX fail "); // LCOV_EXCL_START
		return BOAT_ERROR; 
		// LCOV_EXCL_STOP
	}
	// client and server verification
	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
	cert = buffer2x509(caChain.field_ptr, caChain.field_len);
	if (cert == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, " get x509 cert fail"); // LCOV_EXCL_START
		return BOAT_ERROR; 

		// LCOV_EXCL_STOP
	}
	X509_STORE *certS = SSL_CTX_get_cert_store(ctx);
	X509_STORE_add_cert(certS, cert);
	X509_free(cert);
	if (BOAT_TLS_IDENTIFY_CLIENT == 1)
	{
		/* load client tls cert ,contain public key */
		clientTlsCert = buffer2x509(clientCert.field_ptr, clientCert.field_len);
		if (clientTlsCert == NULL)
		{
			BoatLog(BOAT_LOG_NORMAL, "read client tls cert fail "); // LCOV_EXCL_START
			SSL_CTX_free(ctx);
			return BOAT_ERROR; 
			// LCOV_EXCL_STOP
		}
		if (SSL_CTX_use_certificate(ctx, clientTlsCert) <= 0)
		{
			BoatLog(BOAT_LOG_NORMAL, "load client tls cert fail "); // LCOV_EXCL_START
			SSL_CTX_free(ctx);
			X509_free(clientTlsCert);
			return BOAT_ERROR; 
			// LCOV_EXCL_STOP
		}
		X509_free(clientTlsCert);

		clientTlsprikey = buffer2evpkey(clientPrikey.field_ptr, clientPrikey.field_len);
		if (clientTlsprikey == NULL)
		{
			BoatLog(BOAT_LOG_NORMAL, "read client tls key fail "); // LCOV_EXCL_START
			SSL_CTX_free(ctx);
			return BOAT_ERROR; 
			// LCOV_EXCL_STOP
		}
		/* load client prikey */
		if (SSL_CTX_use_PrivateKey(ctx, clientTlsprikey) <= 0)
		{
			BoatLog(BOAT_LOG_NORMAL, "load client tls key fail "); // LCOV_EXCL_START
			EVP_PKEY_free(clientTlsprikey);
			SSL_CTX_free(ctx);
			return BOAT_ERROR; 
			// LCOV_EXCL_STOP
		}
		EVP_PKEY_free(clientTlsprikey);
		/* check client's private key */
		if (!SSL_CTX_check_private_key(ctx))
		{
			BoatLog(BOAT_LOG_NORMAL, "creat SSL CTX fail "); // LCOV_EXCL_START
			SSL_CTX_free(ctx);
			return BOAT_ERROR; 
			// LCOV_EXCL_STOP
		}
	}
	/* creat new SSL by ctx  */
	void **p = (void **)tlsContext;
	/////*tlsContext = SSL_new(ctx);
	/////SSL_set_fd(*tlsContext, *socketfd);
	*p = SSL_new(ctx);
	SSL_set_fd(*p, *socketfd);
	/* creat connection of SSL */
	/////if (SSL_connect(*tlsContext) == -1)
	if (SSL_connect(*p) == -1)
	{
		BoatLog(BOAT_LOG_NORMAL, "SSL_connect fail "); // LCOV_EXCL_START
		SSL_CTX_free(ctx);
		return BOAT_ERROR; 
		// LCOV_EXCL_STOP
	}
	SSL_CTX_free(ctx);
	return BOAT_SUCCESS;
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
	return SSL_write((SSL *)tlsContext, buf, len);
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
	return SSL_read((SSL *)tlsContext, buf, len);
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
	SSL_shutdown((SSL *)*tlsContext);
	SSL_free((SSL *)*tlsContext);
	*tlsContext = NULL;
#endif
}

