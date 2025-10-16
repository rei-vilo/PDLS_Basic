///
/// @file BWRY_Palette.ino
/// @brief Example for Pervasive Displays Library Suite - All editions
///
/// @details Example for Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @date 21 Jan 2025
/// @version 902
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

// SDK
// #include <Arduino.h>
#include "PDLS_Common.h"

// Include application, user and local libraries
// #include <SPI.h>

// Driver
#include "Pervasive_BWRY_Small.h"

// Screen
#include "PDLS_Basic.h"

#if (SCREEN_EPD_RELEASE < 902)
#error Required SCREEN_EPD_RELEASE 902
#endif // SCREEN_EPD_RELEASE

// Set parameters
#define DISPLAY_COLOURS_BWRY 0
#define DISPLAY_CONTRASTS_BWRY 1
#define DISPLAY_PALETTE_BWRY 1
#define DISPLAY_WHOAMI 0

// Define structures and classes

// Define variables and constants
// Driver
Pervasive_BWRY_Small myDriver(eScreen_EPD_154_QS_0F, boardRaspberryPiPico_RP2040);
// Pervasive_BWRY_Small myDriver(eScreen_EPD_213_QS_0F, boardRaspberryPiPico_RP2040);
// Pervasive_BWRY_Small myDriver(eScreen_EPD_266_QS_0F, boardRaspberryPiPico_RP2040);

// Screen
Screen_EPD myScreen(&myDriver);

uint8_t fontSmall, fontMedium, fontLarge, fontVery;

// Prototypes

// Utilities


// Functions
#if (DISPLAY_PALETTE_BWRY == 1)

