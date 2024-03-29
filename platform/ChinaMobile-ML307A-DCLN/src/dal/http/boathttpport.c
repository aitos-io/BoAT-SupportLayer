/**
****************************************************************************************
* @FilePath: boathttpport.c
* @Author: aitos
* @Date: 2023-3-21 16:25:53
* @LastEditors:
* @LastEditTime: 2023-3-21 16:30:54
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

//#include "httpclient.h"
#include "boatlog.h"
#include "boatplatformosal.h"

#include "cm_http.h"
#include "cm_ssl.h"

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
            memset(boathttpport_context_ptr->http_response_head.string_ptr,0U,boathttpport_context_ptr->http_response_head.string_space);
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
            memset(boathttpport_context_ptr->http_response_body.string_ptr,0U,boathttpport_context_ptr->http_response_body.string_space);
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

    cm_httpclient_ret_code_e  ret;
    cm_httpclient_handle_t client = NULL;

    BOAT_RESULT result = BOAT_ERROR;
    cm_httpclient_sync_param_t param = {};
    cm_httpclient_sync_response_t response = {};
    
    boat_try_declare;

    if (boathttpport_context_ptr == NULL || request_str == NULL || response_str_ptr == NULL || response_len_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        result = BOAT_ERROR_COMMON_INVALID_ARGUMENT;
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, cleanup);
    }

    

    if(client == NULL)
    {
        

        cm_httpclient_cfg_t client_cfg;
        if(0 == strncmp(boathttpport_context_ptr->remote_url_str,"https",strlen("https")))
        {
            client_cfg.ssl_enable = true;                                                   //Use SSL，HTTPS
            client_cfg.ssl_id = 2;                                                          //Set ssl id
        }
        else if(0 == strncmp(boathttpport_context_ptr->remote_url_str,"http",strlen("http")))
        {
            client_cfg.ssl_enable = false;                                                   //Don't use SSL，HTTP
            //client_cfg.ssl_id = 2;                                                          //Set ssl id
        }
        else
        {
            BoatLog(BOAT_LOG_CRITICAL, "Url format is Wrong.");
            result = BOAT_ERROR_COMMON_INVALID_ARGUMENT;
            boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, cleanup);
        }

        ret = cm_httpclient_create((const uint8_t *)(boathttpport_context_ptr->remote_url_str), NULL, &client);
        if (CM_HTTP_RET_CODE_OK != ret || NULL == client)
        {
            BoatLog(BOAT_LOG_CRITICAL, "Create HTTP instance ERROR.");
            result = BOAT_ERROR_HTTP_INIT_FAIL;
            boat_throw(result,cleanup);
        }

        BoatSleepMs(100);  //delay 100ms

        ret = cm_httpclient_custom_header_set(client,"Content-Type: application/json\r\n",strlen("Content-Type: application/json\r\n"));
        BoatLog(BOAT_LOG_CRITICAL, "Set custom header ret = %d",ret);

        BoatSleepMs(100);  //delay 100ms

        client_cfg.cid = 0;                                                             //Can be any value
        client_cfg.conn_timeout = HTTPCLIENT_CONNECT_TIMEOUT_MAXTIME;
        client_cfg.rsp_timeout = HTTPCLIENT_WAITRSP_TIMEOUT_MAXTIME;
        client_cfg.dns_priority = 1;                                                    //ip v4 first
        ret = cm_httpclient_set_cfg(client, client_cfg);                 

        if (CM_HTTP_RET_CODE_OK != ret || NULL == client)
        {
            BoatLog(BOAT_LOG_CRITICAL, "HttpClient set config ERROR.");
            result = BOAT_ERROR_HTTP_INIT_FAIL;
            boat_throw(result,cleanup);
        }

        //int tmp = 0;
        //cm_ssl_setopt(2 ,CM_SSL_PARAM_VERIFY, &tmp);                                    //Set ssl verify mode

    }

    BoatSleepMs(100);  //delay 100ms
    param.method = HTTPCLIENT_REQUEST_POST;
    param.path = (const uint8_t *)"/";
    param.content = (uint8_t *)request_str;
    param.content_length = request_len;

    ret = cm_httpclient_sync_request(client, param, &response);                         //Send http request
    if (CM_HTTP_RET_CODE_OK != ret || NULL == client)
    {
        BoatLog(BOAT_LOG_CRITICAL,"HttpClient POST Failed,ret = %d",ret);
        result  = BOAT_ERROR_HTTP_POST_FAIL;
    }
    else
    {
        BoatLog(BOAT_LOG_VERBOSE,"Http POST response_code is %d", response.response_code);
        BoatLog(BOAT_LOG_VERBOSE,"Http POST response_header: %s", response.response_header);
        BoatLog(BOAT_LOG_VERBOSE,"Http POST response_header_len is %d", response.response_header_len);
        BoatLog(BOAT_LOG_VERBOSE,"Http POST response_content: %s", response.response_content);
        BoatLog(BOAT_LOG_VERBOSE,"Http POST response_content_len is %d", response.response_content_len);

        if((200 == response.response_code) || (201 == response.response_code))
        {
            //Get response data
            if((response.response_header_len < boathttpport_context_ptr->http_response_head.string_space) && \
                (response.response_content_len < boathttpport_context_ptr->http_response_body.string_space))
            {
                memcpy(boathttpport_context_ptr->http_response_head.string_ptr,response.response_header,response.response_header_len);
                boathttpport_context_ptr->http_response_head.string_len = response.response_header_len;

                memcpy(boathttpport_context_ptr->http_response_body.string_ptr,response.response_content,response.response_content_len);
                boathttpport_context_ptr->http_response_body.string_len = response.response_content_len;

                *response_str_ptr = boathttpport_context_ptr->http_response_body.string_ptr;
                *response_len_ptr = boathttpport_context_ptr->http_response_body.string_len;

                result = BOAT_SUCCESS;
            }
            else
            {
                BoatLog(BOAT_LOG_CRITICAL,"Http response message is too long!");
                result = BOAT_ERROR_COMMON_OUT_OF_MEMORY;
            }
        }
        else
        {
            BoatLog(BOAT_LOG_CRITICAL,"Http Response code is Wrong!");
            result = BOAT_ERROR_HTTP_POST_FAIL;
        }
        

    }

    cm_httpclient_custom_header_free(client);
    BoatSleepMs(100);  //delay 100ms

    // Exceptional Clean Up
    boat_catch(cleanup)
    {
        
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        result = boat_exception;
    }

    if(client != NULL)
    {

            cm_httpclient_terminate(client);
            BoatSleepMs(100);
            cm_httpclient_sync_free_data(client);                                        //Release client
            BoatSleepMs(100);
            cm_httpclient_delete(client);
            client = NULL;

            BoatSleepMs(150);   //delay 150ms

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
