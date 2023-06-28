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

 #if RPC_USE_BOATHTTPPORT == 1

// #include "rpcport.h"
#include "boathttpport.h"

#include "httpclient.h"
#include "boatlog.h"
#include "boatplatformosal.h"

#include "http_api.h"


__BOATSTATIC void response_cb( char *data_ptr, int size, 
											     int nmemb, void *userdata )
{
    size_t data_size;
    StringWithLen *mem;
    BUINT32 expand_size;
    BUINT32 expand_steps;
    BCHAR *expanded_str;
    BUINT32 expanded_to_space;
    
    mem = (StringWithLen*)userdata;

    // NOTE: For historic reasons, argument size is always 1 and nmemb is the
    // size of the data chunk to write. And size * nmemb doesn't include null
    // terminator even if the data were string.
    data_size = size * nmemb;
    BoatLog(BOAT_LOG_NORMAL, "response_cb data_size : %d.", data_size);
    // If response buffer has enough space:
    if( mem->string_space - mem->string_len > data_size ) // 1 more byte reserved for null terminator
    {
        memcpy(mem->string_ptr + mem->string_len, data_ptr, data_size);
        mem->string_len += data_size;
        mem->string_ptr[mem->string_len] = '\0';
    }
    else // If response buffer has no enough space
    {
        // If malloc is supported, expand the response buffer in steps of
        // CURLPORT_RECV_BUF_SIZE_STEP.
        expand_size  = data_size - (mem->string_space - mem->string_len) + 1; // plus 1 for null terminator
        expand_steps = (expand_size - 1) / OLHTTPPORT_RECV_BUF_SIZE_STEP + 1;
        expanded_to_space = expand_steps * OLHTTPPORT_RECV_BUF_SIZE_STEP + mem->string_space;
    
        expanded_str = BoatMalloc(expanded_to_space);

        if( expanded_str != NULL )
        {
            memcpy(expanded_str, mem->string_ptr, mem->string_len);
            memcpy(expanded_str + mem->string_len, data_ptr, data_size);
            BoatFree(mem->string_ptr);
            mem->string_ptr = expanded_str;
            mem->string_space = expanded_to_space;
            mem->string_len += data_size;
            mem->string_ptr[mem->string_len] = '\0';
        }

    }

}


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

    BSINT32 response_code;
    BSINT32 ret;

    http_client * client = NULL;
	struct http_client_list * header = NULL;

    long info;
    BOAT_RESULT result = BOAT_ERROR;
    int olhttp_result = 0;
    boat_try_declare;


    if (boathttpport_context_ptr == NULL || request_str == NULL || response_str_ptr == NULL || response_len_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        result = BOAT_ERROR_COMMON_INVALID_ARGUMENT;
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, cleanup);
    }

    client = ol_http_client_init();
    
    if(client == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "ol_http_client_init() fails.");
        boat_throw(BOAT_ERROR_HTTP_INIT_FAIL, cleanup);
    }
    BoatLog(BOAT_LOG_CRITICAL, "olhttp url = %s .",boathttpport_context_ptr->remote_url_str);

    // Set RPC URL in format "<protocol>://<target name or IP>:<port>". e.g. "http://192.168.56.1:7545"
    olhttp_result = ol_http_client_setopt(client, HTTPCLIENT_OPT_URL, boathttpport_context_ptr->remote_url_str);
    if( olhttp_result != HTTP_CLIENT_OK )
    {
        BoatLog(BOAT_LOG_NORMAL, "Unknown URL: %s", boathttpport_context_ptr->remote_url_str);
        boat_throw(BOAT_ERROR_HTTP_PARSE_FAIL, cleanup);
    }

	ol_http_client_setopt(client, HTTPCLIENT_OPT_HTTP1_0, 0);	

    // Set HTTP Type: POST
	ol_http_client_setopt(client, HTTPCLIENT_OPT_METHOD, HTTPCLIENT_REQUEST_POST);	/*set method,support GET/POST/PUT*/

    // Set HTTP HEADER Options
	header = ol_http_client_list_append(header, "Content-Type:application/json;charset=UTF-8\r\n");

    ol_http_client_setopt(client, HTTPCLIENT_OPT_HTTPHEADER, header);

    // Set callback and receive buffer for RESPONSE
    // Clean up response buffer
    memset(boathttpport_context_ptr->http_response_body.string_ptr,U0,boathttpport_context_ptr->http_response_body.string_space);
    boathttpport_context_ptr->http_response_body.string_len = 0;

    ol_http_client_setopt(client, HTTPCLIENT_OPT_RESPONSECB_DATA, boathttpport_context_ptr->http_response_body.string_ptr);
	ol_http_client_setopt(client, HTTPCLIENT_OPT_RESPONSECB, response_cb);


    // Set content to POST    
	ol_http_client_setopt(client, HTTPCLIENT_OPT_POSTDATA, request_str);
	ol_http_client_setopt(client, HTTPCLIENT_OPT_POSTLENGTH, request_len);

    ol_http_client_getinfo(client, HTTPCLIENT_GETINFO_TCP_STATE, &olhttp_result);

    if( olhttp_result != HTTP_TCP_ESTABLISHED )
    {
        BoatLog(BOAT_LOG_NORMAL, "http tcp not connecting: %d.", olhttp_result);
        boat_throw(BOAT_ERROR_CURL_CODE_FAIL - olhttp_result, cleanup);
    }

    // Perform the RPC request
    olhttp_result = ol_http_client_perform(client);

    if( olhttp_result != HTTP_CLIENT_OK )
    {
        BoatLog(BOAT_LOG_NORMAL, "ol_http_client_perform fails: %d.", olhttp_result);
        boat_throw(BOAT_ERROR_CURL_CODE_FAIL - olhttp_result, cleanup);
    }
    
    olhttp_result = ol_http_client_getinfo(client, HTTPCLIENT_GETINFO_RESPONSE_CODE, &info);

    if(( olhttp_result == HTTP_CLIENT_OK ) && (info != HTTP_TCP_ESTABLISHED))
    {
        *response_str_ptr = boathttpport_context_ptr->http_response_body.string_ptr;
        *response_len_ptr = boathttpport_context_ptr->http_response_body.string_len;
        
        BoatLog(BOAT_LOG_VERBOSE, "Post: %s", request_str);
        BoatLog(BOAT_LOG_VERBOSE, "Result Code: %ld", info);
        BoatLog(BOAT_LOG_VERBOSE, "Response: %s", *response_str_ptr);
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "HTTP POST fails with response code %ld.", info);
        result = BOAT_ERROR_HTTP_POST_FAIL - response_code;
    }    

    // Clean Up
    if (header) 
    {
		ol_http_client_list_destroy(header);
		header = NULL;
	}
    ol_http_client_shutdown(client);
    result = BOAT_SUCCESS;


    // Exceptional Clean Up
    boat_catch(cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);

        if( header != NULL )
        {
		    ol_http_client_list_destroy(header);
            header = NULL;
        }

        if( client != NULL )
        {
            ol_http_client_shutdown(client);
        }
        
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
 #endif // end of #if RPC_USE_BOATHTTPPORT == 1
