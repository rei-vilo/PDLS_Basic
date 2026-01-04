///
/// @file BWRY_Contrasts.ino
/// @brief Example for Pervasive Displays Library Suite - All editions
///
/// @details Example for Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @date 21 Nov 2025
/// @version 1000
///
/// @copyright (c) Pervasive Displays Inc., 2021-2026
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
#define DISPLAY_COLOURS_BWRY 0
#define DISPLAY_CONTRASTS_BWRY 1
#define DISPLAY_PALETTE_BWRY 0
#define DISPLAY_WHOAMI 0

// SDK and configuration
// #include <Arduino.h>
#include "PDLS_Common.h"

// Board
Board_EXT myBoard = boardRaspberryPiPico_RP2040_EXT3;

// Driver
#include "Pervasive_BWRY_Small.h"
Pervasive_BWRY_Small myDriver(eScreen_EPD_154_QS_0F, myBoard);
// Pervasive_BWRY_Small myDriver(eScreen_EPD_213_QS_0F, myBoard);
// Pervasive_BWRY_Small myDriver(eScreen_EPD_266_QS_0F, myBoard);

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
#if (DISPLAY_CONTRASTS_BWRY == 1)

///
/// @brief Palette test screen
///
void displayContrastsBWRY()
{
    const uint8_t grid = 4; // 4 or 5
    const uint16_t colours[4] = {myColours.black, myColours.white, myColours.red, myColours.yellow};
    const char label[4] = {'B', 'W', 'R', 'Y'};

    myScreen.setOrientation(ORIENTATION_PORTRAIT);
    myScreen.setPenSolid(false);

    uint16_t x0, y0, dx, dy, dz;

    dz = hV_HAL_min(myScreen.screenSizeX(), myScreen.screenSizeY()) / grid;
    x0 = (myScreen.screenSizeX() - dz * grid) / 2;
    y0 = (myScreen.screenSizeY() - dz * grid) / 2;

    myScreen.setPenSolid(true);
    myScreen.setFontSolid(false);
    myScreen.selectFont(fontMedium);
    dx = (dz - myScreen.characterSizeX()) / 2;
    dy = (dz - myScreen.characterSizeY()) / 2;

    for (uint8_t i = 0; i < grid; i++)
    {
        for (uint8_t j = 0; j < grid; j++)
        {
            myScreen.dRectangle(x0 + dz * i, y0 + dz * j, dz, dz, colours[i]);

#if (STRING_MODE == USE_STRING_OBJECT)

            myScreen.gText(x0 + dz * i + dx, y0 + dz * j + dy, String(label[j]), colours[j]);

#elif (STRING_MODE == USE_CHAR_ARRAY)

            myScreen.gText(x0 + dz * i + dx, y0 + dz * j + dy, label[j], colours[j]);

#endif // STRING_MODE
        }
    }

    myScreen.flush();
}

#endif // DISPLAY_CONTRASTS_BWRY

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
#if (DISPLAY_CONTRASTS_BWRY == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_CONTRASTS_BWRY");
    myScreen.clear();
    displayContrastsBWRY();
    hV_HAL_delayMilliseconds(8000);

#endif // DISPLAY_CONTRASTS_BWRY

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

