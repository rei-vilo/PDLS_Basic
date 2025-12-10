///
/// @file Fast_Speed.ino
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
#define DISPLAY_FAST_TEMPERATURE 0
#define DISPLAY_FAST_SPEED 1

// SDK and configuration
// #include <Arduino.h>
#include "PDLS_Common.h"

// Board
Board_EXT myBoard = boardRaspberryPiPico_RP2040_EXT3;

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
#if (DISPLAY_FAST_SPEED == 1)

///
/// @brief Perform the speed test
///
void performTest()
{
    uint32_t chrono;

    myScreen.clear();
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);

    uint16_t x = myScreen.screenSizeX();
    uint16_t y = myScreen.screenSizeY();
    uint16_t dx = 0;
    uint16_t dy = 0;
    uint16_t dz = y / 2;
    String text = "";

    myScreen.selectFont(fontMedium);

    // 0
    dy = (dz - myScreen.characterSizeY()) / 2;
    hV_HAL_log(LEVEL_INFO, text.c_str());
    dx = (x - myScreen.stringSizeX(text)) / 2;
    myScreen.gText(dx, dy, text);
    myScreen.dRectangle(0, dz * 0, x, dz, myColours.black);

    chrono = hV_HAL_getMilliseconds();
    myScreen.flush();
    chrono = hV_HAL_getMilliseconds() - chrono;

    // 1
    dy += dz;
    // text = formatString("Normal update= %i ms", chrono);
    text = formatString("Fast update= %i ms", chrono);
    hV_HAL_log(LEVEL_INFO, text.c_str());
    dx = (x - myScreen.stringSizeX(text)) / 2;
    myScreen.gText(dx, dy, text);
    myScreen.dRectangle(0, dz * 1, x, dz, myColours.black);

    myScreen.flush();
}

#endif // DISPLAY_FAST_SPEED

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
#if (DISPLAY_FAST_SPEED == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_FAST_SPEED");
    myScreen.clear();
    performTest();
    hV_HAL_delayMilliseconds(8000);

#endif // DISPLAY_FAST_SPEED

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

