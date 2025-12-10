///
/// @file EXT4_WS2813C.ino
/// @brief Example for EXT4 extension board sensors and actuators - All editions
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

// SDK and configuration
// #include <Arduino.h>
#include "PDLS_Common.h"

// Board
Board_EXT myBoard = boardArduinoNanoMatter_EXT4;
// Board_EXT myBoard = boardSiLabsBG24Explorer_EXT4;

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

// Include application, user and local libraries
// WS2813C
/// @warning ezWS2812gpio hangs. Back to previous rawWS2813C
// #include "ezWS2812gpio.h"
#include "rawWS2813C.h"

// Define structures and classes

// Define variables and constants
// WS2813
/// @warning ezWS2812gpio hangs. Back to previous rawWS2813C
// ezWS2812gpio myRGB(1, myBoard.ledData);
rawWS2813C myRGB(1, myBoard.ledData);

// Prototypes

// Functions
void wsFromTo(uint8_t fromRed, uint8_t fromGreen, uint8_t fromBlue,
              uint8_t toRed, uint8_t toGreen, uint8_t toBlue,
              uint8_t steps, uint16_t ms)
{
    hV_HAL_log(LEVEL_INFO, "From %3i.%3i.%3i to %3i.%3i.%3i in %4i steps and %6i ms\n", fromRed, fromGreen, fromBlue,
               toRed, toGreen, toBlue,
               steps, ms);
    uint16_t workRed, workGreen, workBlue;

    for (uint8_t i = 0; i < steps; i += 1)
    {
        workRed = fromRed + (toRed - fromRed) * i / steps;
        workGreen = fromGreen + (toGreen - fromGreen) * i / steps;
        workBlue = fromBlue + (toBlue - fromBlue) * i / steps;
        hV_HAL_log(LEVEL_INFO, "Step %3i: %3i.%3i.%3i\n", i, workRed, workGreen, workBlue);

        // Release 2.2.0 replaces set_pixel() by set_all()
        myRGB.set_all(workRed, workGreen, workBlue);
        hV_HAL_delayMilliseconds(ms);
    }

    hV_HAL_log(LEVEL_INFO, "Step %3i: %3i.%3i.%3i\n", steps, toRed, toGreen, toBlue);
    myRGB.set_all(toRed, toGreen, toBlue);

    hV_HAL_delayMilliseconds(ms);
    hV_HAL_log(LEVEL_INFO, "---");
}

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

    // Check EXT4
    if (myBoard.scope != BOARD_EXT4)
    {
        hV_HAL_log(LEVEL_CRITICAL, "EXT4 board required");
        hV_HAL_exit(RESULT_ERROR);
    }

    // RGB
    myRGB.begin();
    hV_HAL_log(LEVEL_INFO, "Black");
    myRGB.set_all(0, 0, 0);

    hV_HAL_delayMilliseconds(500);
    hV_HAL_log(LEVEL_INFO, "White");
    myRGB.set_all(21, 21, 21);
    hV_HAL_delayMilliseconds(500);
    hV_HAL_log(LEVEL_INFO, "Black");
    myRGB.set_all(0, 0, 0);
    hV_HAL_delayMilliseconds(500);

    hV_HAL_log(LEVEL_INFO, "Red");
    myRGB.set_all(64, 0, 0);
    hV_HAL_delayMilliseconds(500);
    hV_HAL_log(LEVEL_INFO, "Yellow");
    myRGB.set_all(32, 32, 0);
    hV_HAL_delayMilliseconds(500);
    hV_HAL_log(LEVEL_INFO, "Green");
    myRGB.set_all(0, 64, 0);
    hV_HAL_delayMilliseconds(500);
    hV_HAL_log(LEVEL_INFO, "Cyan");
    myRGB.set_all(0, 32, 32);
    hV_HAL_delayMilliseconds(500);
    hV_HAL_log(LEVEL_INFO, "Blue");
    myRGB.set_all(0, 0, 64);
    hV_HAL_delayMilliseconds(500);
    hV_HAL_log(LEVEL_INFO, "Violet");
    myRGB.set_all(32, 0, 32);
    hV_HAL_delayMilliseconds(500);
    hV_HAL_log(LEVEL_INFO, "Black");
    myRGB.set_all(0, 0, 0);

    // wsFromTo(0, 0, 0, 64, 0, 0, 16, 100);
    // wsFromTo(64, 0, 0, 0, 64, 0, 16, 100);
    // wsFromTo(0, 64, 0, 0, 0, 64, 16, 100);
    // wsFromTo(0, 0, 64, 64, 0, 0, 16, 100);
    // wsFromTo(64, 0, 0, 64, 64, 64, 16, 100);
    // wsFromTo(64, 64, 64, 0, 0, 0, 16, 100);

    hV_HAL_exit();
}

///
/// @brief Loop, empty
///
void loop()
{
    hV_HAL_delayMilliseconds(1000);
}

