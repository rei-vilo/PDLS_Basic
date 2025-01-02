///
/// @file EXT4_Accelerometer.ino
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
#include "Driver_EPD_Wide_Small.h"

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

// Set parameters
#define DISPLAY_ACCELEROMETER 1

// Define structures and classes

// Define variables and constants
pins_t myBoard = boardArduinoNanoMatter;
// pins_t myBoard = boardSiLabsBG24Explorer;
Driver_EPD_Wide_Small myDriver(eScreen_EPD_290_KS_0F, myBoard);
Screen_EPD myScreen(&myDriver);

uint8_t fontSmall, fontMedium, fontLarge, fontVery;

uint8_t orientation = 9;
uint8_t oldOrientation = 0;
int16_t aX, aY, aZ;

// Prototypes

// Accelerometer
#define ACC_I2C 0x19

uint8_t bufferWrite[8] = { 0 };
uint8_t bufferRead[8] = { 0 };

typedef union
{
    int16_t i16[3];
    uint8_t ui8[6];
} xyz_i16_ui8_t;
xyz_i16_ui8_t myXYZ;

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

    // Start
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
    myScreen.regenerate(); // Clear buffer and screen

    // MEMS digital output motion sensor: ultra-low-power high-performance 3-axis "femto" accelerometer
    // https://www.st.com/en/mems-and-sensors/lis2dh12.html
    hV_HAL_Wire_begin();

    bufferWrite[0] = 0x0f; // WhoAmI, should be 0x33
    hV_HAL_Wire_transfer(ACC_I2C, bufferWrite, 1, bufferRead, 1);

    hV_HAL_log(LEVEL_INFO, "WhoAmI = 0x%02x", bufferRead[0]);

    bufferWrite[0] = 0x23;
    bufferWrite[1] = 0x80; // Block data update
    hV_HAL_Wire_transfer(ACC_I2C, bufferWrite, 2, bufferRead, 0);

    bufferWrite[0] = 0x20;
    // bufferWrite[1] = 0x2f;  // 10 Hz, LPM, XYZ
    bufferWrite[1] = 0x1f;  // 1 Hz, LPM, XYZ
    hV_HAL_Wire_transfer(ACC_I2C, bufferWrite, 2, bufferRead, 0);

    bufferWrite[0] = 0x1f;
    bufferWrite[1] = 0x00; // No temperature
    hV_HAL_Wire_transfer(ACC_I2C, bufferWrite, 2, bufferRead, 0);

    bufferRead[0] = 0x00;
    while ((bufferRead[0] & 0x0f) != 0x0f)
    {
        bufferWrite[0] = 0x27; // Status
        hV_HAL_Wire_transfer(ACC_I2C, bufferWrite, 1, bufferRead, 1);
        mySerial.print(".");
        hV_HAL_delayMilliseconds(8);
    }
    mySerial.println();

    hV_HAL_GPIO_define(myBoard.button, INPUT_PULLUP);
}

///
/// @brief Loop
///
void loop()
{
    bufferWrite[0] = 0x27;  // Status
    bufferRead[0] = 0x00;
    hV_HAL_Wire_transfer(ACC_I2C, bufferWrite, 1, bufferRead, 1);

    if ((bufferRead[0] & 0x0f) == 0x0f)
    {

        bufferWrite[0] = 0xa8;  // xyz, MSB
        bufferRead[0] = 0x00;

        hV_HAL_Wire_transfer(ACC_I2C, bufferWrite, 1, myXYZ.ui8, 6);
        aX = myXYZ.i16[0] / 16;
        aY = myXYZ.i16[1] / 16;
        aZ = myXYZ.i16[2] / 16;

        mySerial.println();
        hV_HAL_log(LEVEL_INFO, "x %5i, y %5i, z %5i", myXYZ.i16[0] / 16, myXYZ.i16[1] / 16, myXYZ.i16[2] / 16);

        oldOrientation = orientation;
        if (oldOrientation == 9) // Default
        {
            orientation = 3;
        }

        // Tilt Sensing Using a Three-Axis Accelerometer
        // https://www.nxp.com/docs/en/application-note/AN3461.pdf
        if (abs(aZ) < 900)
        {
            if ((aX > 500) and (abs(aY) < 400))
            {
                orientation = 2;
            }
            else if ((aX < -500) and (abs(aY) < 400))
            {
                orientation = 0;
            }
            else if ((aY > 500) and (abs(aX) < 400))
            {
                orientation = 3;
            }
            else if ((aY < -500) and (abs(aX) < 400))
            {
                orientation = 1;
            }
        }
        hV_HAL_log(LEVEL_INFO, ", o %i -> %i", oldOrientation, orientation);

        if (orientation != oldOrientation) // Update display
        {
            myScreen.clear();
            myScreen.setOrientation(orientation);
            myScreen.selectFont(fontMedium);
            myScreen.gText(0, 0, "Orientation");
            uint16_t dt = myScreen.characterSizeY();
            myScreen.selectFont(fontLarge);
            myScreen.gText(0, dt, formatString("%i", orientation));
            myScreen.gText(1, dt, formatString("%i", orientation));

            uint16_t x = myScreen.screenSizeX();
            uint16_t y = myScreen.screenSizeY();
            uint16_t dx = x / 2;
            uint16_t dy = y / 2;
            uint16_t dz = hV_HAL_min(dx, dy) / 3;

            myScreen.setPenSolid(true);
            myScreen.triangle(dx, dy - dz, dx + dz, dy, dx - dz, dy, myColours.black);
            myScreen.rectangle(dx + dz / 3, dy, dx - dz / 3, dy + dz, myColours.black);

            myScreen.setPenSolid(false);
            myScreen.circle(dx, dy, dz  + 8, myColours.black);

            myScreen.flush();
            mySerial.print(" *");
        }
    }
    else
    {
        mySerial.print(".");
        hV_HAL_delayMilliseconds(8);
    }

    if (hV_HAL_GPIO_read(myBoard.button) == LOW)
    {
        myScreen.regenerate();
        while (true)
        {
            hV_HAL_delayMilliseconds(1000);
        }
    }

    hV_HAL_delayMilliseconds(1000);
}

