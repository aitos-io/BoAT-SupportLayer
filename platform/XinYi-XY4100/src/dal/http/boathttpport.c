/**
****************************************************************************************
* @FilePath: boathttpport.c
* @Author: aitos
* @Date: 2022-12-28 18:06:53
* @LastEditors:
* @LastEditTime: 2022-12-28 18:06:54
* @Descripttion:
****************************************************************************************
*/
/******************************************************************************
Copyright (C) 2018-2020 aitos.io

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

/*!@brief boat HTTP porting for RPC

@file
boathttpport.c is the AT command porting of RPC.

DO NOT call functions in this file directly. Instead call wrapper functions
provided by rpcport.

To use boat HTTP RPC porting, RPC_USE_BOATHTTPPORT in boatoptions.h must set to 1.
*/

#include "boatinternal.h"

// #if RPC_USE_BOATHTTPPORT == 1

// #include "rpcport.h"
#include "boathttpport.h"

#include "httpclient.h"
#include "boatlog.h"
#include "boatplatformosal.h"

/*!*****************************************************************************
@brief Initialize boat HTTP RPC context.

Function: BoatHttpPortInit()

    This function initializes boat HTTP RPC context. It also dynamically allocates
    storage to receive response from the peer.

@see BoatHttpPortDeinit()

@return
    This function returns a pointer to boat's HTTP RPC context.\n
    It returns NULL if initialization fails.


@param This function doesn't take any argument.

*******************************************************************************/
BoatHttpPortContext *BoatHttpPortInit(void)
{
    BoatHttpPortContext *boathttpport_context_ptr;
    BOAT_RESULT result = BOAT_SUCCESS;

    boathttpport_context_ptr = BoatMalloc(sizeof(BoatHttpPortContext));

    if (boathttpport_context_ptr != NULL)
    {
        result = BOAT_SUCCESS;
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate boat HTTP RPC Context.");
        result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }

    if (result == BOAT_SUCCESS)
    {
        boathttpport_context_ptr->http_response_head.string_ptr = BoatMalloc(BOATHTTPPORT_RECV_BUF_SIZE);

        if (boathttpport_context_ptr->http_response_head.string_ptr != NULL)
        {
            boathttpport_context_ptr->http_response_head.string_space = BOATHTTPPORT_RECV_BUF_SIZE;
            boathttpport_context_ptr->http_response_head.string_len = 0;
        }
        else
        {
            BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate boat HTTP RESPONSE head buffer.");
            result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
    }

    if (result == BOAT_SUCCESS)
    {
        boathttpport_context_ptr->http_response_body.string_ptr = BoatMalloc(BOATHTTPPORT_RECV_BUF_SIZE);

        if (boathttpport_context_ptr->http_response_body.string_ptr != NULL)
        {
            boathttpport_context_ptr->http_response_body.string_space = BOATHTTPPORT_RECV_BUF_SIZE;
            boathttpport_context_ptr->http_response_body.string_len = 0;
        }
        else
        {
            BoatLog(BOAT_LOG_CRITICAL, "Fail to allocate boat HTTP RESPONSE body buffer.");
            result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
    }

    if (result != BOAT_SUCCESS)
    {
        if (boathttpport_context_ptr != NULL)
        {
            if (boathttpport_context_ptr->http_response_head.string_ptr != NULL)
            {
                BoatFree(boathttpport_context_ptr->http_response_head.string_ptr);
                boathttpport_context_ptr->http_response_head.string_ptr = NULL;
                boathttpport_context_ptr->http_response_head.string_space = 0;
                boathttpport_context_ptr->http_response_head.string_len = 0;
            }

            if (boathttpport_context_ptr->http_response_body.string_ptr != NULL)
            {
                BoatFree(boathttpport_context_ptr->http_response_body.string_ptr);
                boathttpport_context_ptr->http_response_body.string_ptr = NULL;
                boathttpport_context_ptr->http_response_body.string_space = 0;
                boathttpport_context_ptr->http_response_body.string_len = 0;
            }

            BoatFree(boathttpport_context_ptr);
            boathttpport_context_ptr = NULL;
        }
    }

    return boathttpport_context_ptr;
}

/*!*****************************************************************************
@brief Deinitialize boat HTTP RPC context.

Function: BoatHttpPortDeinit()

    This function de-initializes boat HTTP RPC context. It also frees the dynamically
    allocated storage to receive response from the peer.

@see BoatHttpPortInit()

@return
    This function doesn't return any value.


@param[in] boathttpport_context_ptr
    Pointer to the context of BoatHttpPortContext, which is returned by BoatHttpPortInit()

*******************************************************************************/
void BoatHttpPortDeinit(BoatHttpPortContext *boathttpport_context_ptr)
{
    if (boathttpport_context_ptr == NULL)
    {
        return;
    }

    boathttpport_context_ptr->http_response_head.string_space = 0;
    boathttpport_context_ptr->http_response_head.string_len = 0;

    boathttpport_context_ptr->http_response_body.string_space = 0;
    boathttpport_context_ptr->http_response_body.string_len = 0;

    if (boathttpport_context_ptr->http_response_head.string_ptr != NULL)
    {
        BoatFree(boathttpport_context_ptr->http_response_head.string_ptr);
        boathttpport_context_ptr->http_response_head.string_ptr = NULL;
    }

    if (boathttpport_context_ptr->http_response_body.string_ptr != NULL)
    {
        BoatFree(boathttpport_context_ptr->http_response_body.string_ptr);
        boathttpport_context_ptr->http_response_body.string_ptr = NULL;
    }

    BoatFree(boathttpport_context_ptr);

    return;
}

/*!*****************************************************************************
@brief Set options for use with boat HTTP RPC.

Function: BoatHttpPortSetOpt()

    This function set options for boat HTTP.


@return
    This function returns BOAT_SUCCESS if succeeds.
    Otherwise it returns one of the error codes.


@param[in] boathttpport_context_ptr
    A pointer to the boat HTTP RPC context

@param[in] remote_url_str
    The URL of the remote server.

*******************************************************************************/
BOAT_RESULT BoatHttpPortSetOpt(BoatHttpPortContext *boathttpport_context_ptr, BCHAR *remote_url_str)
{
    if (boathttpport_context_ptr == NULL || remote_url_str == NULL)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }

    boathttpport_context_ptr->remote_url_str = remote_url_str;

    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief Perform a synchronous HTTP POST and wait for its response.

Function: BoatHttpPortRequestSync()

    This function performs a synchronous boat HTTP POST and waits for its response.

@see https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html


@return
    This function returns BOAT_SUCCESS if succeeds.
    Otherwise it returns one of the error codes.

@param[in] boathttpport_context_ptr
    A pointer to the boat HTTP RPC context.

@param[in] request_str
    A pointer to the request string to POST.

@param[in] request_len
    The length of <request_str> excluding NULL terminator. This function is
    wrapped by RpcRequestSync() and thus takes this argument for compatibility
    with the wrapper function. Typically it equals to strlen(request_str).

@param[out] response_str_ptr
    The address of a BCHAR* pointer (i.e. a double pointer) to hold the address
    of the receiving buffer.\n
    The receiving buffer is internally maintained by curlport and the caller
    shall only read from the buffer. DO NOT modify the buffer or save the address
    for later use.

@param[out] response_len_ptr
    The address of a BUINT32 integer to hold the effective length of
    <response_str_ptr> excluding NULL terminator. This function is wrapped by
    RpcRequestSync() and thus takes this argument for compatibility with the
    wrapper function. Typically it equals to strlen(response_str_ptr).

*******************************************************************************/
BOAT_RESULT BoatHttpPortRequestSync(BoatHttpPortContext *boathttpport_context_ptr,
                                    const BCHAR *request_str,
                                    BUINT32 request_len,
                                    BOAT_OUT BCHAR **response_str_ptr,
                                    BOAT_OUT BUINT32 *response_len_ptr)
{

    httpclient_t client = {0};
    httpclient_data_t client_data = {0};
    BSINT32 response_code;
    BSINT32 ret;

    BOAT_RESULT result = BOAT_ERROR;
    boat_try_declare;

    if (boathttpport_context_ptr == NULL || request_str == NULL || response_str_ptr == NULL || response_len_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        result = BOAT_ERROR_COMMON_INVALID_ARGUMENT;
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, cleanup);
    }

    memset(&client, 0x00, sizeof(client));
    memset(&client_data, 0x00, sizeof(client_data));

    client.timeout_in_sec = 30;

    client_data.is_more = false;

    client_data.header_buf = boathttpport_context_ptr->http_response_head.string_ptr;
    client_data.header_buf_len = boathttpport_context_ptr->http_response_head.string_space;
    memset(client_data.header_buf, 0, client_data.header_buf_len);

    client_data.response_buf = boathttpport_context_ptr->http_response_body.string_ptr;
    client_data.response_buf_len = boathttpport_context_ptr->http_response_body.string_space;
    memset(client_data.response_buf, 0, client_data.response_buf_len);

    client_data.post_buf = request_str;
    client_data.post_buf_len = strlen(request_str);

    client_data.post_content_type = "application/json";

    BoatLog(BOAT_LOG_VERBOSE, "[HTTP]POST Start\n");
    ret = httpclient_post(&client, boathttpport_context_ptr->remote_url_str, &client_data);
    if (ret < 0)
    {
        BoatLog(BOAT_LOG_VERBOSE, "[HTTP]POST Failed:%d\n", ret);
        result = BOAT_ERROR_HTTP_POST_FAIL;
    }
    else
    {
        BoatLog(BOAT_LOG_VERBOSE, "[HTTP]POST:\n%s\n", client_data.response_buf);

        boathttpport_context_ptr->http_response_body.string_len = client_data.response_buf_len;

        response_code = httpclient_get_response_code(&client);

        if (response_code == 200 || response_code == 201)
        {
            *response_str_ptr = boathttpport_context_ptr->http_response_body.string_ptr;
            *response_len_ptr = boathttpport_context_ptr->http_response_body.string_len;

            BoatLog(BOAT_LOG_VERBOSE, "Post: %s", request_str);
            BoatLog(BOAT_LOG_VERBOSE, "Result Code: %d", response_code);
            BoatLog(BOAT_LOG_VERBOSE, "Response: %s", *response_str_ptr);

            result = BOAT_SUCCESS;
        }
        else
        {
            BoatLog(BOAT_LOG_NORMAL, "HTTP POST fails with response code %d.", response_code);
            result = BOAT_ERROR_HTTP_POST_FAIL - response_code;
        }
    }

    // Exceptional Clean Up
    boat_catch(cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }

    return result;
}


/**
****************************************************************************************
* @brief:
* This function init CURL_GLOBAL_DEFAULT
* On platform L610, don't need to init this param
* @return
*  This function will return BOAT_SUCCESS if executed successfully.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT BoatHttpGlobalInit(void)
{
    return BOAT_SUCCESS;
}

/**
****************************************************************************************
* @brief:
* This function deinit CURL_GLOBAL_DEFAULT
* On platform L610, don't need to init this param
* @return
*  This function has no return value
****************************************************************************************
*/
void BoatHttpGlobalDeInit(void)
{
    return BOAT_SUCCESS;
}
// #endif // end of #if RPC_USE_BOATHTTPPORT == 1
