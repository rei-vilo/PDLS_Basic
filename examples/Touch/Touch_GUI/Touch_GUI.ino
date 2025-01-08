///
/// @file Touch_GUI.ino
/// @brief Example of features for Evaluation edition
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
// #include <Wire.h>
// #include <SPI.h>

// Driver
#include "Pervasive_Touch_Small.h"

// Screen
#include "PDLS_Basic.h"

#if (SCREEN_EPD_RELEASE < 902)
#error Required SCREEN_EPD_RELEASE 902
#endif // SCREEN_EPD_RELEASE

// Set parameters
#define DISPLAY_GUI 1

// Define structures and classes

// Define variables and constants
// Driver
// Pervasive_Touch_Small myDriver(eScreen_EPD_370_PS_0C_Touch, boardRaspberryPiPico_RP2040);
Pervasive_Touch_Small myDriver(eScreen_EPD_271_KS_09_Touch, boardRaspberryPiPico_RP2040);

// Screen
Screen_EPD myScreen(&myDriver);

uint8_t fontSmall, fontMedium, fontLarge, fontVery;

// GUI
#include "hV_GUI.h"

#ifndef WITH_TOUCH
#error Required WITH_TOUCH
#endif // WITH_TOUCH

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
        mySerial.print(formatString(" > %i  \r", i));
        hV_HAL_delayMilliseconds(1000);
    }
    mySerial.print("         \r");
}

// Functions
#if (DISPLAY_GUI == 1)

GUI myGUI(&myScreen);

void displayGUI()
{
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);
    myScreen.selectFont(fontMedium);

    myScreen.clear();

    myGUI.begin();

    Button myButtonNormal(&myGUI);
    Button myButtonInstant(&myGUI);
    Text myText(&myGUI);

    uint16_t x = myScreen.screenSizeX();
    uint16_t y = myScreen.screenSizeY();

    uint16_t dx = x / 7;
    uint16_t dy = y / 5;

    myGUI.delegate(false);

    myButtonNormal.dStringDefine(dx * 1, dy * 3, dx * 2, dy, "Normal", fontMedium);
    myButtonInstant.dStringDefine(dx * 4, dy * 3, dx * 2, dy, "Instant", fontMedium);
    myText.dDefine(0, dy, x, dy, fontMedium);

    myButtonNormal.draw();
    myButtonInstant.draw();
    myText.draw("Empty");

    myScreen.flush();

    myGUI.delegate(true);

    uint8_t k = 8;
    uint32_t chrono32;
    while (k > 0)
    {
        if (myScreen.getTouchInterrupt())
        {
            chrono32 = hV_HAL_getMilliseconds();
            if (myButtonNormal.check(checkNormal))
            {
                k -= 1;
                chrono32 = hV_HAL_getMilliseconds() - chrono32;
                myText.draw(formatString("%s in %i ms (%i left)", "Normal", chrono32, k));
                hV_HAL_log(LEVEL_INFO, "%3i: %s in %i ms", k, "Normal", chrono32);
            }

            chrono32 = hV_HAL_getMilliseconds();
            if (myButtonInstant.check(checkInstant))
            {
                k -= 1;
                chrono32 = hV_HAL_getMilliseconds() - chrono32;
                myText.draw(formatString("%s in %i ms (%i left)", "Instant", chrono32, k));
                hV_HAL_log(LEVEL_INFO, "%3i: %s in %i ms", k, "Instant", chrono32);
            }
        } // getTouchInterrupt

        hV_HAL_delayMilliseconds(100);
    }

    myScreen.clear();
}
#endif // DISPLAY_GUI

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
    myScreen.regenerate();

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
#if (DISPLAY_GUI == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_GUI");
    myScreen.clear();
    displayGUI();
    wait(8);

#endif // DISPLAY_GUI

    hV_HAL_log(LEVEL_INFO, "Regenerate");
    myScreen.regenerate();

    hV_HAL_exit(0);
}

///
/// @brief Loop, empty
///
void loop()
{
    hV_HAL_delayMilliseconds(1000);
}
