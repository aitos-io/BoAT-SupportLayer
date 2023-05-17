

#ifndef __HTTPCLIENT_H__
#define __HTTPCLIENT_H__


#include <stdint.h>
#include <stdbool.h>
//#include <types.h>

// struct timeval {
//     time_t      tv_sec;     /* seconds */
//     suseconds_t tv_usec;    /* and microseconds */
// };

/** @brief   This enumeration defines the API return type.  */
typedef enum {
    HTTPCLIENT_ERROR_PARSE = -6,           /**< A URL parse error occurred. */
    HTTPCLIENT_UNRESOLVED_DNS = -5,        /**< Could not resolve the hostname. */
    HTTPCLIENT_ERROR_PRTCL = -4,           /**< A protocol error occurred. */
    HTTPCLIENT_ERROR = -3,                 /**< An unknown error occurred. */
    HTTPCLIENT_CLOSED = -2,                /**< Connection was closed by a remote host. */
    HTTPCLIENT_ERROR_CONN = -1,            /**< Connection failed. */
    HTTPCLIENT_OK = 0,                     /**< The operation was successful. */
    HTTPCLIENT_RETRIEVE_MORE_DATA = 1      /**< More data needs to be retrieved. */
} HTTPCLIENT_RESULT;

/** @defgroup httpclient_define Define
  * @{
  */
/** @brief   This macro defines the deault HTTP port.  */
#define HTTP_PORT   80

/** @brief   This macro defines the deault HTTPS port.  */
#define HTTPS_PORT 443

/**
 * @}
 */

/** @defgroup httpclient_enum Enum
 *  @{
 */
/** @brief   This enumeration defines the HTTP request type.  */
typedef enum {
    HTTPCLIENT_GET,
    HTTPCLIENT_POST,
    HTTPCLIENT_PUT,
    HTTPCLIENT_DELETE,
    HTTPCLIENT_HEAD
} HTTPCLIENT_REQUEST_TYPE;


/** @defgroup httpclient_struct Struct
  * @{
  */
/** @brief   This structure defines the httpclient_t structure.  */
typedef struct {
    int socket_id;                     /**< Socket ID. */
    int remote_port;                /**< HTTP or HTTPS port. */
    int response_code;              /**< Response code. */
    char *header;                   /**< Request custom header. */
    char *auth_user;                /**< Username for basic authentication. */
    char *auth_password;            /**< Password for basic authentication. */
    bool is_http;                   /**< Http connection? if 1, http; if 0, https. */
    int method;                     /**< HTTP method, 0 ~ 4.  */
    unsigned int timeout_in_sec;    /**< HTTP socket recv timeout. */
} httpclient_t;

/** @brief   This structure defines the HTTP data structure.  */
typedef struct {
    bool is_more;                /**< Indicates if more data needs to be retrieved. */
    bool is_chunked;             /**< Response data is encoded in portions/chunks.*/
    int retrieve_len;            /**< Content length to be retrieved. */
    int response_content_len;    /**< Response content length. */
    int content_block_len;       /**< The content length of one block. */
    int post_buf_len;            /**< Post data length. */
    int response_buf_len;        /**< Response body buffer length. */
    int header_buf_len;          /**< Response head buffer lehgth. */
    char *post_content_type;     /**< Content type of the post data. */
    char *post_buf;              /**< User data to be posted. */
    char *response_buf;          /**< Buffer to store the response body data. */
    char *header_buf;            /**< Buffer to store the response head data. */
    char *url;
    int method;
} httpclient_data_t;


HTTPCLIENT_RESULT httpclient_post(httpclient_t *client, char *url, httpclient_data_t *client_data);

int httpclient_get_response_code(httpclient_t *client);

#endif