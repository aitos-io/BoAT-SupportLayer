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

#ifndef __BOATPLATFORMDAL_H__
#define __BOATPLATFORMDAL_H__


//!@brief Abstract definition of uart object based on Linux
#ifndef PLATFORM_DAL_UART_
#define PLATFORM_DAL_UART_
struct boatPlatformUART{
    int UARTid; //! Linux UARTaphore descriptor
};
#endif

#ifndef PLATFORM_DAL_I2C
#define PLATFORM_DAL_I2C

struct boatPlatformI2C{
    i2c_Handle I2CID; //! Linux UARTaphore descriptor
};
#endif



//!@brief Abstract definition of virtual AT object based on Linux

#ifndef PLATFORM_DAL_VIRTUALAT
#define PLATFORM_DAL_VIRTUALAT
//! struct boatPlatformVirtualAT{
//!    BSINT32 virtualATID; //! Fibocom-L610 virtual AT descriptor
//! };
#endif

#endif
