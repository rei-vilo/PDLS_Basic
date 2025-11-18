///
/// @file Common_Grid.ino
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
#define DISPLAY_GRID 1

// SDK and configuration
// #include <Arduino.h>
#include "PDLS_Common.h"

// Board
pins_t myBoard = boardRaspberryPiPico_RP2040;

// Driver
#include "Pervasive_Wide_Small.h"
Pervasive_Wide_Small myDriver(eScreen_EPD_271_KS_0C, boardRaspberryPiPico_RP2040);

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
#if (DISPLAY_GRID == 1)

///
/// @brief Example of grid
/// @param flagNew true = default = with GridXY, false = manual
///
void displayGrid(bool flagNew = true)
{
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);
    myScreen.clear();
    uint16_t ix = 5;
    uint16_t jy = 3;

    if (flagNew == false)
    {
        uint16_t x0 = 0;
        uint16_t y0 = 0;
        uint16_t x = myScreen.screenSizeX();
        uint16_t y = myScreen.screenSizeY();
        uint16_t dx = x / ix;
        uint16_t dy = y / jy;

        myScreen.selectFont(fontLarge);
        myScreen.gText(x0 + dx * 0, y0 + dy * 0, "Grid old");

        myScreen.selectFont(fontMedium);
        for (uint8_t i = 0; i < ix; i++)
        {
            for (uint8_t j = 1; j < jy; j++)
            {
                // myScreen.dRectangle(x0 + dx * i, y0 + dy * j, dx, dy, myColours.grey);
                myScreen.gText(x0 + dx * i + 4, y0 + dy * j + 4, formatString("%i.%i", i, j));
            }
        }
        myScreen.dRectangle(x0 + dx * 0, y0 + dy * 1, dx * (ix - 0), dy * (jy - 1), myColours.black);

        hV_HAL_log(LEVEL_INFO, "%s xy0 %i %i dxy %i %i", "OLD", x0, y0, x0 + dx * ix, y0 + dy * jy);
    }
    else
    {
        myScreen.setOrientation(ORIENTATION_LANDSCAPE);
        myScreen.clear();
        myScreen.selectFont(fontLarge);
        myScreen.gText(0, 0, "Grid XY");

        GridXY myXY;
        myXY.define(0, 0, myScreen.screenSizeX(), myScreen.screenSizeY(), ix, jy, true);

        myScreen.selectFont(fontLarge);
        myScreen.gText(myXY.x(0), myXY.y(0), "Grid XY");

        myScreen.selectFont(fontMedium);
        for (uint8_t i = 0; i < ix; i++)
        {
            for (uint8_t j = 1; j < jy; j++)
            {
                myScreen.dRectangle(myXY.x(i), myXY.y(j), myXY.dX(), myXY.dY(), myColours.grey);
                myScreen.gText(myXY.x(i) + 4, myXY.y(j) + 4, formatString("%i.%i", i, j));
            }
        }
        myScreen.dRectangle(myXY.x(0), myXY.y(1), myXY.dX(ix - 0), myXY.dY(jy - 1), myColours.black);
        hV_HAL_log(LEVEL_INFO, "%s xy0 %i %i dxy %i %i", "XY", myXY.x(0), myXY.y(0), myXY.dX(ix), myXY.dY(jy));
    }

    myScreen.flushFast();
}

#endif // DISPLAY_GRID

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
#if (DISPLAY_GRID == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_GRID");
    myScreen.clear();
    displayGrid(false);
    hV_HAL_delayMilliseconds(8000);
    displayGrid(true);
    hV_HAL_delayMilliseconds(8000);

#endif // DISPLAY_GRID

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

