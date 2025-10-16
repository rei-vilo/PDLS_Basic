///
/// @file EXT4_LargeFont.ino
/// @brief Example for Pervasive Displays Library Suite - Basic edition
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @date 21 Jan 2025
/// @version 902
///
/// @copyright (c) Etigues, 2010-2025
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

// SDK and configuration
// #include <Arduino.h>
#include "PDLS_Common.h"

// Driver
#include "Pervasive_Wide_Small.h"

// Screen
#include "PDLS_Basic.h"

#warning PDLS_Basic only

#if (SCREEN_EPD_RELEASE < 902)
#error Required SCREEN_EPD_RELEASE 902
#endif // SCREEN_EPD_RELEASE

// Include application, user and local libraries
// #include <SPI.h>

// Set parameters
#define DISPLAY_MULTIPLIER 1

// Define structures and classes

// Define variables and constants
pins_t myBoard = boardArduinoNanoMatter;
// pins_t myBoard = boardSiLabsBG24Explorer;
Pervasive_Wide_Small myDriver(eScreen_EPD_290_KS_0F, myBoard);
Screen_EPD myScreen(&myDriver);

uint8_t fontSmall, fontMedium, fontLarge, fontVery;

// Prototypes

// Utilities

// Functions

#if (DISPLAY_MULTIPLIER == 1)

void displayMultiplier()
{
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);
    uint16_t x = 0;
    uint16_t y = 0;

    String text = "abAB12";
    uint8_t length = text.length();

    myScreen.selectFont(Font_Terminal16x24);
    myScreen.gText(x, y, text);
    x += myScreen.characterSizeX() * length;
    myScreen.gTextLarge(x, y, text);
    y += myScreen.characterSizeY() * 2;
    x = 0;

    myScreen.selectFont(Font_Terminal12x16);
    myScreen.gText(x, y, text);
    x += myScreen.characterSizeX() * length;
    myScreen.gTextLarge(x, y, text);
    y += myScreen.characterSizeY() * 2;
    x = 0;

    myScreen.selectFont(Font_Terminal8x12);
    myScreen.gText(x, y, text);
    x += myScreen.characterSizeX() * length;
    myScreen.gTextLarge(x, y, text);
    y += myScreen.characterSizeY() * 2;
    x = 0;

    myScreen.selectFont(Font_Terminal6x8);
    myScreen.gText(x, y, text);
    x += myScreen.characterSizeX() * length;
    myScreen.gTextLarge(x, y, text);
    y += myScreen.characterSizeY() * 2;
    x = 0;

    myScreen.flush();
    hV_HAL_delayMilliseconds(8);
}

#endif // DISPLAY_MULTIPLIER

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
#error This example requires PDLS_Basic.
#endif // FONT_MODE

    // Example
#if (DISPLAY_MULTIPLIER == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_MULTIPLIER");
    myScreen.clear();
    displayMultiplier();
    hV_HAL_delayMilliseconds(8000);

#endif  // DISPLAY_MULTIPLIER

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

