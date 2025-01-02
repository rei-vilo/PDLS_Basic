///
/// @file BWRY_Colours.ino
/// @brief Example for Pervasive Displays Library Suite - All editions
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Jan 2025
/// @version 902
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

// SDK
// #include <Arduino.h>
#include "PDLS_Common.h"

// Include application, user and local libraries
// #include <SPI.h>

// Driver
#include "Driver_EPD_BWRY_Small.h"

// Screen
#include "PDLS_Basic.h"

#if (SCREEN_EPD_RELEASE < 902)
#error Required SCREEN_EPD_RELEASE 902
#endif // SCREEN_EPD_RELEASE

// Set parameters
#define DISPLAY_COLOURS_BWRY 1
#define DISPLAY_CONTRASTS_BWRY 0
#define DISPLAY_PALETTE_BWRY 0
#define DISPLAY_WHOAMI 0

// Define structures and classes

// Define variables and constants
// Driver
Driver_EPD_BWRY_Small myDriver(eScreen_EPD_154_QS_0F, boardRaspberryPiPico_RP2040);
// Driver_EPD_BWRY_Small myDriver(eScreen_EPD_213_QS_0F, boardRaspberryPiPico_RP2040);
// Driver_EPD_BWRY_Small myDriver(eScreen_EPD_266_QS_0F, boardRaspberryPiPico_RP2040);

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
#if (DISPLAY_COLOURS_BWRY == 1)

void displayColoursBWRY()
{
    myScreen.setOrientation(ORIENTATION_PORTRAIT);
    myScreen.setPenSolid(false);
    myScreen.selectFont(fontMedium);

    uint16_t x, y, dx, dy;

    const uint8_t grid = 4; // 4 or 5

    x = myScreen.screenSizeX();
    y = myScreen.screenSizeY();
    dx = x / 1;
    dy = y / 4;

    myScreen.setPenSolid(true);
    myScreen.dRectangle(dx * 0, dy * 0, dx, dy, myColours.black);
    myScreen.dRectangle(dx * 0, dy * 1, dx, dy, myColours.white);
    myScreen.dRectangle(dx * 0, dy * 2, dx, dy, myColours.red);
    myScreen.dRectangle(dx * 0, dy * 3, dx, dy, myColours.yellow);

    myScreen.setFontSolid(false);
    myScreen.gText(dx * 0 + 4, dy * 0 + 4, "Black", myColours.white);
    myScreen.gText(dx * 0 + 4, dy * 1 + 4, "White", myColours.black);
    myScreen.gText(dx * 0 + 4, dy * 2 + 4, "Red", myColours.white);
    myScreen.gText(dx * 0 + 4, dy * 3 + 4, "Yellow", myColours.black);

    myScreen.flush();
}

#endif // DISPLAY_COLOURS_BWRY

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
#if (DISPLAY_COLOURS_BWRY == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_COLOURS_BWRY");
    myScreen.clear();
    displayColoursBWRY();
    wait(8);

#endif // DISPLAY_COLOURS_BWRY

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
