///
/// @file Common_Grid.ino
/// @brief Example for Pervasive Displays Library Suite - All editions
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 May 2025
/// @version 909
///
/// @copyright (c) Rei Vilo, 2010-2025
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

// SDK and configurations
// #include <Arduino.h>
#include "PDLS_Common.h"

// Include application, user and local libraries
// #include <SPI.h>

// Driver
#include "Pervasive_Wide_Small.h"

// Screen
#include "PDLS_Basic.h"

#if (SCREEN_EPD_RELEASE < 902)
#error Required SCREEN_EPD_RELEASE 902
#endif // SCREEN_EPD_RELEASE

// Set parameters
#define DISPLAY_GRID 1

// Define structures and classes

// Define variables and constants
// Driver
Pervasive_Wide_Small myDriver(eScreen_EPD_271_KS_09, boardRaspberryPiPico_RP2040);

// Screen
Screen_EPD myScreen(&myDriver);

uint8_t fontSmall, fontMedium, fontLarge, fontVery;

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

        hV_HAL_log(LEVEL_INFO, formatString("%s xy0 %i %i dxy %i %i", "OLD", x0, y0, x0 + dx * ix, y0 + dy * jy));
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
        hV_HAL_log(LEVEL_INFO, formatString("%s xy0 %i %i dxy %i %i", "XY", myXY.x(0), myXY.y(0), myXY.dX(ix), myXY.dY(jy)));
    }

    myScreen.flushFast();
}

#endif // DISPLAY_GRID

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

    // Start
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
    wait(8);
    displayGrid(true);
    wait(8);

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
