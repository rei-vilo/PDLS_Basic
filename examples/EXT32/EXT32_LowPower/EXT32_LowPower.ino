///
/// @file EXT32_LowPower.ino
/// @brief Example for Pervasive Displays Library Suite - All editions
///
/// @details Example for Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @date 21 Apr 2026
/// @version 1005
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
#define DISPLAY_LOWPOWER 1

// SDK and configuration
// #include <Arduino.h>
#include "PDLS_Common.h"

// Board
Board_EXT myBoard = boardRaspberryPiPico_RP2040_EXT3;

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
#if (FONT_MODE == USE_FONT16_HEADER)
#include "hV_Font16_Latin_DejaVu.h"
#endif
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
        hV_HAL_log(LEVEL_INFO, "Wait %i", i);
        hV_HAL_delayMilliseconds(1000);
    }
    hV_HAL_Serial_crlf();
}

// Functions
#if (DISPLAY_LOWPOWER == 1)

void displayLowPower(bool flagLowPower = true)
{
    uint16_t x = 4;
    uint16_t y = 4;

    hV_HAL_Serial_crlf();
    hV_HAL_log(LEVEL_INFO, flagLowPower ? "DISPLAY_LOW_POWER" : "DISPLAY_NORMAL_POWER");

    // Resume from low power, for demonstration purposes
    // Not needed as flushFast() checks and resumes power if needed
    myScreen.resume();

    myScreen.setOrientation(ORIENTATION_LANDSCAPE);
    myScreen.selectFont(fontLarge);
    uint16_t dt = myScreen.characterSizeY()  + 4;

    myScreen.clear();
    myScreen.gText(x, y, myScreen.WhoAmI());
    y += dt;

    myScreen.gText(x, y, flagLowPower ? "WITH LOW POWER" : "WITH NORMAL POWER");
    y += dt;

    myScreen.gText(x, y, "Wait 4 s...");
    y += dt;

    myScreen.flushFast();

    if ((flagLowPower))
    {
        // Enter low power
        myScreen.suspend(POWER_SCOPE_BUS_GPIO);
    }
}

#endif // DISPLAY_LOWPOWER

///
/// @brief Setup
///
void setup()
{
    hV_HAL_begin();
    hV_HAL_GPIO_define(LED_BUILTIN, OUTPUT);
    hV_HAL_GPIO_set(LED_BUILTIN);
    hV_HAL_delayMilliseconds(1000);

    hV_HAL_Serial_crlf();
    hV_HAL_log(LEVEL_INFO, __FILE__);
    hV_HAL_log(LEVEL_INFO, __DATE__ " " __TIME__);
    hV_HAL_Serial_crlf();

    // Screen
    // Pin 8 = panelPower on EXT3.2 = panelCS on EXT3.1
    // setPanelPowerPin() sets panelCS = NOT_CONNECTED if already set to panelCS
    myScreen.setPanelPowerPin(myBoard.panelCS);
    myScreen.begin();
    myScreen.setPowerProfile(POWER_MODE_MANUAL, POWER_SCOPE_BUS_GPIO);
    myScreen.suspend(POWER_SCOPE_BUS_GPIO);
    hV_HAL_GPIO_clear(LED_BUILTIN);
    wait(2);

    // Fonts
#if (FONT_MODE == USE_FONT_TERMINAL)

    fontSmall = Font_Terminal6x8;
    fontMedium = Font_Terminal8x12;
    fontLarge = Font_Terminal12x16;
    fontVery = Font_Terminal16x24;

#else // FONT_MODE

    fontSmall = myScreen.addFont(Font16_Latin_DejaVuSans14);
    fontSmall -= ((fontSmall > 0) ? 1 : 0);
    fontMedium = myScreen.addFont(Font16_Latin_DejaVuSans14b);
    fontMedium -= ((fontMedium > 0) ? 1 : 0);
    fontLarge = myScreen.addFont(Font16_Latin_DejaVuSans20);
    fontLarge -= ((fontLarge > 0) ? 1 : 0);
    fontVery = myScreen.addFont(Font16_Latin_DejaVuMono48);
    fontVery -= ((fontVery > 0) ? 1 : 0);

#endif // FONT_MODE

    // Example
#if (DISPLAY_LOWPOWER == 1)

    displayLowPower(false);
    wait(4);

    displayLowPower(true);
    wait(4);

#endif // DISPLAY_LOWPOWER

    hV_HAL_Serial_crlf();
    hV_HAL_log(LEVEL_INFO, "Regenerate");
    myScreen.regenerate();

    hV_HAL_GPIO_set(LED_BUILTIN);
    hV_HAL_exit();
}

///
/// @brief Loop, empty
///
void loop()
{
    hV_HAL_delayMilliseconds(1000);
}

