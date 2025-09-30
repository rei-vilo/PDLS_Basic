///
/// @file Fast_Line.ino
/// @brief Example for Pervasive Displays Library Suite - All editions
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
#define DISPLAY_FAST_ORIENTATION 0
#define DISPLAY_FAST_LINE 1
#define DISPLAY_FAST_TEMPERATURE 0
#define DISPLAY_FAST_SPEED 0

// Define structures and classes

// Define variables and constants
// Driver
Pervasive_Wide_Small myDriver(eScreen_EPD_290_KS_0F, boardRaspberryPiPico_RP2040);

// Screen
Screen_EPD myScreen(&myDriver);

uint8_t fontSmall, fontMedium, fontLarge, fontVery;

// Prototypes

// Utilities


// Functions
#if (DISPLAY_FAST_LINE == 1)

///
/// @brief Flush screen and display time
///
void flush_ms()
{
    uint32_t chrono = (uint32_t)hV_HAL_getMilliseconds();
    myScreen.flush();
    hV_HAL_log(LEVEL_INFO, "%i", hV_HAL_getMilliseconds() - chrono);
    mySerial.println(" ms");
}

void displayFastLine()
{
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);

    uint16_t x = myScreen.screenSizeX();
    uint16_t y = myScreen.screenSizeY();

    uint16_t increment = 8;
    uint16_t dx = x / increment;
    uint16_t dy = y / 5;

    myScreen.selectFont(fontMedium);
    myScreen.gText(0, 0, "Line");

    myScreen.flush();

    myScreen.setPenSolid(true);
    uint32_t chrono;
    for (uint16_t index = 0; index < increment; index += 1)
    {
        uint16_t x0 = index * dx;
        uint16_t x1 = x0 + dx;
        uint16_t y0 = (index % 2) ? y : dy;
        uint16_t y1 = (index % 2) ? dy : y;

        myScreen.line(x0, y0, x1, y1, myColours.black);
        chrono = hV_HAL_getMilliseconds();
        myScreen.flush();
        hV_HAL_log(LEVEL_INFO, "%i - %i = %i", chrono, hV_HAL_getMilliseconds(), hV_HAL_getMilliseconds() - chrono);
    }
}

#endif // DISPLAY_FAST_LINE

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
#if (DISPLAY_FAST_LINE == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_FAST_LINE");
    myScreen.clear();
    displayFastLine();
    hV_HAL_delayMilliseconds(8000);

#endif // DISPLAY_FAST_LINE

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

