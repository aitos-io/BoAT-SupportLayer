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

#include "boathttpport.h"

#include "boatlog.h"
#include "boatplatformosal.h"

#include "ql_http_client.h"

#define HTTPSPROFILE_IDX 1
#define PRINT_BUF_SIZE 65

static QL_HTTP_CLIENT_EVENT_E httpsrequeststatus = QL_HTTP_CLIENT_EVENT_SEND_FAIL; 
static BUINT8 httpsrequestcbstate = 0;
#define HTTPSREQUESTCBLOCK 0xba
#define HTTPSREQUESTCUNBLOCK 0xab
static ql_sem_t httpsrequeststatussemaphore = NULL;

typedef struct{
int code;
BoatHttpPortContext *boathttpport_context_ptr;
}priData;




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

    httpsrequeststatus = 0;
    httpsrequestcbstate = 0;

    ql_rtos_semaphore_create(&httpsrequeststatussemaphore, 0);


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

	ql_rtos_semaphore_delete(httpsrequeststatussemaphore);

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

void httpsrequestreleasesemaphore(void)
{
    BUINT32 cnt = 0;
    if(ql_rtos_semaphore_get_cnt(httpsrequeststatussemaphore, &cnt) == 0) // succ
    {
        if(cnt == 0)
        {
            ql_rtos_semaphore_release(httpsrequeststatussemaphore);
        }            
    }
}


