///
/// @file BWRY_WhoAmI.ino
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
#define DISPLAY_WHOAMI 1

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
#if (DISPLAY_WHOAMI == 1)

///
/// @brief Who am i? test screen
///
/// @image html T2_WHOAMI.jpg
/// @image latex T2_WHOAMI.PDF width=10cm
///
void displayWhoAmI()
{
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);
    myScreen.selectFont(fontMedium);

    uint16_t x = 4;
    uint16_t y = 4;
    uint16_t dy = myScreen.characterSizeY();

    myScreen.gText(x, y, "Who Am I");
    myScreen.gText(x + 1, y, "Who Am I");
    y += dy;

#if (STRING_MODE == USE_STRING_OBJECT)

    // formatString() requires char * as input
    myScreen.gText(x, y, formatString("Screen %s", myScreen.WhoAmI().c_str()));
    y += dy;
    myScreen.gText(x, y, formatString("Size %ix%i", myScreen.screenSizeX(), myScreen.screenSizeY()));
    y += dy;
    myScreen.gText(x, y, formatString("Number %s", myScreen.screenNumber().c_str()));
    y += dy;
    myScreen.gText(x, y, formatString("Driver %s", myDriver.reference().c_str()));
    y += dy;
    myScreen.gText(x, y, formatString("PDLS %s", myScreen.reference().c_str()));
    y += dy;

#elif (STRING_MODE == USE_CHAR_ARRAY)

    // formatString() requires char * as input
    myScreen.gText(x, y, formatString("Screen %s", myScreen.WhoAmI()));
    y += dy;
    myScreen.gText(x, y, formatString("Size %ix%i", myScreen.screenSizeX(), myScreen.screenSizeY()));
    y += dy;
    myScreen.gText(x, y, formatString("Number %s", myScreen.screenNumber()));
    y += dy;
    myScreen.gText(x, y, formatString("Driver %s", myDriver.reference()));
    y += dy;
    myScreen.gText(x, y, formatString("PDLS %s", myScreen.reference()));
    y += dy;

#endif // STRING_MODE

    char * name = "Colours ";
    myScreen.gText(x, y, name);
    x += myScreen.stringSizeX(name);

    myScreen.setPenSolid(true);
    myScreen.dRectangle(x + dy * 0, y, dy - 1, dy - 1, myColours.black);
    myScreen.setPenSolid(false);
    myScreen.dRectangle(x + dy * 1, y, dy - 1, dy - 1, myColours.black);
    myScreen.setPenSolid(true);

    uint8_t number = myScreen.screenColours();

    if (number >= 3)
    {
        myScreen.dRectangle(x + dy * 2, y, dy - 1, dy - 1, myColours.red);

#if defined(WITH_COLOURS_BWRY)
        if (number == 4)
        {
            myScreen.dRectangle(x + dy * 3, y, dy - 1, dy - 1, myColours.yellow);
        }
#endif // WITH_COLOURS_BWRY
    }

    myScreen.flush();
}

#endif // DISPLAY_WHOAMI

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
#if (DISPLAY_WHOAMI == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_WHOAMI");
    myScreen.clear();
    displayWhoAmI();
    wait(8);

#endif // DISPLAY_WHOAMI

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
