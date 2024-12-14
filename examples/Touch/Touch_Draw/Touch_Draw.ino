///
/// @file Touch_Draw.ino
/// @brief Example for Pervasive Displays Library Suite - All editions
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Nov 2024
/// @version 900
///
/// @copyright (c) Rei Vilo, 2010-2024
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

// SDK
// #include <Arduino.h>
#include "PDLS_Common.h"

// Include application, user and local libraries
// #include <Wire.h>
// #include <SPI.h>

// Driver
#include "Driver_EPD_Touch_Small.h"

// Screen
#include "PDLS_Basic.h"

#if (SCREEN_EPD_RELEASE < 900)
#error Required SCREEN_EPD_RELEASE 900
#endif // SCREEN_EPD_RELEASE

// Set parameters
#define DISPLAY_TOUCH 1

// Define structures and classes

// Define variables and constants
// Driver
// Driver_EPD myDriver(eScreen_EPD_370_PS_0C_Touch, boardRaspberryPiPico_RP2040);
Driver_EPD_Touch_Small myDriver(eScreen_EPD_271_KS_09_Touch, boardRaspberryPiPico_RP2040);

// Screen
Screen_EPD myScreen(&myDriver);

const uint8_t myOrientation = ORIENTATION_LANDSCAPE;
uint8_t fontExtra, fontLarge, fontMedium, fontSmall;

#ifndef WITH_TOUCH
#error Required WITH_TOUCH
#endif // WITH_TOUCH

#if (TOUCH_MODE == USE_TOUCH_NONE)
#error Required TOUCH_MODE USE_TOUCH_YES
#endif // TOUCH_MODE

// Prototypes

// Utilities
///
/// @brief Wait with countdown
/// @param second duration, s
///
void wait(uint8_t second)
{
    for (uint8_t i = second; i > 0; i--)
    {
        hV_HAL_Serial.print(formatString(" > %i  \r", i));
        hV_HAL_delayMilliseconds(1000);
    }
    hV_HAL_Serial.print("         \r");
}

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
    // hV_HAL_Serial = Serial by default, otherwise edit hV_HAL_Peripherals.h
    hV_HAL_begin(); // with Serial at 115200

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

    fontSmall = myScreen.addFont(Font_DejaVuSans12);
    fontSmall -= ((fontSmall > 0) ? 1 : 0);
    fontMedium = myScreen.addFont(Font_DejaVuSans16);
    fontMedium -= ((fontMedium > 0) ? 1 : 0);
    fontLarge = myScreen.addFont(Font_DejaVuSans24);
    fontLarge -= ((fontLarge > 0) ? 1 : 0);
    fontVery = myScreen.addFont(Font_DejaVuMono48);
    fontVery -= ((fontVery > 0) ? 1 : 0);

#endif // FONT_MODE

    // Example
#if (DISPLAY_TOUCH == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_TOUCH");
    myScreen.clear();
    displayTouch();
    wait(8);

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
