/**
****************************************************************************************
* @FilePath: http_client.c
* @Author: aitos
* @Date: 2022-12-28 14:37:58
* @LastEditors:
* @LastEditTime: 2022-12-28 14:37:59
* @Descripttion:
****************************************************************************************
*/
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

#include "http_client.h"

#include "boatlog.h"

BCHAR *httpclientResponseBuf = NULL;

#define HTTPCLIENT_AUTHB_SIZE 128

#define HTTPCLIENT_CHUNK_SIZE 512
#define HTTPCLIENT_SEND_BUF_SIZE 512

#define HTTPCLIENT_MAX_HOST_LEN 64
#define HTTPCLIENT_MAX_URL_LEN 256

#define HTTPCLIENT_MAX_SOC_TIMEOUT (10 * 60) // 10 min

int httpclient_parse_url(const char *url, char *scheme, size_t max_scheme_len, char *host, size_t maxhost_len, int *port, char *path, size_t max_path_len)
{
    char *scheme_ptr = (char *)url;
    char *host_ptr = (char *)strstr(url, "://");
    size_t host_len = 0;
    size_t path_len;
    char *port_ptr;
    char *path_ptr;
    char *fragment_ptr;

    if (host_ptr == NULL)
    {
        // WARN("Could not find host");
        // return HTTPCLIENT_ERROR_PARSE; /* URL is invalid */
        BoatLog(BOAT_LOG_NORMAL, "Could not find http char");
        memcpy(scheme, "http", 4);
        scheme[4] = '\0';
        host_ptr = scheme_ptr;
    }
    else
    {
        if (max_scheme_len < host_ptr - scheme_ptr + 1)
        { /* including NULL-terminating char */
            BoatLog(BOAT_LOG_NORMAL, "Scheme str is too small (%d >= %d)", max_scheme_len, host_ptr - scheme_ptr + 1);
            return HTTPCLIENT_ERROR_PARSE;
        }
        memcpy(scheme, scheme_ptr, host_ptr - scheme_ptr);
        scheme[host_ptr - scheme_ptr] = '\0';

        host_ptr += 3;
    }

    port_ptr = strchr(host_ptr, ':');
    if (port_ptr != NULL)
    {
        uint16_t tport;
        host_len = port_ptr - host_ptr;
        port_ptr++;
        if (sscanf(port_ptr, "%hu", &tport) != 1)
        {
            BoatLog(BOAT_LOG_NORMAL, "Could not find port");
            return HTTPCLIENT_ERROR_PARSE;
        }
        *port = (int)tport;
    }
    else
    {
        *port = 0;
    }
    path_ptr = strchr(host_ptr, '/');
    // if (path_ptr == NULL)
    //{
    //     return HTTPCLIENT_ERROR_PARSE;
    // }
    if (host_len == 0)
    {
        host_len = path_ptr - host_ptr;
    }

    if (maxhost_len < host_len + 1)
    { /* including NULL-terminating char */
        BoatLog(BOAT_LOG_NORMAL, "Host str is too small (%d >= %d)", maxhost_len, host_len + 1);
        return HTTPCLIENT_ERROR_PARSE;
    }
    memcpy(host, host_ptr, host_len);
    host[host_len] = '\0';

    fragment_ptr = strchr(host_ptr, '#');
    if (fragment_ptr != NULL)
    {
        path_len = fragment_ptr - path_ptr;
    }
    else
    {
        if (path_ptr != NULL)
        {
            path_len = strlen(path_ptr);
        }
        else
        {
            path_len = 0;
        }
    }

    if (max_path_len < path_len + 1)
    { /* including NULL-terminating char */
        BoatLog(BOAT_LOG_NORMAL, "Path str is too small (%d >= %d)", max_path_len, path_len + 1);
        return HTTPCLIENT_ERROR_PARSE;
    }

    if (path_ptr != NULL && path_len > 0)
    {
        memcpy(path, path_ptr, path_len);
    }
    path[path_len] = '\0';

    return HTTPCLIENT_OK;
}

