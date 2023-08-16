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

#include "boattypes.h"

#include "sc_i2c.h"
#include "simcom_common.h"
#include "simcom_os.h"
#include "simcom_ssl.h"

#ifndef __BOATPLATFORMDAL_H__
#define __BOATPLATFORMDAL_H__

#define BOAT_TLS_SUPPORT 1
#define BOAT_TLS_IDENTIFY_CLIENT 1

//!@brief Abstract definition of uart object based on Linux
#ifndef PLATFORM_DAL_UART
#define PLATFORM_DAL_UART
struct boatPlatformUART
{
    BUINT8 uartPort; //! UART descriptor
};
#endif // PLATFORM_DAL_UART

#ifndef PLATFORM_DAL_I2C
#define PLATFORM_DAL_I2C

struct boatPlatformI2C
{
 //   mbtk_i2c_devno_enum i2cId; //! only  support devno_2
};
#endif // PLATFORM_DAL_I2C

//!@brief Abstract definition of virtual AT object based on Linux

#ifndef PLATFORM_DAL_VIRTUALAT
#define PLATFORM_DAL_VIRTUALAT
//! struct boatPlatformVirtualAT{
//!    BSINT32 virtualATID; //! Fibocom-L610 virtual AT descriptor
//! };

extern void dal_virtualAT_callback(BUINT8 *buf, BUINT16 len);

#endif

#ifndef PLATFORM_DAL_SSL
#define PLATFORM_DAL_SSL

struct boatPlatformSSL
{
    // i2c_Handle I2CID; //! Linux UARTaphore descriptor
    // void;
    SCSslCtx_t SSLCtx;

};
#endif


#endif