void displayPaletteBWRY()
{
    // Grid = 5
    // .  .  .  Y  Y
    // .  .  R  O  R
    // .  W  lR lY W
    // B  G  dR dY B
    // B  W  R  Y

    // Grid = 4
    // .  .  .  Y
    // .  .  R  O
    // .  W  lR lY
    // B  G  dR dY

    const uint8_t grid = 4; // 4 or 5

    myScreen.setOrientation(ORIENTATION_PORTRAIT);
    myScreen.setPenSolid(false);

    uint16_t x0, y0, dx, dy, dz;

    dz = hV_HAL_min(myScreen.screenSizeX(), myScreen.screenSizeY()) / grid;
    x0 = (myScreen.screenSizeX() - dz * grid) / 2;
    y0 = (myScreen.screenSizeY() - dz * grid) / 2;

    myScreen.setPenSolid(true);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 0, y0 + dz * 4, dz, dz, myColours.black);
        myScreen.dRectangle(x0 + dz * 1, y0 + dz * 4, dz, dz, myColours.white);
        myScreen.dRectangle(x0 + dz * 2, y0 + dz * 4, dz, dz, myColours.red);
        myScreen.dRectangle(x0 + dz * 3, y0 + dz * 4, dz, dz, myColours.yellow);
    }
    myScreen.dRectangle(x0 + dz * 0, y0 + dz * 3, dz, dz, myColours.black);
    myScreen.dRectangle(x0 + dz * 1, y0 + dz * 3, dz, dz, myColours.grey);
    myScreen.dRectangle(x0 + dz * 2, y0 + dz * 3, dz, dz, myColours.darkRed);
    myScreen.dRectangle(x0 + dz * 3, y0 + dz * 3, dz, dz, myColours.darkYellow);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4, y0 + dz * 3, dz, dz, myColours.black);
    }

    myScreen.dRectangle(x0 + dz * 1, y0 + dz * 2, dz, dz, myColours.white);
    myScreen.dRectangle(x0 + dz * 2, y0 + dz * 2, dz, dz, myColours.lightRed);
    myScreen.dRectangle(x0 + dz * 3, y0 + dz * 2, dz, dz, myColours.lightYellow);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4, y0 + dz * 2, dz, dz, myColours.white);
    }

    myScreen.dRectangle(x0 + dz * 2, y0 + dz * 1, dz, dz, myColours.red);
    myScreen.dRectangle(x0 + dz * 3, y0 + dz * 1, dz, dz, myColours.orange);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4, y0 + dz * 1, dz, dz, myColours.red);
    }

    myScreen.dRectangle(x0 + dz * 3, y0 + dz * 0, dz, dz, myColours.yellow);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4, y0 + dz * 0, dz, dz, myColours.yellow);
    }

    myScreen.setPenSolid(false);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 0 + 2, y0 + dz * 4 + 2, dz - 4, dz - 4, myColours.white);
        myScreen.dRectangle(x0 + dz * 1 + 2, y0 + dz * 4 + 2, dz - 4, dz - 4, myColours.black);
        myScreen.dRectangle(x0 + dz * 2 + 2, y0 + dz * 4 + 2, dz - 4, dz - 4, myColours.white);
        myScreen.dRectangle(x0 + dz * 3 + 2, y0 + dz * 4 + 2, dz - 4, dz - 4, myColours.black);
    }
    myScreen.dRectangle(x0 + dz * 0 + 2, y0 + dz * 3 + 2, dz - 4, dz - 4, myColours.white);
    myScreen.dRectangle(x0 + dz * 1 + 2, y0 + dz * 3 + 2, dz - 4, dz - 4, myColours.white);
    myScreen.dRectangle(x0 + dz * 2 + 2, y0 + dz * 3 + 2, dz - 4, dz - 4, myColours.white);
    myScreen.dRectangle(x0 + dz * 3 + 2, y0 + dz * 3 + 2, dz - 4, dz - 4, myColours.white);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4 + 2, y0 + dz * 3 + 2, dz - 4, dz - 4, myColours.white);
    }

    myScreen.dRectangle(x0 + dz * 1 + 2, y0 + dz * 2 + 2, dz - 4, dz - 4, myColours.black);
    myScreen.dRectangle(x0 + dz * 2 + 2, y0 + dz * 2 + 2, dz - 4, dz - 4, myColours.black);
    myScreen.dRectangle(x0 + dz * 3 + 2, y0 + dz * 2 + 2, dz - 4, dz - 4, myColours.black);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4 + 2, y0 + dz * 2 + 2, dz - 4, dz - 4, myColours.black);
    }

    myScreen.dRectangle(x0 + dz * 2 + 2, y0 + dz * 1 + 2, dz - 4, dz - 4, myColours.white);
    myScreen.dRectangle(x0 + dz * 3 + 2, y0 + dz * 1 + 2, dz - 4, dz - 4, myColours.white);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4 + 2, y0 + dz * 1 + 2, dz - 4, dz - 4, myColours.white);
    }

    myScreen.dRectangle(x0 + dz * 3 + 2, y0 + dz * 0 + 2, dz - 4, dz - 4, myColours.black);
    if (grid > 4)
    {
        myScreen.dRectangle(x0 + dz * 4 + 2, y0 + dz * 0 + 2, dz - 4, dz - 4, myColours.black);
    }

    myScreen.setFontSolid(false);
    myScreen.selectFont(fontMedium);

    if (grid > 4)
    {
        myScreen.gText(x0 + dz * 0 + 4, y0 + dz * 4 + 4, "B", myColours.white);
        myScreen.gText(x0 + dz * 1 + 4, y0 + dz * 4 + 4, "W", myColours.black);
        myScreen.gText(x0 + dz * 2 + 4, y0 + dz * 4 + 4, "R", myColours.white);
        myScreen.gText(x0 + dz * 3 + 4, y0 + dz * 4 + 4, "Y", myColours.black);
    }
    myScreen.gText(x0 + dz * 0 + 4, y0 + dz * 3 + 4, "B", myColours.white);
    myScreen.gText(x0 + dz * 1 + 4, y0 + dz * 3 + 4, "G", myColours.white);
    myScreen.gText(x0 + dz * 2 + 4, y0 + dz * 3 + 4, "dR", myColours.white);
    myScreen.gText(x0 + dz * 3 + 4, y0 + dz * 3 + 4, "dY", myColours.white);
    if (grid > 4)
    {
        myScreen.gText(x0 + dz * 4 + 4, y0 + dz * 3 + 4, "B", myColours.white);
    }

    myScreen.gText(x0 + dz * 1 + 4, y0 + dz * 2 + 4, "W", myColours.black);
    myScreen.gText(x0 + dz * 2 + 4, y0 + dz * 2 + 4, "lR", myColours.black);
    myScreen.gText(x0 + dz * 3 + 4, y0 + dz * 2 + 4, "lY", myColours.black);
    if (grid > 4)
    {
        myScreen.gText(x0 + dz * 4 + 4, y0 + dz * 2 + 4, "W", myColours.black);
    }

    myScreen.gText(x0 + dz * 2 + 4, y0 + dz * 1 + 4, "R", myColours.white);
    myScreen.gText(x0 + dz * 3 + 4, y0 + dz * 1 + 4, "O", myColours.white);
    if (grid > 4)
    {
        myScreen.gText(x0 + dz * 4 + 4, y0 + dz * 1 + 4, "R", myColours.white);
    }

    myScreen.gText(x0 + dz * 3 + 4, y0 + dz * 0 + 4, "Y", myColours.black);
    if (grid > 4)
    {
        myScreen.gText(x0 + dz * 4 + 4, y0 + dz * 0 + 4, "Y", myColours.black);
    }

    myScreen.flush();
}

#endif // DISPLAY_PALETTE_BWRY

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
#if (DISPLAY_PALETTE_BWRY == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_PALETTE_BWRY");
    myScreen.clear();
    displayPaletteBWRY();
    hV_HAL_delayMilliseconds(8000);

#endif // DISPLAY_PALETTE_BWRY

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
