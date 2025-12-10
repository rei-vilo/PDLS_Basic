///
/// @file EXT4_ScreenSizes.ino
/// @brief Example for EXT4 extension board sensors and actuators - All editions
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
#define DISPLAY_SIZES 1

// SDK and configuration
// #include <Arduino.h>
#include "PDLS_Common.h"

// Board
Board_EXT myBoard = boardArduinoNanoMatter_EXT4;
// Board_EXT myBoard = boardSiLabsBG24Explorer_EXT4;

// Driver
#include "Pervasive_Wide_Small.h"
// Pervasive_Wide_Small myDriver(eScreen_EPD_152_KS_0J, myBoard);
// Pervasive_Wide_Small myDriver(eScreen_EPD_206_KS_0E, myBoard);
// Pervasive_Wide_Small myDriver(eScreen_EPD_213_KS_0E, myBoard);
// Pervasive_Wide_Small myDriver(eScreen_EPD_266_KS_0C, myBoard);
// Pervasive_Wide_Small myDriver(eScreen_EPD_271_KS_0C, myBoard);
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
#if (DISPLAY_SIZES == 1)

///
/// @brief Displays x.y size
///
void displayWhoAmI()
{
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);
    myScreen.selectFont(fontMedium);

    uint16_t x = 4;
    uint16_t y = 4;
    uint16_t dy = myScreen.characterSizeY();

    myScreen.gText(x, y, formatString("x %i", myScreen.screenSizeX()));
    y += dy;
    myScreen.gText(x, y, formatString("y %i", myScreen.screenSizeY()));
    y += dy;

    myScreen.setPenSolid(true);
    myScreen.dRectangle(x + dy * 0, y, dy - 1, dy - 1, myColours.black);
    myScreen.setPenSolid(false);
    myScreen.dRectangle(x + dy * 1, y, dy - 1, dy - 1, myColours.black);

    myScreen.selectFont(fontSmall);

#if (STRING_MODE == USE_STRING_OBJECT)

    String text = myScreen.screenNumber();
    x = myScreen.screenSizeX() - myScreen.stringSizeX(text) - 4;
    y = myScreen.screenSizeY() - myScreen.characterSizeY() - 4;
    myScreen.gText(x, y, text);

#elif (STRING_MODE == USE_CHAR_ARRAY)

    char text[64];
    strcpy(text, myScreen.screenNumber().c_str());
    x = myScreen.screenSizeX() - myScreen.stringSizeX(text) - 4;
    y = myScreen.screenSizeY() - myScreen.characterSizeY() - 4;
    myScreen.gText(x, y, text);

#endif // STRING_MODE

    myScreen.flush();
}

#endif // DISPLAY_SIZES

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

    // Check EXT4
    if (myBoard.scope != BOARD_EXT4)
    {
        hV_HAL_log(LEVEL_CRITICAL, "EXT4 board required");
        hV_HAL_exit(RESULT_ERROR);
    }

    // Check panelPower
    if (myBoard.panelPower == NOT_CONNECTED)
    {
        hV_HAL_log(LEVEL_INFO, "panelPower not connected");
        hV_HAL_exit(0x01);
    }

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
#if (DISPLAY_SIZES == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_SIZES");
    myScreen.clear();
    displayWhoAmI();
    hV_HAL_delayMilliseconds(8000);

#endif // DISPLAY_SIZES

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

