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

/*!@brief Define the abstract types of DAL based on Linux,
 * and include the external header files.

@file
* platform_dal.h defines the abstract types for boatDAL.h.
* Do not use the types in platform.h directly.
*/

#include "drv_i2c.h"
#include "fibo_opencpu.h"

#ifndef __BOATPLATFORMDAL_H__
#define __BOATPLATFORMDAL_H__


//!@brief Abstract definition of uart object based on Linux
#ifndef PLATFORM_DAL_UART
#define PLATFORM_DAL_UART
struct boatPlatformUART{
    hal_uart_port_t uartPort; //! Fibocom  UART descriptor
};
#endif//PLATFORM_DAL_UART

#ifndef PLATFORM_DAL_I2C
#define PLATFORM_DAL_I2C

struct boatPlatformI2C{
    i2c_Handle i2cId; //! Linux UARTaphore descriptor
};
#endif//PLATFORM_DAL_I2C



//!@brief Abstract definition of virtual AT object based on Linux

#ifndef PLATFORM_DAL_VIRTUALAT
#define PLATFORM_DAL_VIRTUALAT
//! struct boatPlatformVirtualAT{
//!    BSINT32 virtualATID; //! Fibocom-L610 virtual AT descriptor
//! };

extern void dal_virtualAT_callback(BUINT8 *buf, BUINT16 len);

#endif

#ifndef PLATFORM_DAL_BLE_SERVICE
#define PLATFORM_DAL_BLE_SERVICE

#include "fibo_opencpu.h"

extern void dal_bleService_sig_callback(GAPP_SIGNAL_ID_T sig, va_list arg);

#endif

#endif
