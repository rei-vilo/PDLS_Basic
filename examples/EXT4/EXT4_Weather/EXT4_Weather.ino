///
/// @file EXT4_Weather.ino
/// @brief Example for EXT4 extension board sensors and actuators - All editions
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
/// @see
/// * HDC2080 2% RH ultra-low-power digital relative humidity sensor, interrupt/DRDY
/// https://www.ti.com/product/HDC2080
/// * Weather Sensors Library
/// https://github.com/rei-vilo/SensorsWeather_Library
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

// HDC2080
#include "Wire.h"

// Set parameters

// Define structures and classes

// Define variables and constants
// Board
pins_t myBoard = boardArduinoNanoMatter;
// pins_t myBoard = boardSiLabsBG24Explorer;

// Driver
Pervasive_Wide_Small myDriver(eScreen_EPD_290_KS_0F, myBoard);

// Screen
Screen_EPD myScreen(&myDriver);

uint8_t fontSmall, fontMedium, fontLarge, fontVery;

// HDC2080
#define HDC_I2C 0x40

uint8_t bufferWrite[8] = { 0 };
uint8_t bufferRead[8] = { 0 };

struct measure_s
{
    float value;
    float oldValue = 999.9;
    float minimum = 999.9;
    float maximum = -999.9;
};

measure_s temperature;
measure_s humidity;

static uint32_t chrono32 = 0;
static uint8_t countFlush = 1; // Counter for normal update
const uint8_t FAST_BEFORE_NORMAL = 8; // Number of fast updates before normal update
bool flagDisplay = true;

// Prototypes
bool displayValue(uint8_t slot, STRING_CONST_TYPE name, measure_s * value, STRING_CONST_TYPE unit);

// Functions
// --- Screen
bool displayValue(uint8_t slot, STRING_CONST_TYPE name, measure_s * value, STRING_CONST_TYPE unit)
{
    uint16_t x = myScreen.screenSizeX();
    uint16_t y = myScreen.screenSizeY();
    uint16_t dx, dy, x0, y0;

    x0 = x * slot / 2;
    dx = x / 8;
    y0 = 0;
    dy = y / 5;

    (*value).value = ((int32_t)(10 * (*value).value + 5)) / 10.0;
    bool result = ((*value).value != (*value).oldValue);
    (*value).oldValue = (*value).value;
    (*value).maximum = hV_HAL_max((*value).maximum, (*value).value);
    (*value).minimum = hV_HAL_min((*value).minimum, (*value).value);

    myScreen.setPenSolid(true);
    myScreen.setFontSolid(true);
    myScreen.dRectangle(x0, y0, dx * 4, dy * 4, myColours.white);

    myScreen.selectFont(fontLarge);
    myScreen.gText(x0, y0, name);

    myScreen.selectFont(fontVery);
    myScreen.gTextLarge(x0, y0 + 1 * dy, formatString("%5.1f", (*value).value));

    myScreen.selectFont(fontLarge);
    char unit_c[4] = {0};
    strcpy(unit_c, utf2iso(unit).c_str());
    myScreen.gText(x0 + 3 * dx - myScreen.characterSizeX() * 0, y0 + 1 * dy - myScreen.characterSizeY(), formatString("%s", unit_c));

    myScreen.selectFont(fontMedium);
    myScreen.gText(x0, y0 + 3 * dy, "Minimum");
    myScreen.gText(x0 + 2 * dx, y0 + 3 * dy, "Maximum");

    myScreen.selectFont(fontLarge);
    myScreen.gText(x0, y0 + 4 * dy, formatString("%5.1f", (*value).minimum));
    myScreen.gText(x0 + 2 * dx, y0 + 4 * dy, formatString("%5.1f", (*value).maximum));

    // for (uint8_t i = 0; i < 4; i += 1)
    // {
    //     myScreen.line(x0, y0 + i * dy, x0 + x, y0 + i * dy, myColours.grey);
    //     myScreen.line(x0 + i * dx, y0, x0 + i * dx, y0 + y, myColours.grey);
    // }

    myScreen.setPenSolid(false);
    return result;
}
// --- End of Screen

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
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);
    myScreen.regenerate(); // Clear buffer and screen

    // --- HDC2080
    hV_HAL_Wire_begin();

    bufferWrite[0] = 0x0e; // HDC20X0_CONFIGURATION
    bufferWrite[1] = 0x00; // HDC20X0_DEFAULT_SETTINGS
    hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 2, bufferRead, 0);

    bufferWrite[0] = 0x0f; // HDC20X0_MEASURE_CONFIGURATION
    bufferWrite[1] = 0xfe; // HDC20X0_MEASURE_SETTINGS
    hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 2, bufferRead, 0);
    // --- End of HDC2080
}

///
/// @brief Loop
///
void loop()
{
    if (hV_HAL_getMilliseconds() > chrono32)
    {
        // HDC2080
        bufferWrite[0] = 0x0f; // HDC20X0_MEASURE_CONFIGURATION
        bufferWrite[1] = 0xff; // HDC20X0_DEFAULT_SETTINGS
        hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 2, bufferRead, 0);
        hV_HAL_delayMilliseconds(50);

        // Temperature
        bufferWrite[0] = 0x00; // HDC20X0_TEMPERATURE_LOW
        hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 1, bufferRead, 2);
        temperature.value = bufferRead[0] + bufferRead[1] * 256.0;
        temperature.value = temperature.value * 165.0 / 65536.0 - 40.0; // +273.15; // from °C to °K

        // Humidity
        bufferWrite[0] = 0x02; // HDC20X0_HUMIDITY_LOW
        hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 1, bufferRead, 2);
        humidity.value = bufferRead[0] + bufferRead[1] * 256.0;
        humidity.value = humidity.value / 65536.0 * 100.0;

        // Screen
        countFlush %= FAST_BEFORE_NORMAL;
        if (countFlush == 0)
        {
            myScreen.regenerate();
            hV_HAL_log(LEVEL_INFO, "countFlush = 0 ! regenerate");
            flagDisplay = true;
        }

        flagDisplay |= displayValue(0, "Temperature", &temperature, "°C");
        flagDisplay |= displayValue(1, "Humidity", &humidity, "%");
        if (flagDisplay == true)
        {
            countFlush += 1;
            myScreen.flush();

            flagDisplay = false;

            hV_HAL_log(LEVEL_INFO, "countFlush = %i", countFlush);
        }

        // Serial
        hV_HAL_log(LEVEL_INFO, "Temperature = %5.1f < %5.1f < %5.1f oC, Humidity= %5.1f < %5.1f < %5.1f %%",
                   temperature.minimum, temperature.value, temperature.maximum,
                   humidity.minimum, humidity.value, humidity.maximum);

        chrono32 = hV_HAL_getMilliseconds() + 10000; // 10 s
    }

    hV_HAL_delayMilliseconds(1000);
}

