///
/// @file rawWS2813C.cpp
///
/// @brief Driver for WS2813C
/// @version 103
/// @date 16 Feb 2024
///
/// @copyright Copyright (c) 2021-2025 Pervasive Displays
/// @copyright License CC-BY-SA Creative Commons - Attribution - Share Alike
/// https://creativecommons.org/licenses/by-sa/4.0/deed.en
///

#include "rawWS2813C.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "em_common.h"
#include "em_gpio.h"
#include "sl_udelay.h"

///
/// @brief Define GPIO for WS2813C
/// @param port port, gpioPortA..gpioPortD
/// @param pin pin, O..16 according to port
///
void wsDefine(uint8_t port, uint8_t pin);

///
/// @brief Set RGB values
/// @param red red component, 0..255
/// @param green green component, 0..255
/// @param blue blue component, 0..255
///
void wsWrite(uint8_t red, uint8_t green, uint8_t blue);

uint8_t wsPort;
uint8_t wsBit;

void wsDefine(uint8_t port, uint8_t pin)
{
    wsPort = port;
    wsBit = pin;

    GPIO_PinModeSet(wsPort, wsBit, gpioModePushPull, 0);
    wsWrite(0, 0, 0);
}

void wsWrite(uint8_t red, uint8_t green, uint8_t blue)
{
    // uint32_t valueG70R70B70 = 0b111100001010101011001100; // 0xf0aacc
    uint32_t valueG70R70B70 = (green << 16) | (red << 8) | blue;
    uint32_t valueB07R07G07 = 0;

    for (uint8_t i = 0; i < 24; i += 1)
    {
        valueB07R07G07 <<= 1;
        valueB07R07G07 |= (valueG70R70B70 & 1);
        valueG70R70B70 >>= 1;
    }

    for (uint8_t i = 0; i < 24; i += 1)
    {
        if (valueB07R07G07 & 1)
        {
            // 1 code = T1H T1L
            // T1H 1-code, High-level time 580ns~1.6μs
            // T1L 1-code, Low-level time 220ns~420ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns * 14 = 784 ns

            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns * 6 = 276 ns
        }
        else
        {
            // 0 code = T0H T0L
            // T0H 0-code, High-level time 220ns~380ns
            // T0L 0-code, Low-level time 580ns~1.6μs
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns
            GPIO_PinOutSet(wsPort, wsBit);  // 56 ns * 5 = 280 ns

            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns
            GPIO_PinOutClear(wsPort, wsBit);  // 46 ns * 15 = 690 ns
        }
        valueB07R07G07 >>= 1;
    }
}


rawWS2813C::rawWS2813C(uint32_t num_leds, pin_size_t pin)
{
    wsPinArduino = pin;
}

void rawWS2813C::begin()
{
    PinName wsName;

    wsName = pinToPinName(wsPinArduino);
    wsPort = getSilabsPortFromArduinoPin(wsName);
    wsBit = getSilabsPinFromArduinoPin(wsName);

    wsDefine(wsPort, wsBit);
}

void rawWS2813C::set_all(uint8_t red, uint8_t green, uint8_t blue)
{
    wsWrite(red, green, blue);
};
