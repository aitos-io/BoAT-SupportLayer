/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/*!@brief RPC wrapper

@file
rpcintf.c contains wrapper functions for RPC.
The exact implementation of the actual RPC mechanism is controlled by
RPC_USE_XXX macros.
*/

#include "boatconfig.h"
#include "boatinternal.h"
#include "boathttpport.h"

void *RpcInit(void)
{
    void *rpc_context_ptr = NULL;

    rpc_context_ptr = BoatHttpPortInit();

    return rpc_context_ptr;
}

void RpcDeinit(void *rpc_context_ptr)
{
    if (rpc_context_ptr == NULL)
    {
        return;
    }

    BoatHttpPortDeinit(rpc_context_ptr);
}

BOAT_RESULT RpcRequestSet(void *rpc_context_ptr, BCHAR *remote_url_str)
{
    // BOAT_RESULT result = BOAT_ERROR;

    return BoatHttpPortSetOpt((BoatHttpPortContext *)rpc_context_ptr, remote_url_str);
}

BOAT_RESULT RpcRequestSync(void *rpc_context_ptr,
                           BUINT8 *request_ptr,
                           BUINT32 request_len,
                           BOAT_OUT BUINT8 **response_pptr,
                           BOAT_OUT BUINT32 *response_len_ptr)
{
    BOAT_RESULT result = BOAT_ERROR;

    result = BoatHttpPortRequestSync(rpc_context_ptr, (const BCHAR *)request_ptr, request_len, (BOAT_OUT BCHAR **)response_pptr, response_len_ptr);

    return result;
}