int httpclient_get_header(char *url, int senddatalen, char *head, int headlen)
{
    char scheme[8] = {0};
    char host[HTTPCLIENT_MAX_HOST_LEN] = {0};
    char path[HTTPCLIENT_MAX_URL_LEN] = {0};
    int len, offset;
    char send_buf[HTTPCLIENT_SEND_BUF_SIZE] = {0};
    char buf[HTTPCLIENT_SEND_BUF_SIZE] = {0};
    char *meth = "POST";
    int ret, port;

    /* First we need to parse the url (http[s]://host[:port][/[path]]) */
    int res = httpclient_parse_url(url, scheme, sizeof(scheme), host, sizeof(host), &(port), path, sizeof(path));
    if (res != HTTPCLIENT_OK)
    {
        BoatLog(BOAT_LOG_NORMAL, "httpclient_parse_url returned %d", res);
        return res;
    }

    /* Send request */
    memset(send_buf, 0, HTTPCLIENT_SEND_BUF_SIZE);
    len = 0; /* Reset send buffer */

    if (strlen(path) == 0)
    {
        len = snprintf(buf, sizeof(buf), "%s /%s HTTP/1.1\r\nHost: %s\r\n", meth, path, host); /* Write request */
    }
    else
    {
        len = snprintf(buf, sizeof(buf), "%s %s HTTP/1.1\r\nHost: %s\r\n", meth, path, host); /* Write request */
    }
    offset = len;

    if (senddatalen > 0)
    {
        len = snprintf(buf + offset - 1, sizeof(buf) - offset + 1, "Content-Length: %d\r\n", senddatalen);
        offset += len;

        // if (client_data->post_content_type != NULL)
        {
            len = snprintf(buf + offset - 1, sizeof(buf) - offset + 1, "Content-Type: %s\r\n", "application/json");
            offset += len;
        }
    }

    /* Close headers */
    len = snprintf(buf + offset - 1, sizeof(buf) - offset + 1, "\r\n");
    offset += len;

    BoatLog(BOAT_LOG_NORMAL, "Trying to write %d bytes http header:%s", offset, buf);
    strncpy(head, buf, headlen);

    return HTTPCLIENT_OK;
}

oc_http_param_t *httpClientInit(void)
{
    oc_http_param_t *pstHttpParam = NULL;
    pstHttpParam = fibo_http_new();
    return pstHttpParam;
}

void httpClientDeinit(oc_http_param_t *pstHttpParam)
{
    fibo_http_delete(pstHttpParam);
    if (httpclientResponseBuf != NULL)
    {
        BoatFree(httpclientResponseBuf);
        httpclientResponseBuf = NULL;
    }
}

BOAT_RESULT httpClientSetUrl(oc_http_param_t *pstHttpParam, BCHAR *url)
{
    BOAT_RESULT ret = BOAT_SUCCESS;
    if (pstHttpParam == NULL || url == NULL)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    memset(pstHttpParam->url, 0, OC_HTTP_URL_LEN + 1);
    if (strlen(url) <= OC_HTTP_URL_LEN)
    {
        strcpy((char *)pstHttpParam->url, url);
    }
    else
    {
        return BOAT_ERROR;
    }
    return ret;
}

void cb_http_response_handle(void *pHttpParam, INT8 *data, INT32 len)
{
    fibo_taskSleep(50);
    fibo_http_response_status_line((oc_http_param_t *)pHttpParam);
    BoatLog(BOAT_LOG_NORMAL, "boat  HTTP response code          : %d", ((oc_http_param_t *)pHttpParam)->respCode);
    BoatLog(BOAT_LOG_NORMAL, "boat  HTTP response version       : %s", ((oc_http_param_t *)pHttpParam)->respVersion);
    BoatLog(BOAT_LOG_NORMAL, "boat  HTTP response reason phrase : %s", ((oc_http_param_t *)pHttpParam)->respReasonPhrase);

    BoatLog(BOAT_LOG_NORMAL, "boat  HTTP Recive Data : %d %s", len, data);
    // httpclientResponseBuf = BoatMalloc(len + 1);
    // memcpy(httpclientResponseBuf, data, len);
}

void cb_http_header_handle(const UINT8 *name, const UINT8 *value)
{
    BoatLog(BOAT_LOG_NORMAL, "boat  Header: %s : %s", name, value);
}

