/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __HTTPCLIENT_H__
#define __HTTPCLIENT_H__

#include <stdint.h>
#include <stdbool.h>
#include "fibo_opencpu.h"
#include "boattypes.h"
#include "boaterrcode.h"

/** @brief   This enumeration defines the API return type.  */
typedef enum
{
    HTTPCLIENT_ERROR_PARSE = -6,      /**< A URL parse error occurred. */
    HTTPCLIENT_UNRESOLVED_DNS = -5,   /**< Could not resolve the hostname. */
    HTTPCLIENT_ERROR_PRTCL = -4,      /**< A protocol error occurred. */
    HTTPCLIENT_ERROR = -3,            /**< An unknown error occurred. */
    HTTPCLIENT_CLOSED = -2,           /**< Connection was closed by a remote host. */
    HTTPCLIENT_ERROR_CONN = -1,       /**< Connection failed. */
    HTTPCLIENT_OK = 0,                /**< The operation was successful. */
    HTTPCLIENT_RETRIEVE_MORE_DATA = 1 /**< More data needs to be retrieved. */
} HTTPCLIENT_RESULT;

oc_http_param_t *httpClientInit(void);
void httpClientDeinit(oc_http_param_t *pstHttpParam);
BOAT_RESULT httpClientSetUrl(oc_http_param_t *pstHttpParam, BCHAR *url);
BOAT_RESULT httpClientRequestSync(oc_http_param_t *pstHttpParam,
                                  BUINT8 *request_ptr,
                                  BUINT32 request_len,
                                  BOAT_OUT BUINT8 **response_pptr,
                                  BOAT_OUT BUINT32 *response_len_ptr);

#endif /* __HTTPCLIENT_H__ */
