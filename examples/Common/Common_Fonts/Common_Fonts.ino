///
/// @file Common_Fonts.ino
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
#define DISPLAY_FONTS 1

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

#if (DISPLAY_FONTS == 1)

///
/// @brief Fonts test screen
/// @param flag true = default = perform flush, otherwise no
///
/// @image html T2_FONTS..jpg
/// @image latex T2_FONTS.PDF width=10cm
///
void displayFonts(bool flag = true)
{
    uint16_t x = 10;
    uint16_t y = 10;
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);

    myScreen.selectFont(fontLarge);

    myScreen.gText(x, y, myScreen.WhoAmI(), myColours.red);
    y += myScreen.characterSizeY();
    myScreen.gText(x, y, formatString("%i x %i", myScreen.screenSizeX(), myScreen.screenSizeY()), myColours.red);
    y += myScreen.characterSizeY();
    y += myScreen.characterSizeY();

    myScreen.selectFont(fontSmall);
    myScreen.gText(x, y, "Terminal6x8");
    y += myScreen.characterSizeY();

    myScreen.selectFont(fontMedium);
    myScreen.gText(x, y, "Terminal8x12");
    y += myScreen.characterSizeY();

    myScreen.selectFont(fontLarge);
    myScreen.gText(x, y, "Terminal12x16");
    y += myScreen.characterSizeY();

    myScreen.selectFont(fontVery);
    myScreen.gText(x, y, "Terminal16x24");
    y += myScreen.characterSizeY();

    // Extended
    myScreen.selectFont(fontMedium);

#if (STRING_MODE == USE_STRING_OBJECT)

    String text = utf2iso("éàîüç $£€¥ ¿?");

#elif (STRING_MODE == USE_CHAR_ARRAY)

    char text[24];
    strcpy(text, utf2iso("éàîüç $£€¥ ¿?"));

#endif // STRING_MODE

    uint16_t z = myScreen.stringSizeX(text);
    uint16_t t = myScreen.characterSizeY();
    myScreen.gText(myScreen.screenSizeX() - z, myScreen.screenSizeY() - t, text);
    y += myScreen.characterSizeY();

    myScreen.flush();
}

#endif // DISPLAY_FONTS

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
#if (DISPLAY_FONTS == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_FONTS");
    myScreen.clear();
    displayFonts();
    wait(8);

#endif // DISPLAY_FONTS

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
