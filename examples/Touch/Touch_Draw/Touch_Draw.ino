///
/// @file Touch_Draw.ino
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
#define DISPLAY_TOUCH 1

// SDK and configuration
// #include <Arduino.h>
#include "PDLS_Common.h"

// Board
Board_EXT myBoard = boardRaspberryPiPico_RP2040_EXT3;

// Driver
#include "Pervasive_Touch_Small.h"
// Pervasive_Touch_Small myDriver(eScreen_EPD_370_PS_0C_Touch, myBoard);
Pervasive_Touch_Small myDriver(eScreen_EPD_271_KS_09_Touch, myBoard);

// Screen
#include "PDLS_Basic.h"
Screen_EPD myScreen(&myDriver);

// Checks
#if (SCREEN_EPD_RELEASE < 1000)
#error Required SCREEN_EPD_RELEASE 1000
#endif // SCREEN_EPD_RELEASE

// Fonts
uint8_t fontSmall, fontMedium, fontLarge, fontVery;

const uint8_t myOrientation = ORIENTATION_LANDSCAPE;

// Checks
#ifndef WITH_TOUCH
#error Required WITH_TOUCH
#endif // WITH_TOUCH

// Prototypes

// Utilities

// Functions
#if (DISPLAY_TOUCH == 1)
///
/// @brief Minimal test screen
/// @param flag true=default=perform flush, otherwise no
///
void displayTouch(bool flag = true)
{
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);
    myScreen.selectFont(fontMedium);

    myScreen.clear();
    myScreen.gText(10, 10, "Draw!");

    myScreen.flush();

    touch_t touch;
    uint16_t k = 32; // 64; // 256;

    bool flagLine = false;
    uint16_t x0, y0;

    myScreen.setPenSolid(true);
    while (k > 0)
    {
        if (myScreen.getTouch(touch))
        {
            // hV_HAL_log(LEVEL_DEBUG, "%3i: xyzt= %4i %4i %4i %4i %8i", k, touch.x, touch.y, touch.z, touch.t, hV_HAL_getMilliseconds());
            if ((touch.t == TOUCH_EVENT_RELEASE) or (touch.t == TOUCH_EVENT_MOVE))
            {
                myScreen.circle(touch.x, touch.y, 4, myColours.black);
                myScreen.flushFast();

                hV_HAL_log(LEVEL_DEBUG, "%i left", k);
                k--;
            }

            /*
            if (touch.t == TOUCH_EVENT_PRESS)
            {
                flagLine = false;
            }

            if (flagLine)
            {
                myScreen.line(x0, y0, touch.x, touch.y, myColours.black);
                myScreen.flushFast();
            }

            x0 = touch.x;
            y0 = touch.y;
            flagLine = true;
            k--;
            */
        }

        hV_HAL_delayMilliseconds(10);
    }
}
#endif // DISPLAY_TOUCH

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

    myScreen.regenerate();

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
#if (DISPLAY_TOUCH == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_TOUCH");
    myScreen.clear();
    displayTouch();
    hV_HAL_delayMilliseconds(8000);

#endif // DISPLAY_TOUCH

    // Against possible ghosting
    myScreen.regenerate();

    hV_HAL_exit(0);
}

///
/// @brief Loop, empty
///
void loop()
{
    hV_HAL_delayMilliseconds(1000);
}

