///
/// @file Common_Forms.ino
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
#include "Pervasive_Wide_Small.h"

// Screen
#include "PDLS_Basic.h"

#if (SCREEN_EPD_RELEASE < 902)
#error Required SCREEN_EPD_RELEASE 902
#endif // SCREEN_EPD_RELEASE

// Set parameters
#define DISPLAY_FORMS 1

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

#if (DISPLAY_FORMS == 1)

///
/// @brief Basic forms test screen
/// @param flag true = default = perform flush, otherwise no
///
/// @image html T2_FORMS.jpg
/// @image latex T2_FORMS.PDF width=10cm
///
void displayForms(bool flag = true)
{
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);

    uint16_t x = myScreen.screenSizeX();
    uint16_t y = myScreen.screenSizeY();
    uint16_t z = hV_HAL_min(x, y);

    myScreen.setPenSolid(false);
    myScreen.dRectangle(0, 0, x, y, myColours.black);
    myScreen.dLine(0, 0, x, y, myColours.red);

    myScreen.setPenSolid(true);
    myScreen.circle(x / 3, y / 3, z / 4, myColours.grey);
    // myScreen.triangle(x*2/3, y/3, x/2, y*2/3, x*3/4, y*2/3-10, myColours.black);
    myScreen.triangle(x * 2 / 3, y / 3, x * 3 / 4, y * 2 / 3 - 10, x - 10, 10, myColours.black);
    myScreen.dRectangle(x / 3, y * 2 / 3, x / 3, y / 4, myColours.red);

    myScreen.flush();
}

#endif // DISPLAY_FORMS

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
#if (DISPLAY_FORMS == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_FORMS");
    myScreen.clear();
    displayForms();
    wait(8);

#endif // DISPLAY_FORMS

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
