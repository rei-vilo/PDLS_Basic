///
/// @file Common_Colours.ino
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
#define DISPLAY_COLOURS 1

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
#if (DISPLAY_COLOURS == 1)

///
/// @brief Palette test screen
/// @param flag true = default = perform flush, otherwise no
///
/// @image html T2_PALET.jpg
/// @image latex T2_PALET.PDF width=10cm
///
void displayPalette(bool flag = true)
{
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);

    uint16_t x0, y0, dx, dy, maxSize, colour;
    maxSize = hV_HAL_min((myScreen.screenSizeX() * 10 / 50), (myScreen.screenSizeY() * 10 / 35));
    dx = (myScreen.screenSizeX() - maxSize * 50 / 10) / 2;
    dy = (myScreen.screenSizeY() - maxSize * 35 / 10) / 2;

    y0 = dy + maxSize / 2;
    x0 = dx + maxSize / 2;
    myScreen.dRectangle(x0 - 2, y0 - 2, maxSize + 4, maxSize + 4, myColours.red);
    x0 = dx + maxSize * 4 / 2;
    myScreen.dRectangle(x0 - 2, y0 - 2, maxSize + 4, maxSize + 4, myColours.red);
    x0 = dx + maxSize * 7 / 2;
    myScreen.dRectangle(x0 - 2, y0 - 2, maxSize + 4, maxSize + 4, myColours.red);

    y0 = dy + maxSize * 4 / 2;
    x0 = dx + maxSize / 2;
    myScreen.dRectangle(x0 - 2, y0 - 2, maxSize + 4, maxSize + 4, myColours.black);
    x0 = dx + maxSize * 4 / 2;
    myScreen.dRectangle(x0 - 2, y0 - 2, maxSize + 4, maxSize + 4, myColours.black);
    x0 = dx + maxSize * 7 / 2;
    myScreen.dRectangle(x0 - 2, y0 - 2, maxSize + 4, maxSize + 4, myColours.black);

    myScreen.setPenSolid();
    myScreen.selectFont(fontSmall);

    x0 = dx + maxSize / 2;
    y0 = dy + maxSize / 2;
    // Light red
    myScreen.dRectangle(x0, y0, maxSize, maxSize, myColours.lightRed);
    myScreen.gText(x0, y0 + maxSize + 6, "light", myColours.red);

    x0 = dx + maxSize * 4 / 2;
    y0 = dy + maxSize / 2;
    // Red
    myScreen.dRectangle(x0, y0, maxSize, maxSize, myColours.red);
    myScreen.gText(x0, y0 + maxSize + 6, "red", myColours.red);

    x0 = dx + maxSize * 7 / 2;
    y0 = dy + maxSize / 2;
    // Dark-red
    myScreen.dRectangle(x0, y0, maxSize, maxSize, myColours.darkRed);
    myScreen.gText(x0, y0 + maxSize + 6, "dark", myColours.red);

    x0 = dx + maxSize / 2;
    y0 = dy + maxSize * 4 / 2;
    // White
    myScreen.dRectangle(x0, y0, maxSize, maxSize, myColours.white);
    myScreen.gText(x0, y0 + maxSize + 6, "white", myColours.black);

    x0 = dx + maxSize * 4 / 2;
    y0 = dy + maxSize * 4 / 2;
    // Grey
    myScreen.dRectangle(x0, y0, maxSize, maxSize, myColours.grey);
    myScreen.gText(x0, y0 + maxSize + 6, "grey", myColours.black);

    x0 = dx + maxSize * 7 / 2;
    y0 = dy + maxSize * 4 / 2;
    // Black
    myScreen.dRectangle(x0, y0, maxSize, maxSize, myColours.black);
    myScreen.gText(x0, y0 + maxSize + 6, "black", myColours.black);

    myScreen.selectFont(fontMedium);
    myScreen.gText(0, 0, myScreen.WhoAmI(), myColours.black);

    myScreen.flush();
}

#endif // DISPLAY_COLOURS

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
#if (DISPLAY_COLOURS == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_COLOURS");
    myScreen.clear();
    displayPalette();
    hV_HAL_delayMilliseconds(8000);

#endif // DISPLAY_COLOURS

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

