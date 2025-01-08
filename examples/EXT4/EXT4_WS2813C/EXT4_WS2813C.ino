///
/// @file EXT4_WS2813C.ino
/// @brief Example for EXT4 extension board sensors and actuators - All editions
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

// SDK and configuration
// #include <Arduino.h>
#include "PDLS_Common.h"

// Driver
#include "Pervasive_Wide_Small.h"

// Screen
#include "PDLS_Basic.h"

#if (SCREEN_EPD_RELEASE < 902)
#error Required SCREEN_EPD_RELEASE 902
#endif // SCREEN_EPD_RELEASE

// Include application, user and local libraries
// #include <SPI.h>

#if (USE_EXT_BOARD != BOARD_EXT4)
#error Required USE_EXT_BOARD = BOARD_EXT4
#endif // USE_EXT_BOARD

// WS2813C
#include "ezWS2812gpio.h"

// Set parameters

// Define structures and classes

// Define variables and constants
const pins_t myBoard = boardArduinoNanoMatter;

// WS2813
ezWS2812gpio myRGB(1, myBoard.ledData);

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
    // hV_HAL_Serial = Serial by default, otherwise edit hV_HAL_Peripherals.h
    hV_HAL_begin(); // with Serial at 115200

    hV_HAL_Serial_crlf();
    hV_HAL_log(LEVEL_INFO, __FILE__);
    hV_HAL_log(LEVEL_INFO, __DATE__ " " __TIME__);
    hV_HAL_Serial_crlf();

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

