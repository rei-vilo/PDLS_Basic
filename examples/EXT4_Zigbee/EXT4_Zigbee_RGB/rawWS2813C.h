///
/// @file rawWS2813C.h
/// @brief Driver for WS2813C
/// @version 1011
/// @date 17 Oct 2026
///
/// @copyright (c) Pervasive Displays Inc., 2024
/// @copyright License CC-BY-SA Creative Commons - Attribution - Share Alike
/// https://creativecommons.org/licenses/by-sa/4.0/deed.en
///
/// @see https://github.com/SiliconLabs/arduino/issues/106
///
/// @warning `-Os` optimization level required to match the data transfer time of the WS2813C.
/// @n By default, Arduino uses the `-O0` optimization level, too slow to transfer the data.
/// @warning Different profiles, specific for ARDUINO_ZIGBEE and general
/// @warning Measure data transfer time with a logic analyser and adapt the profile accordingly
///

#ifndef rawWS2813C_RELEASE
#define rawWS2813C_RELEASE 1011

#include "Arduino.h"

///
/// @class rawWS2813C
/// @details Minimal class for WS2813C
///
class rawWS2813C
{
  public:
    ///
    /// @brief Construct WS2813C object
    ///
    /// @param num_leds 1, ignored
    /// @param pin Arduino pin, name `D4` or number `4`
    ///
    rawWS2813C(uint32_t num_leds, pin_size_t pin);

    ///
    /// @brief Initialise
    ///
    void begin();

    ///
    /// @brief Set RGB LED
    ///
    /// @param red red component, 0..255
    /// @param green green component, 0..255
    /// @param blue blue component, 0..255
    ///
    void set_all(uint8_t red, uint8_t green, uint8_t blue);

  private:
    uint8_t wsPinArduino;
    uint32_t wsBit;
    uint8_t wsPort; // enum GPIO_Port_TypeDef
};

#endif // rawWS2813C_RELEASE