static int httpsresponsecb(
    QL_HTTP_CLIENT_T *client,
    QL_HTTP_CLIENT_EVENT_E event,
    int status_code, 
    char *data, 
    int data_len, 
    void *private_data)
{
    int ret=0;

    priData *pHttpPriData = (priData *)private_data;
    
    // set the https status
    if(httpsrequestcbstate != HTTPSREQUESTCBLOCK)
    {
        httpsrequeststatus = event;
    }

    switch(event)
    {
        case QL_HTTP_CLIENT_EVENT_SEND_FAIL:
            BoatLog(BOAT_LOG_VERBOSE, "http send failed!\n");
			httpsrequeststatus = 0;
            httpsrequestcbstate = HTTPSREQUESTCBLOCK; 
            httpsrequestreleasesemaphore(); // this cb can not change the stat before a new https request
            break;
        case QL_HTTP_CLIENT_EVENT_SEND_SUCCESSED:
            BoatLog(BOAT_LOG_VERBOSE, "http send successed! \n");

            break;
        case QL_HTTP_CLIENT_EVENT_RECV_HEADER_FAIL:
            BoatLog(BOAT_LOG_VERBOSE, "http parse response header failed!\n");
            httpsrequestcbstate = HTTPSREQUESTCBLOCK; 
		
            httpsrequestreleasesemaphore(); // this cb can not change the stat before a new https request
            break;
        case QL_HTTP_CLIENT_EVENT_RECV_HEADER_FINISHED:
            BoatLog(BOAT_LOG_VERBOSE, "http recv header status_code:%d;header_len:%d!\n",status_code,data_len);
			pHttpPriData->code = status_code;
			if(pHttpPriData->boathttpport_context_ptr->http_response_head.string_len >
				pHttpPriData->boathttpport_context_ptr->http_response_head.string_space - data_len)
			{
				data_len = pHttpPriData->boathttpport_context_ptr->http_response_head.string_space -
					pHttpPriData->boathttpport_context_ptr->http_response_head.string_len;
			}
			
			memcpy(pHttpPriData->boathttpport_context_ptr->http_response_head.string_ptr + 
				pHttpPriData->boathttpport_context_ptr->http_response_head.string_len, data, data_len);

			pHttpPriData->boathttpport_context_ptr->http_response_head.string_len += data_len;
			ret = 1;
            break;
        case QL_HTTP_CLIENT_EVENT_RECV_BODY:
            BoatLog(BOAT_LOG_VERBOSE, "http recv body status_code:%d;recv_body_len:%d!\n",status_code,data_len);
			if(pHttpPriData->boathttpport_context_ptr->http_response_body.string_len >
				pHttpPriData->boathttpport_context_ptr->http_response_body.string_space - data_len)
			{
				data_len = pHttpPriData->boathttpport_context_ptr->http_response_body.string_space -
					pHttpPriData->boathttpport_context_ptr->http_response_body.string_len;
			}
			
			memcpy(pHttpPriData->boathttpport_context_ptr->http_response_body.string_ptr + 
				pHttpPriData->boathttpport_context_ptr->http_response_body.string_len, data, data_len);

			pHttpPriData->boathttpport_context_ptr->http_response_body.string_len += data_len;
            ret = 1; 
            break;
        case QL_HTTP_CLIENT_EVENT_RECV_BODY_FINISHED:
            BoatLog(BOAT_LOG_VERBOSE, "http recv body finished!\n");
            httpsrequestcbstate = HTTPSREQUESTCBLOCK; 
            httpsrequeststatus = event;
            httpsrequestreleasesemaphore(); // this cb can not change the stat before a new https request

            break;
        case QL_HTTP_CLIENT_EVENT_DISCONNECTED:
            break;
        default:
            break;
    }

    httpsrequeststatus = event;
    return ret;
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

    int  ret;
    QL_HTTP_CLIENT_T * client = NULL;
    QL_HTTP_CLIENT_LIST_T * header = NULL;

    BOAT_RESULT result = BOAT_ERROR;
    
    boat_try_declare;

    if (boathttpport_context_ptr == NULL || request_str == NULL || response_str_ptr == NULL || response_len_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Argument cannot be NULL.");
        result = BOAT_ERROR_COMMON_INVALID_ARGUMENT;
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, cleanup);
    }

    if(client == NULL)
    {
        
		client = ql_http_client_init();
		
		ret = ql_http_client_setopt(client, QL_HTTP_CLIENT_OPT_PDP_CID, 1); //set PDP cid,if not set,using default PDP
		if(ret != 0)
		{
            result = BOAT_ERROR_HTTP_INIT_FAIL;
            boat_throw(result,cleanup);
		}
		ret = ql_http_client_setopt(client, QL_HTTP_CLIENT_OPT_PROTOCOL_VER, 1); //"0" is HTTP 1.1, "1" is HTTP 1.0		
		if(ret != 0)
		{
            result = BOAT_ERROR_HTTP_INIT_FAIL;
            boat_throw(result,cleanup);
		}
		ret = ql_http_client_setopt(client, QL_HTTP_CLIENT_OPT_ENABLE_COOKIE, 1); //"0" is HTTP 1.1, "1" is HTTP 1.0
		if(ret != 0)
		{
            result = BOAT_ERROR_HTTP_INIT_FAIL;
            boat_throw(result,cleanup);
		}

        if(0 == strncmp(boathttpport_context_ptr->remote_url_str,"https",strlen("https"))) //  debug case sensitive
        {
			SSLConfig sslConfig = 
			{
				.en = 1,
				.profileIdx = HTTPSPROFILE_IDX,
				.serverName = "dev.api.joy-notary.molian.technology", // mmp: input parameter is better than const string //"www.baidu.com",	// maybe the part of hqd.url's domain name
				.serverPort = 443,
				.protocol = 0,
				.dbgLevel = 1,
				.sessionReuseEn = 0,
				.vsn = SSL_VSN_ALL,
				.verify = SSL_VERIFY_MODE_OPTIONAL,
				.cert.from = SSL_CERT_FROM_BUF,
				.cert.path.rootCA = NULL, // rootCA_path_aitos, // aitosroot.pem // debug, need a interface to get CA data
				.cert.path.clientKey = NULL,
				.cert.path.clientCert = NULL,
				.cert.clientKeyPwd.data = NULL,
				.cert.clientKeyPwd.len = 0,
				.cipherList = "ALL",
				.CTRDRBGSeed.data = NULL,
				.CTRDRBGSeed.len = 0
			};

			ret = ql_http_client_setopt(client, QL_HTTP_CLIENT_OPT_SSL_CTX, &sslConfig); // CA certification 
			if(ret != 0)
			{
				result = BOAT_ERROR_HTTP_INIT_FAIL;
				boat_throw(result,cleanup);
			}
        }
        else if(0 == strncmp(boathttpport_context_ptr->remote_url_str,"http",strlen("http"))) //  debug case sensitive
        {
			ret = ql_http_client_setopt(client, QL_HTTP_CLIENT_OPT_SSL_CTX, NULL); // no CA certification 
			if(ret != 0)
			{
				result = BOAT_ERROR_HTTP_INIT_FAIL;
				boat_throw(result,cleanup);
			}
        }
        else
        {
            BoatLog(BOAT_LOG_CRITICAL, "Url format is Wrong.");
            result = BOAT_ERROR_COMMON_INVALID_ARGUMENT;
            boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, cleanup);
        }

		header = ql_http_client_list_append(header, "Connection: keep-alive\r\n"); // end with \r\n	
		header = ql_http_client_list_append(header, "Content-type: application/json\r\n"); // end with \r\n		
		ret = ql_http_client_setopt(client, QL_HTTP_CLIENT_OPT_HTTPHEADER, header);
		if(ret != 0)
		{
            result = BOAT_ERROR_HTTP_INIT_FAIL;
            boat_throw(result,cleanup);
		}

		priData httpPriData;
		httpPriData.code = 0;
		httpPriData.boathttpport_context_ptr = boathttpport_context_ptr;
		
	    ret=ql_http_client_request(client, boathttpport_context_ptr->remote_url_str,
                                QL_HTTP_CLIENT_REQUEST_POST,
                                QL_HTTP_CLIENT_AUTH_TYPE_BASE,
                                NULL,NULL, (char *)request_str, request_len,
                                httpsresponsecb,&httpPriData);
		if(ret != 0)
		{
            result = BOAT_ERROR_HTTP_CONNECT_FAIL;
            boat_throw(result,cleanup);
		}
		
		BoatLog(BOAT_LOG_VERBOSE, "ql_http_client_request ret=%d!\n",ret);
		
		ret = ql_rtos_semaphore_wait(httpsrequeststatussemaphore, 20000);
		int status = httpsrequeststatus;
		BoatLog(BOAT_LOG_VERBOSE, "ql_rtos_semaphore_wait status:%d %d\r\n", status, ret); //gethttpsrequeststatus(),ret);
		if(ret == 0)
		{
			if(QL_HTTP_CLIENT_EVENT_RECV_BODY_FINISHED == status) //gethttpsrequeststatus()) // data recived
			{
				if((200 == httpPriData.code) || (201 == httpPriData.code))
				{
					*response_str_ptr = boathttpport_context_ptr->http_response_body.string_ptr;
					*response_len_ptr = boathttpport_context_ptr->http_response_body.string_len;
					result = BOAT_SUCCESS;
				}
				else
				{
					BoatLog(BOAT_LOG_CRITICAL,"Http Response code is Wrong!");
					result = BOAT_ERROR_HTTP_POST_FAIL;
				}
			}
			else
			{
				ret = BOAT_ERROR;
			}
		}
			
		ql_http_client_list_destroy(header);
		ql_http_client_release(client); /*release http resources*/

    }

    BoatSleepMs(100);  //delay 100ms

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
    return;
}

// #endif // end of #if RPC_USE_BOATHTTPPORT == 1
