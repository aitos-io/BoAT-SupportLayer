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

/*!@brief Boatwallet options

@file
boatLogConfig.h defines options for compiling.
*/

#ifndef __BOATLOG_H__
#define __BOATLOG_H__

#include "boattypes.h"
#include "osi_log.h"

//! BOAT LOG LEVEL DEFINITION
//! Log level is used to control the detail of log output.
//! 3 types of detail level can be specified in BoatLog():
//! "CRITICAL" is for critical exceptions that may halt the wallet from going on.
//! "NORMAL" is for warnings that may imply some error but could be held.
//! "VERBOSE" is for detail information that is only useful for debug.
#define BOAT_LOG_NONE 0
#define BOAT_LOG_CRITICAL 1
#define BOAT_LOG_NORMAL 2
#define BOAT_LOG_VERBOSE 3

//! BOAT_LOG_LEVEL is a macro that limits the log detail up to that level.
//! Seting it to BOAT_LOG_NONE means outputing nothing.
#define BOAT_LOG_LEVEL BOAT_LOG_VERBOSE

// void BoatPrintf(0,char* fmt, ...);
/////#define BoatPrintf OSI_PRINTFI
#define BoatPrintf OSI_LOGE
extern const BCHAR *const g_log_level_name_str[];

/*!****************************************************************************
 * @brief Log Output
 *
 * @param level
 *   Log priority level.
 *   One of BOAT_LOG_CRITICAL, BOAT_LOG_NORMAL or BOAT_LOG_VERBOSE.
 *
 *@param format
 *   Similar to that in printf().
 ******************************************************************************/
#if BOAT_LOG_LEVEL == BOAT_LOG_NONE
#define BoatLog(level, format, ...)
#else
#define BoatLog(level, format, ...)                                                         \
    do                                                                                      \
    {                                                                                       \
        if (level <= BOAT_LOG_LEVEL)                                                        \
        {                                                                                   \
            BoatPrintf(0," [BOAT] %s: "__FILE__                                               \
                       ":%d, %s(): " format "\r\n",                                           \
                       g_log_level_name_str[level - 1], __LINE__, __func__, ##__VA_ARGS__); \
        }                                                                                   \
    } while (0)
#endif
#if 0

#endif
/*!****************************************************************************
 * @brief Log Output in hex mode
 *
 * @param level
 *   Log priority level.
 *   One of BOAT_LOG_CRITICAL, BOAT_LOG_NORMAL or BOAT_LOG_VERBOSE.
 *
 * @param title
 *   Tips of this dump.
 *
 * @param buf
 *   Buffer to be dump.
 *
 * @param len
 *   Size of parameter <buf>.
 *
 * @see BoatLog BoatLog_hexasciidump
 ******************************************************************************/
#if BOAT_LOG_LEVEL == BOAT_LOG_NONE
#define BoatLog_hexdump(level, title, buf, len)
#else
#define BoatLog_hexdump(level, title, buf, len)                                                           \
    do                                                                                                    \
    {                                                                                                     \
        if (level <= BOAT_LOG_LEVEL)                                                                      \
        {                                                                                                 \
            BoatPrintf(0,"%s: %s[%03d]: ", g_log_level_name_str[level - 1], title, len);                    \
            if (len > 0)                                                                                  \
            {                                                                                             \
                for (int i = 0; i < len; i++)                                                             \
                {                                                                                         \
                    BoatPrintf(0,"%c%c", "0123456789ABCDEF"[buf[i] / 16], "0123456789ABCDEF"[buf[i] % 16]); \
                }                                                                                         \
            }                                                                                             \
            else                                                                                          \
            {                                                                                             \
                BoatPrintf(0,"(nil)");                                                                      \
            }                                                                                             \
            BoatPrintf(0,"\n");                                                                             \
        }                                                                                                 \
    } while (0)
#endif

/*!****************************************************************************
 * @brief Log Output in hex and acsii mode
 *
 * @param level
 *   Log priority level.
 *   One of BOAT_LOG_CRITICAL, BOAT_LOG_NORMAL or BOAT_LOG_VERBOSE.
 *
 * @param title
 *   Tips of this dump.
 *
 * @param buf
 *   Buffer to be dump.
 *
 * @param len
 *   Size of parameter <buf>.
 *
 * @see BoatLog BoatLog_hexdump
 ******************************************************************************/
#if BOAT_LOG_LEVEL == BOAT_LOG_NONE
#define BoatLog_hexasciidump(level, title, buf, len)
#else
#define BoatLog_hexasciidump(level, title, buf, len)                                                     \
    do                                                                                                   \
    {                                                                                                    \
        if (level <= BOAT_LOG_LEVEL)                                                                     \
        {                                                                                                \
            BoatPrintf(0,"%s: %s[%03d]: \n", g_log_level_name_str[level - 1], title, len);                 \
            if (len > 0)                                                                                 \
            {                                                                                            \
                for (int j = 0; j < len / 16 + 1; j++)                                                   \
                {                                                                                        \
                    BoatPrintf(0,"%08x  ", j * 16);                                                        \
                    for (int i = 0; i < 16; i++)                                                         \
                    {                                                                                    \
                        BoatPrintf(0,"%c%c ",                                                              \
                                   "0123456789ABCDEF "[(i + j * 16 < len) ? buf[i + j * 16] / 16 : 16],  \
                                   "0123456789ABCDEF "[(i + j * 16 < len) ? buf[i + j * 16] % 16 : 16]); \
                    }                                                                                    \
                    BoatPrintf(0," |");                                                                    \
                    for (int i = 0; i < 16; i++)                                                         \
                    {                                                                                    \
                        if (i + j * 16 < len)                                                            \
                        {                                                                                \
                            if ((buf[i + j * 16] > 32) && (buf[i + j * 16] < 127))                       \
                            {                                                                            \
                                BoatPrintf(0,"%c", buf[i + j * 16]);                                       \
                            }                                                                            \
                            else                                                                         \
                            {                                                                            \
                                BoatPrintf(0,".");                                                         \
                            }                                                                            \
                        }                                                                                \
                        else                                                                             \
                        {                                                                                \
                            BoatPrintf(0," ");                                                             \
                        }                                                                                \
                    }                                                                                    \
                    BoatPrintf(0,"|\n");                                                                   \
                }                                                                                        \
            }                                                                                            \
            else                                                                                         \
            {                                                                                            \
                BoatPrintf(0,"(nil)");                                                                     \
            }                                                                                            \
            BoatPrintf(0,"\n");                                                                            \
        }                                                                                                \
    } while (0)
#endif
#endif