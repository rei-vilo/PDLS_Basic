///
/// @file Fast_Temperature.ino
/// @brief Example for Pervasive Displays Library Suite - All editions
///
/// @details Example for Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @date 21 Nov 2025
/// @version 1000
///
/// @copyright (c) Pervasive Displays Inc., 2021-2025
/// @copyright All rights reserved
/// @copyright For exclusive use with Pervasive Displays screens
///
/// * Basic edition: for hobbyists and for basic usage
/// @n Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
///
/// * Evaluation edition: for professionals or organisations, no commercial usage
/// @n All rights reserved
///
/// * Commercial edition: for professionals or organisations, commercial usage
/// @n All rights reserved
///

// Set parameters
#define DISPLAY_FAST_ORIENTATION 0
#define DISPLAY_FAST_LINE 0
#define DISPLAY_FAST_TEMPERATURE 1
#define DISPLAY_FAST_SPEED 0

// SDK and configuration
// #include <Arduino.h>
#include "PDLS_Common.h"

// Board
pins_t myBoard = boardRaspberryPiPico_RP2040;

// Driver
#include "Pervasive_Wide_Small.h"
Pervasive_Wide_Small myDriver(eScreen_EPD_290_KS_0F, myBoard);

// Screen
#include "PDLS_Basic.h"
Screen_EPD myScreen(&myDriver);

// Checks
#if (SCREEN_EPD_RELEASE < 1000)
#error Required SCREEN_EPD_RELEASE 1000
#endif // SCREEN_EPD_RELEASE

// Fonts
uint8_t fontSmall, fontMedium, fontLarge, fontVery;

// Prototypes

// Utilities

// Functions
#if (DISPLAY_FAST_TEMPERATURE == 1)

void check(int8_t temperatureC, uint8_t expectedMode)
{
    const char * stringMode[] = { "NONE", "NORMAL", "FAST", "PARTIAL" };

    myScreen.setTemperatureC(temperatureC);
    uint8_t recommendedMode = myScreen.checkTemperatureMode(expectedMode);
    hV_HAL_log(LEVEL_INFO, "Temperature= %+4i C - Mode: %8s -> %-8s", temperatureC, stringMode[expectedMode], stringMode[recommendedMode]);
}

void performTest()
{
    check(+70, UPDATE_FAST);
    check(+60, UPDATE_FAST);
    check(+50, UPDATE_FAST);
    check(+25, UPDATE_FAST);
    check(-15, UPDATE_FAST);
    check(-25, UPDATE_FAST);
}

#endif // DISPLAY_FAST_TEMPERATURE

///
/// @brief Setup
///
void setup()
{
    hV_HAL_begin();

    hV_HAL_Serial_crlf();
    hV_HAL_log(LEVEL_INFO, __FILE__);
    hV_HAL_log(LEVEL_INFO, __DATE__ " " __TIME__);
    hV_HAL_Serial_crlf();

    // Screen
    myScreen.begin();

    // Fonts
#if (FONT_MODE == USE_FONT_TERMINAL)

    fontSmall = Font_Terminal6x8;
    fontMedium = Font_Terminal8x12;
    fontLarge = Font_Terminal12x16;
    fontVery = Font_Terminal16x24;

#else // FONT_MODE

    fontSmall = myScreen.addFont(Font_Latin_DejaVuSans12);
    fontSmall -= ((fontSmall > 0) ? 1 : 0);
    fontMedium = myScreen.addFont(Font_Latin_DejaVuSans16);
    fontMedium -= ((fontMedium > 0) ? 1 : 0);
    fontLarge = myScreen.addFont(Font_Latin_DejaVuSans24);
    fontLarge -= ((fontLarge > 0) ? 1 : 0);
    fontVery = myScreen.addFont(Font_Latin_DejaVuMono48);
    fontVery -= ((fontVery > 0) ? 1 : 0);

#endif // FONT_MODE

    // Example
#if (DISPLAY_FAST_TEMPERATURE == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_FAST_TEMPERATURE");
    performTest();

#endif // DISPLAY_FAST_TEMPERATURE

    hV_HAL_log(LEVEL_INFO, "Regenerate");
    myScreen.regenerate();

    hV_HAL_exit();
}

///
/// @brief Loop, empty
///
void loop()
{
    hV_HAL_delayMilliseconds(1000);
}

