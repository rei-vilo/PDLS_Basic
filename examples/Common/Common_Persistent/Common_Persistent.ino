///
/// @file Common_Persistent.ino
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
#define DISPLAY_PERSISTENT 1

// SDK and configuration
// #include <Arduino.h>
#include "PDLS_Common.h"

// Board
pins_t myBoard = boardRaspberryPiPico_RP2040;

// Driver
#include "Pervasive_Wide_Small.h"
Pervasive_Wide_Small myDriver(eScreen_EPD_271_KS_09, boardRaspberryPiPico_RP2040);

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
#if (DISPLAY_PERSISTENT == 1)

///
/// @brief Who am I? test screen
///
void displayPersistent()
{
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);
    uint16_t x = 4;
    uint16_t y = 4;

    myScreen.selectFont(fontLarge);
    myScreen.gText(x, y, "Who Am I");

    uint16_t dy = myScreen.characterSizeY();
    y += dy;
    myScreen.gText(x, myScreen.screenSizeY() - dy, "Unplug when the LED is on");

    myScreen.selectFont(fontMedium);
    dy = myScreen.characterSizeY();

#if (STRING_MODE == USE_STRING_OBJECT)

    // formatString() requires char * as input
    myScreen.gText(x, y, formatString("%8s %s", "Screen", myScreen.WhoAmI().c_str()));
    y += dy;
    myScreen.gText(x, y, formatString("%8s %ix%i", "Size", myScreen.screenSizeX(), myScreen.screenSizeY()));
    y += dy;
    String text = myScreen.screenNumber();
    myScreen.gText(x, y, formatString("%8s %s", "Number", text.c_str()));
    y += dy;
    myScreen.gText(x, y, formatString("%8s %s", "PDLS", myScreen.reference().c_str()));
    y += dy;

#elif (STRING_MODE == USE_CHAR_ARRAY)

    // formatString() requires char * as input
    myScreen.gText(x, y, formatString("%8s %s", "Screen", myScreen.WhoAmI()));
    y += dy;
    myScreen.gText(x, y, formatString("%8s %ix%i", "Size", myScreen.screenSizeX(), myScreen.screenSizeY()));
    y += dy;
    myScreen.gText(x, y, formatString("%8s %s", "Number", myScreen.screenNumber()));
    y += dy;
    myScreen.gText(x, y, formatString("%8s %s", "Driver", myDriver.reference()));
    y += dy;
    myScreen.gText(x, y, formatString("%8s %s", "PDLS", myScreen.reference()));
    y += dy;

#endif // STRING_MODE

    myScreen.gText(x, y, formatString("%8s", "Colours"));
    x += 9 * myScreen.characterSizeX();

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

#endif // DISPLAY_PERSISTENT

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
#if (DISPLAY_PERSISTENT == 1)

    hV_HAL_GPIO_define(LED_BUILTIN, OUTPUT);
    for (uint8_t i = 1; i < 7; i += 1)
    {
        hV_HAL_GPIO_write(LED_BUILTIN, i % 2);
        hV_HAL_delayMilliseconds(250);
    }

    hV_HAL_log(LEVEL_INFO, "DISPLAY_PERSISTENT");
    myScreen.clear();
    displayPersistent();

    hV_HAL_GPIO_write(LED_BUILTIN, HIGH);
    hV_HAL_delayMilliseconds(8000);

#endif // DISPLAY_PERSISTENT

    // Uncomment to clear the screen
    // hV_HAL_log(LEVEL_INFO, "Regenerate");
    // myScreen.regenerate();

    hV_HAL_exit();
}

///
/// @brief Loop, empty
///
void loop()
{
    hV_HAL_delayMilliseconds(1000);
}

