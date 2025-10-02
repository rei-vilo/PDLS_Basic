///
/// @file PDLS_Basic.h
/// @brief Driver for Pervasive Displays iTC screens and EXT3 extension board
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @date 21 Aug 2025
/// @version 920
///
/// @copyright (c) Etigues, 2010-2025
/// @copyright (c) Pervasive Displays Inc., 2021-2025
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

// SDK and configuration
#include "PDLS_Common.h"

#if (PDLS_COMMON_RELEASE < 920)
#error Required PDLS_COMMON_RELEASE 920
#endif // PDLS_COMMON_RELEASE

#ifndef EDITION_BASIC
#error Required Basic edition of PDLS_Common
#endif // EDITION_BASIC

#ifndef PDLS_BASIC_RELEASE
///
/// @brief Library release number
///
#define PDLS_BASIC_RELEASE 920

#include "Screen_EPD.h"

#endif // PDLS_EXT3_BASIC_RELEASE

