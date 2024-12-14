///
/// @file EXT4_ScreenSizes.ino
/// @brief Example for EXT4 extension board sensors and actuators - All editions
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Nov 2024
/// @version 900
///
/// @copyright (c) Rei Vilo, 2010-2024
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

// SDK and configuration
// #include <Arduino.h>
#include "PDLS_Common.h"

// Driver
#include "Driver_EPD_Wide_Small.h"

// Screen
#include "PDLS_Basic.h"

#if (SCREEN_EPD_RELEASE < 900)
#error Required SCREEN_EPD_RELEASE 900
#endif // SCREEN_EPD_RELEASE

#if (USE_EXT_BOARD != BOARD_EXT4)
#error Required USE_EXT_BOARD = BOARD_EXT4
#endif // USE_EXT_BOARD

// Set parameters
#define DISPLAY_SIZES 1

// Define structures and classes

// Define variables and constants
// Board
pins_t myBoard = boardArduinoNanoMatter;
// pins_t myBoard = boardSiLabsBG24Explorer;

// Driver
// Driver_EPD_Wide_Small myDriver(eScreen_EPD_152_KS_0J, myBoard);
// Driver_EPD_Wide_Small myDriver(eScreen_EPD_206_KS_0E, myBoard);
// Driver_EPD_Wide_Small myDriver(eScreen_EPD_213_KS_0E, myBoard);
// Driver_EPD_Wide_Small myDriver(eScreen_EPD_266_KS_0C, myBoard);
// Driver_EPD_Wide_Small myDriver(eScreen_EPD_271_KS_0C, myBoard);
Driver_EPD_Wide_Small myDriver(eScreen_EPD_290_KS_0F, myBoard);

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
#if (DISPLAY_SIZES == 1)

///
/// @brief Displays x.y size
///
void displayWhoAmI()
{
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);
    myScreen.selectFont(fontMedium);

    uint16_t x = 4;
    uint16_t y = 4;
    uint16_t dy = myScreen.characterSizeY();

    myScreen.gText(x, y, formatString("x %i", myScreen.screenSizeX()));
    y += dy;
    myScreen.gText(x, y, formatString("y %i", myScreen.screenSizeY()));
    y += dy;

    myScreen.setPenSolid(true);
    myScreen.dRectangle(x + dy * 0, y, dy - 1, dy - 1, myColours.black);
    myScreen.setPenSolid(false);
    myScreen.dRectangle(x + dy * 1, y, dy - 1, dy - 1, myColours.black);

    myScreen.selectFont(fontSmall);
    char * text = myScreen.screenNumber();
    x = myScreen.screenSizeX() - myScreen.stringSizeX(text) - 4;
    y = myScreen.screenSizeY() - myScreen.characterSizeY() - 4;
    myScreen.gText(x, y, text);

    myScreen.flush();
}

#endif // DISPLAY_SIZES

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
    if (myBoard.panelPower == NOT_CONNECTED)
    {
        hV_HAL_log(LEVEL_INFO, "panelPower not connected");
        hV_HAL_exit(0x01);
    }
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
#if (DISPLAY_SIZES == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_SIZES");
    myScreen.clear();
    displayWhoAmI();
    wait(8);

#endif // DISPLAY_SIZES

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
