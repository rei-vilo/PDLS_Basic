///
/// @file Screen_EPD_Table.h
/// @brief Library for Pervasive Displays screens and boards
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @date 21 Feb 2025
/// @version 1003
///
/// @copyright (c) Pervasive Displays Inc., 2021-2026
/// @copyright (c) Etigues, 2010-2026
/// @copyright All rights reserved
/// @copyright For exclusive use with Pervasive Displays screens
///
/// * Basic edition: for hobbyists and for basic usage
/// @n Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
/// @see https://creativecommons.org/licenses/by-sa/4.0/
///
/// @n Consider the Evaluation or Commercial editions for professionals or organisations and for commercial usage
///
/// * Evaluation edition: for professionals or organisations, evaluation only, no commercial usage
/// @n All rights reserved
///
/// * Commercial edition: for professionals or organisations, commercial usage
/// @n All rights reserved
///
/// * Viewer edition: for professionals or organisations
/// @n All rights reserved
///
/// * Documentation
/// @n All rights reserved
///

#ifndef SCREEN_EPD_TABLE_RELEASE
#define SCREEN_EPD_TABLE_RELEASE 1003

// SDK and configuration
#include "PDLS_Common.h"

///
/// @brief Type for screens table
///
typedef struct screenTable_t
{
    uint16_t code; ///< code size
    uint16_t sizeV; ///< vertical = wide size
    uint16_t sizeH; ///< horizontal = small size
} screenTable_t;

///
/// @brief Table of supported screens
///
static const screenTable_t screenTable[] =
{
    { SIZE_150, 200, 200 }, ///<  1.50"
    { SIZE_152, 200, 200 }, ///<  1.52"
    { SIZE_154, 152, 152 }, ///<  1.54"
    { SIZE_206, 248, 128 }, ///<  2.06"
    { SIZE_213, 212, 104 }, ///<  2.13"
    { SIZE_266, 296, 152 }, ///<  2.66"
    { SIZE_271, 264, 176 }, ///<  2.71"
    { SIZE_287, 296, 128 }, ///<  2.87"
    { SIZE_290, 384, 168 }, ///<  2.90"
    { SIZE_340, 392, 456 }, ///<  3.40"
    { SIZE_343, 392, 456 }, ///<  3.43"
    { SIZE_370, 416, 240 }, ///<  3.70"
    { SIZE_417, 300, 400 }, ///<  4.17"
    { SIZE_437, 480, 176 }, ///<  4.37"
    { SIZE_565, 600, 448 }, ///<  5.65"
    { SIZE_581, 720, 256 }, ///<  5.81"
    { SIZE_741, 800, 480 }, ///<  7.41"
    { SIZE_969, 672, 960 }, ///<  9.69", 2 * 480
    { SIZE_1198, 768, 960 }, ///<  11.98", 2 * 480
    { SIZE_NONE, 0, 0 } ///< End
};

#endif // SCREEN_EPD_TABLE_RELEASE