BOAT_RESULT httpClientRequestSync(oc_http_param_t *pstHttpParam,
                                  BUINT8 *request_ptr,
                                  BUINT32 request_len,
                                  BOAT_OUT BUINT8 **response_pptr,
                                  BOAT_OUT BUINT32 *response_len_ptr)
{
    BOAT_RESULT ret = BOAT_SUCCESS;
    BCHAR Post_Head[256] = {0};
    /* prepare http get param */
    pstHttpParam->timeout = 2 * 60;
    pstHttpParam->cbReadBody = cb_http_response_handle;
    pstHttpParam->respCode = 500;
    pstHttpParam->bIsIpv6 = false;
    pstHttpParam->enHttpReadMethod = OC_USER_CALLBACK;
    // const char *Post_Head = "Content-Type:application/json;charset=UTF-8\r\nAccept:application/json, text/javascript, */*;q=0.01\r\nAccept-Language:zh-CN,zh;q=0.8\r\n";
    // const char *Post_Head = "POST / HTTP/1.1\r\nAccept: application/json\r\nConnection: close\r\nContent-Type: application/json\r\n";
    // const char *Post_Head = "POST /v1 HTTP/1.1\r\nContent-Type:application/json;charset=UTF-8\r\n";
    // const char *Post_Head = "POST / HTTP/1.1\r\nAccept: application/json\r\nConnection: close\r\nContent-Type: application/json\r\n\r\n";
    // const char *Post_Head = "POST / HTTP/1.1\r\nAccept: application/json\r\nConnection: close\r\nContent-Type: application/json\r\nContent-Length: 29\r\nHost: 111.231.250.105\r\n\r\n";
    ret = httpclient_get_header(pstHttpParam->url, request_len, Post_Head, sizeof(Post_Head));
    BoatLog(BOAT_LOG_NORMAL, " httpclient_send_header ret = %d ", ret);
    // BCHAR Post_Head[256] = {0};
    // snprintf(
    //     Post_Head,
    //     sizeof(Post_Head),
    //     "POST / HTTP/1.1\r\nAccept: application/json\r\nConnection: close\r\nContent-Type: application/json\r\nContent-Length: %d\r\nHost: 101.35.163.23\r\n\r\n",
    //     request_len + 1);
    // const char *Post_Data = "{\"device_mac\":\"121212121212\"}";
    // BoatLog(BOAT_LOG_NORMAL, " boat httpclient url : %s ", pstHttpParam->url);
    // BoatLog(BOAT_LOG_NORMAL, " boat httpclient post : %s ", request_ptr);
    BoatLog(BOAT_LOG_NORMAL, " boat httpclient Post_Head : %s ", Post_Head);
    // fibo_http_head(pstHttpParam, Post_Head);
    ret = fibo_http_post(pstHttpParam, (UINT8 *)request_ptr, NULL);
    // ret = fibo_http_post_ex(pstHttpParam, request_ptr, NULL, request_len);
    BoatLog(BOAT_LOG_NORMAL, "boat HTTP response code          : %d", pstHttpParam->respCode);
    BoatLog(BOAT_LOG_NORMAL, "boat HTTP response version       : %s", pstHttpParam->respVersion);
    BoatLog(BOAT_LOG_NORMAL, "boat HTTP response reason phrase : %s", pstHttpParam->respReasonPhrase);
    BoatLog(BOAT_LOG_NORMAL, "boat HTTP response body len      : %d", pstHttpParam->respContentLen);
    BoatLog(BOAT_LOG_NORMAL, "boat HTTP response ret      : %d", ret);
    fibo_taskSleep(5 * 1000);
    // fibo_http_response_header_foreach(pstHttpParam, cb_http_header_handle);
    if ((ret == 0) || (pstHttpParam->respCode == 200 || pstHttpParam->respCode == 201))
    {
        // if (httpclientResponseBuf == NULL)
        // {
        //     return BOAT_ERROR;
        // }
        // *response_len_ptr = pstHttpParam->respContentLen;
        // *response_pptr = BoatMalloc(pstHttpParam->respContentLen + 1);
        // memcpy(*response_pptr, httpclientResponseBuf, pstHttpParam->respContentLen);
        // BoatFree(httpclientResponseBuf);
        // httpclientResponseBuf = NULL;
        return BOAT_SUCCESS;
    }
    else
    {
        return BOAT_ERROR;
    }
}