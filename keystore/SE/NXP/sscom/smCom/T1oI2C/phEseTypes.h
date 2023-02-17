/*
 * Copyright 2010-2014,2018-2019 NXP
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PHESETYPES_H
#define PHESETYPES_H
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "boatlog.h"
#include "boatosal.h"

#define I2C_OK 0
#define I2C_FAILED 13
#define I2C_NACK_ON_ADDRESS -1

typedef unsigned int i2c_error_t;

#define I2C_BUS_0 (0)

#define MAX_DATA_LEN 260

typedef uint8_t utf8_t;     /* UTF8 Character String */
typedef uint8_t bool_t;     /* boolean data type */
typedef uint16_t ESESTATUS; /* Return values */
#define STATIC static

#define UNUSED(X) (void)X;

#define sm_sleep BoatSleepMs

#ifndef T1oI2C_UM11225
#define T1oI2C_UM11225
#endif

#ifndef NX_ENSURE_DO_LOG_MESSAGE
#define NX_ENSURE_DO_LOG_MESSAGE 1
#endif /* NX_ENSURE_DO_LOG_MESSAGE */

#define MAX_APDU_BUF_LENGTH (256 + 1024) // This value has not been optimized for TGT_A71CH (256+64)

#if NX_ENSURE_DO_LOG_MESSAGE
#define NX_ENSURE_MESSAGE(strCONDITION) \
    BoatLog(BOAT_LOG_NORMAL, "nxEnsure:'" strCONDITION "' failed. At Line:%d Function:%s", __LINE__, __FUNCTION__)
#else                                   /* NX_ENSURE_DO_LOG_MESSAGE */
#define NX_ENSURE_MESSAGE(strCONDITION) /* No Message */
#endif                                  /* NX_ENSURE_DO_LOG_MESSAGE */

#define ENSURE_OR_GO_EXIT(CONDITION)   \
    if (!(CONDITION))                  \
    {                                  \
        NX_ENSURE_MESSAGE(#CONDITION); \
        goto exit;                     \
    }

#ifndef SSS_MALLOC
#define SSS_MALLOC BoatMalloc
#endif // SSS_MALLOC

#ifndef SSS_FREE
#define SSS_FREE BoatFree
#endif // SSS_FREE

#endif /* PHESETYPES_H */
