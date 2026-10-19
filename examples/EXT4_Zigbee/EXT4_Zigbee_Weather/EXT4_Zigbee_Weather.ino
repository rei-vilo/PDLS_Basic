///
/// @file EXT4_Zigbee_Weather.ino
/// @brief Example for EXT4 extension board sensors and actuators - All editions
///
/// @details Based on Zigbee multiple sensor example
///
/// The example shows how to create multiple sensor instances with the Arduino Zigbee API.
///
/// The example creates a Zigbee temperature sensor device and publishes the current CPU temperature through it.
/// The device has to be commissioned to a Zigbee network first.
/// Open your Zigbee coordinator (e.g. Home Assistant with ZHA or Zigbee2MQTT) and put it in pairing mode.
///
/// Compatible boards:
/// - Arduino Nano Matter
/// - Silicon Labs xG24 Explorer Kit
/// - SparkFun Thing Plus Matter
/// - Seeed Studio XIAO MG24 (Sense)
///
/// @author Tamas Jozsi (Silicon Labs)
///
/// @date 21 Oct 2026
/// @version 1011
///
/// @copyright (c) Pervasive Displays Inc., 2021-2026
/// @copyright All rights reserved
/// @copyright For exclusive use with Pervasive Displays screens
/// * 2026-10-21 Rei Vilo
/// * Added support for EXT4
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
#define ZIGBEE_VENDOR_NAME "Pervasive Displays Inc."
#define ZIGBEE_EXAMPLE_NAME "Zigbee Weather"
#define ZIGBEE_EXAMPLE_RELEASE 201

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
uint8_t fontSmall, fontMedium, fontLarge, fontVery;

// Checks: Silicon Labs Zigbee only
#ifndef ARDUINO_ARCH_SILABS
#error Silicon Labs architecture required
#endif

// Checks: Arduino Zigbee only
#ifndef ARDUINO_ZIGBEE
#error Zigbee library required
#endif

// Include application, user and local libraries
#include <Zigbee.h>
#include <ZigbeeTemperatureSensor.h>
#include <ZigbeeHumiditySensor.h>

#define myZigbee Zigbee
ZigbeeTemperatureSensor myZigbeeTemperature;
ZigbeeHumiditySensor myZigbeeHumidity;

// #include "ZigbeeAfLock.h"
// extern "C" {
//     #include "stack-info.h"
// }

//
// --- HDC2080 section
//
#include "Wire.h"

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
static uint8_t countFlush = 1;          // Counter for normal update
const uint8_t FAST_BEFORE_NORMAL = 16;  // Number of fast updates before normal update

static uint8_t countSend = 1;         // Counter for fast update
const uint8_t SEND_BEFORE_FAST = 16;  // Number of Zigbee sends before fast update
bool flagDisplay = true;

// Prototypes

///
/// @brief Display the About page
/// @see https://www.pervasivedisplays.com
///
void displayAbout();

///
/// @brief Display the QR-code
///
/// @param code text
/// @see https://github.com/ricmoo/qrcode/
///
void displayQR(const char * code);

///
/// @brief Manage and display the measure
///
/// @param slot 0 or 1
/// @param name name of the measure
/// @param value measure as measure_s structure
/// @param unit text of the unit
/// @return true if different value
/// @note
/// * value rounded to one decimal
/// * min and max values updated
/// * old value updated
///
bool displayValue(uint8_t slot, STRING_CONST_TYPE name, measure_s * value, STRING_CONST_TYPE unit);

///
/// @brief Display a menu managed by a single button
///
/// @param button GPIO of the button
/// @param title text for title
/// @param option1 text for option 1, required
/// @param option2 text for option 2, optional
/// @param option3 text for option 3, optional
/// @param option4 text for option 4, optional
/// @return number of the selected option, `0` = none
/// @note Procedure
/// * Press and hold the button to choose the option
/// * A progress bar shows the selected option
/// * Release the button to select the option
/// @warning button should be initialised before calling `menuOneButton()`
///
uint8_t menuOneButton(uint8_t button, const char * title, const char * option1, const char * option2 = 0, const char * option3 = 0, const char * option4 = 0);

///
/// @brief Display QR-code
/// @param code text
///
void displayQR(const char * code);

///
/// @brief Display the commission process
///
void displayCommissioning();

///
/// @brief Display the decommission process
///
void displayDecommissioning();

///
/// @brief Display the identification
///
void displayIdenfication();

// Functions

// --- QR-code
#include "qrcode.h"

void displayQR(const char * code)
{
    // Create the QR code
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(3)];
    qrcode_initText(&qrcode, qrcodeData, 3, 0, code);

    uint16_t x = myScreen.screenSizeX();
    uint16_t y = myScreen.screenSizeY();
    uint8_t k = qrcode.size;
    uint16_t dxy = hV_HAL_min(x, y);
    uint16_t dz = dxy / k;
    uint16_t dxy0 = (dxy - k * dz) / 2;
    uint16_t dx0 = x - dxy + dxy0;
    uint16_t dy0 = 0 + dxy0;

    myScreen.setPenSolid(true);
    myScreen.dRectangle(x - dxy, 0, dxy, dxy, myColours.white);

    for (uint8_t jy = 0; jy < k; jy++)
    {
        for (uint8_t ix = 0; ix < k; ix++)
        {
            uint16_t colour = qrcode_getModule(&qrcode, ix, jy) ? myColours.black : myColours.white;
            myScreen.dRectangle(dx0 + dz * ix, dy0 + dz * jy, dz, dz, colour);
        }
    }
    myScreen.setPenSolid(false);
}
// --- End of QR-code

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
    char unit_c[4] = { 0 };
    strcpy(unit_c, unit.c_str());
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

void displayAbout()
{
    myScreen.clear();

    myScreen.selectFont(fontLarge);
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t dy = myScreen.characterSizeY();

    hV_HAL_log(LEVEL_INFO, ZIGBEE_EXAMPLE_NAME);
    myScreen.gText(x, y, ZIGBEE_EXAMPLE_NAME);
    y += dy * 2;
    myScreen.selectFont(fontMedium);

    myScreen.gText(x, y, formatString("%8s v%i.%i.%i", "Release", ZIGBEE_EXAMPLE_RELEASE / 100, (ZIGBEE_EXAMPLE_RELEASE / 10) % 10, ZIGBEE_EXAMPLE_RELEASE % 10));
    y += dy;
    myScreen.gText(x, y, formatString("%8s pin %i", "Switch", myBoard.button));
    y += dy;

    myScreen.gText(x, y, formatString("%8s %s", "Screen", myScreen.WhoAmI().c_str()));
    y += dy;
    myScreen.gText(x, y, formatString("%8s %i x %i", "Size", myScreen.screenSizeX(), myScreen.screenSizeY()));
    y += dy;
    myScreen.gText(x, y, formatString("%8s %s", "Number", myScreen.screenNumber().c_str()));
    y += dy;
    myScreen.gText(x, y, formatString("%8s %s", "PDLS", myScreen.reference().c_str()));
    y += dy;

    myScreen.gText(x, y, formatString("%8s %s", "Colours", ""));
    uint16_t dx = x + myScreen.characterSizeX() * 9;
    myScreen.setPenSolid(true);
    myScreen.dRectangle(dx + dy * 0, y, dy - 1, dy - 1, myColours.black);
    myScreen.setPenSolid(false);
    myScreen.dRectangle(dx + dy * 1, y, dy - 1, dy - 1, myColours.black);
    myScreen.setPenSolid(true);

    uint8_t number = myScreen.screenColours();

    if (number >= 3)
    {
        myScreen.dRectangle(dx + dy * 2, y, dy - 1, dy - 1, myColours.red);

#if defined(WITH_COLOURS_BWRY)
        if (number == 4)
        {
            myScreen.dRectangle(dx + dy * 3, y, dy - 1, dy - 1, myColours.yellow);
        }
#endif // WITH_COLOURS_BWRY
    }

    y += dy;
    myScreen.gText(x, myScreen.screenSizeY() - dy, "Wait 10 seconds...");

    displayQR("https://www.pervasivedisplays.com");

    myScreen.flush();
    hV_HAL_delayMilliseconds(10000);
    myScreen.clear();
}
//
// --- End of Screen
//

void displayIdenfication()
{
    // hV_HAL_GPIO_define(LED_BUILTIN, OUTPUT);
    // static bool oldIdentification = false;
    // bool flagIdentification = false;
    // flagIdentification |= myZigbeeTemperature.get_identify_in_progress();
    // flagIdentification |= myZigbeeHumidity.get_identify_in_progress();
    // // Handle the identification enable/disable transitions
    // if ((flagIdentification == true) and (oldIdentification == false))
    // {
    //     // Identification starts
    //     hV_HAL_log(LEVEL_INFO, "Identification started");
    // }
    // else if ((flagIdentification == false) and (oldIdentification == true))
    // {
    //     // Identification ends
    //     // Turn off the LED once the identification ends
    //     hV_HAL_GPIO_write(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    //     hV_HAL_log(LEVEL_INFO, "Identification ended");
    // }
    // oldIdentification = flagIdentification;

    // // Blink the LED while the identification is in progress
    // if (flagIdentification)
    // {
    //     if (hV_HAL_getMilliseconds() % 1000 < 500)
    //     {
    //         hV_HAL_GPIO_write(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    //     }
    //     else
    //     {
    //         hV_HAL_GPIO_write(LED_BUILTIN, LED_BUILTIN_ACTIVE);
    //     }
    // }
}

// --- Menu
uint8_t menuOneButton(uint8_t button, const char * title, const char * option1, const char * option2, const char * option3, const char * option4)
{
    uint8_t result = 0;

    if (hV_HAL_GPIO_read(button) == LOW)
    {
        uint8_t delay_s = 2; // 2 seconds
        uint8_t options_max = 4;

        uint16_t x = myScreen.screenSizeX();
        uint16_t y = myScreen.screenSizeY();

        myScreen.selectFont(fontLarge);
        uint16_t options_dy = myScreen.characterSizeY();
        myScreen.selectFont(fontMedium);
        uint16_t title_dy = myScreen.characterSizeY();

        // From top to bottom
        // area_y0, +2, title_dy, +4, options_dy
        uint16_t area_y0 = y - (options_dy + 4 + title_dy + 2);
        myScreen.selectFont(fontMedium);
        myScreen.setPenSolid(true);
        myScreen.rectangle(0, area_y0, x, y, myColours.white);
        myScreen.dRectangle(0, area_y0, x, 1, myColours.black);
        myScreen.setPenSolid(false);
        myScreen.gText(0, area_y0 + 2, title);

        uint16_t options_dx = x / options_max;
        myScreen.selectFont(fontLarge);
        uint16_t options_y0 = y - options_dy;
        uint16_t bar_y0 = options_y0 - 4;

        uint8_t options_count = 0;
        myScreen.gText(options_count * options_dx, options_y0, option1);
        options_count += 1;

        if (option2 != 0)
        {
            myScreen.gText(options_count * options_dx, options_y0, option2);
            options_count += 1;

            if (option3 != 0)
            {
                myScreen.gText(options_count * options_dx, options_y0, option3);
                options_count += 1;

                if (option4 != 0)
                {
                    myScreen.gText(options_count * options_dx, options_y0, option4);
                    options_count += 1;
                }
            }
        }

        uint8_t options_index = 0;
        uint8_t delay_count = 0;

        uint32_t chronoSwitch = 0;

        while (hV_HAL_GPIO_read(myBoard.button) == LOW)
        {
            if (hV_HAL_getMilliseconds() - chronoSwitch > 1000)
            {
                if (delay_count == delay_s)
                {
                    options_index += 1;
                    options_index %= options_count;
                    delay_count = 0;
                }

                delay_count += 1;
                chronoSwitch = hV_HAL_getMilliseconds();

                myScreen.setPenSolid(true);
                myScreen.dRectangle(0, bar_y0, x, 4, myColours.white);
                myScreen.dRectangle(options_index * options_dx, bar_y0, options_dx * delay_count / delay_s, 2, myColours.black);
                myScreen.setPenSolid(false);
                myScreen.flush();
            }
        }
        result = options_index + 1;

        myScreen.setPenSolid(true);
        myScreen.rectangle(0, area_y0, x, y, myColours.white);
        // myScreen.flush();
    }

    return result;
}
// --- End of Menu

// --- Zigbee
void displayCommissioning()
{
    myScreen.selectFont(fontLarge);
    uint16_t y = 0;
    uint16_t dy = myScreen.characterSizeY();

    myScreen.gText(0, y, ZIGBEE_EXAMPLE_NAME);
    y += dy * 2;
    myScreen.selectFont(fontMedium);

    myZigbee.setVendorName(ZIGBEE_VENDOR_NAME);
    myZigbee.setProductName(ZIGBEE_EXAMPLE_NAME);
    myZigbee.setFirmwareVersion((int)ZIGBEE_EXAMPLE_RELEASE);
    myZigbee.begin();
    myZigbeeTemperature.begin();
    myZigbeeHumidity.begin();

    // uint8_t ieee[8];
    // {
    //     ZigbeeAfLock lock;
    //     memcpy(ieee, sl_zigbee_get_eui64(), sizeof(ieee));
    // }
    //
    // hV_HAL_log(LEVEL_INFO, "IEEE %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", ieee[7], ieee[6], ieee[5], ieee[4], ieee[3], ieee[2], ieee[1], ieee[0]);
    // myScreen.gText(0, y, formatString("IEEE %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", ieee[7], ieee[6], ieee[5], ieee[4], ieee[3], ieee[2], ieee[1], ieee[0]));
    // y += dy;

    if (!myZigbee.isPaired())
    {
        hV_HAL_log(LEVEL_INFO, "Device not commissioned");
        myScreen.gText(0, y, "Device not commissioned");
        y += dy;
        hV_HAL_log(LEVEL_INFO, "Commission it on your Zigbee hub");
        myScreen.gText(0, y, "Commission on Zigbee hub");
        y += dy;
    }
    myScreen.flush();

    while (!myZigbee.isPaired())
    {
        hV_HAL_delayMilliseconds(200);
    }

    hV_HAL_log(LEVEL_INFO, "Connecting to Zigbee network");
    myScreen.gText(0, y, "Connecting to Zigbee network");
    y += dy;
    myScreen.flush();
    while (!myZigbee.isConnectedToNetwork())
    {
        hV_HAL_delayMilliseconds(200);
    }

    hV_HAL_log(LEVEL_INFO, "Connected to Zigbee network");
    myScreen.gText(0, y, "Connected to Zigbee network");
    y += dy;

    hV_HAL_log(LEVEL_INFO, "Channel %i", myZigbee.getChannel());
    myScreen.gText(0, y, formatString("Channel %i", myZigbee.getChannel()));
    y += dy;

    hV_HAL_log(LEVEL_INFO, "PAN ID 0x%x", myZigbee.getPanId());
    myScreen.gText(0, y, formatString("PAN ID 0x%x", myZigbee.getPanId()));
    y += dy;

    hV_HAL_log(LEVEL_INFO, "Node ID 0x%x", myZigbee.getNodeId());
    myScreen.gText(0, y, formatString("Node ID 0x%x", myZigbee.getNodeId()));
    y += dy;

    myScreen.flush();

    hV_HAL_delayMilliseconds(2000);
}

void displayDecommissioning()
{
    myScreen.clear();
    myScreen.selectFont(fontLarge);
    uint16_t y = 0;
    uint16_t dy = myScreen.characterSizeY();

    myScreen.gText(0, y, ZIGBEE_EXAMPLE_NAME);
    y += dy * 2;
    myScreen.flush();
    myScreen.selectFont(fontMedium);

    hV_HAL_log(LEVEL_INFO, "Resetting");
    myScreen.gText(0, y, "Resetting");
    y += dy;
    myScreen.flush();

    if (!myZigbee.isPaired())
    {
        hV_HAL_log(LEVEL_INFO, "Already done");
        myScreen.gText(0, y, ". Already done");
        y += dy;
    }
    else
    {
        hV_HAL_log(LEVEL_INFO, "Factory reset");
        myScreen.gText(0, y, ". Factory reset");
        y += dy;
        Zigbee.factoryReset();
        hV_HAL_log(LEVEL_INFO, "Done");
        myScreen.gText(0, y, ". Done");
        y += dy;
    }
    hV_HAL_log(LEVEL_INFO, "Restart the device");
    myScreen.gText(0, y, "Restart the device");
    myScreen.flush();

    hV_HAL_delayMilliseconds(2000);
}
// --- End of Zigbee

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

    // Check panelPower
    if (myBoard.panelPower == NOT_CONNECTED)
    {
        hV_HAL_log(LEVEL_INFO, "panelPower not connected");
        hV_HAL_exit(RESULT_ERROR);
    }

    // Screen
    myScreen.begin();

    // Fonts
#if (FONT_MODE == USE_FONT_TERMINAL)

    fontSmall = Font_Terminal6x8;
    fontMedium = Font_Terminal8x12;
    fontLarge = Font_Terminal12x16;
    fontVery = Font_Terminal16x24;

#else // FONT_MODE

    fontSmall = myScreen.addFont(Font_Latin_DejaVuSans12);
    fontSmall -= ((fontSmall > 0) ? 1 : 0);
    fontMedium = myScreen.addFont(Font_Latin_DejaVuSans14b);
    fontMedium -= ((fontMedium > 0) ? 1 : 0);
    fontLarge = myScreen.addFont(Font_Latin_DejaVuSans24);
    fontLarge -= ((fontLarge > 0) ? 1 : 0);
    fontVery = myScreen.addFont(Font_Latin_DejaVuMono48);
    fontVery -= ((fontVery > 0) ? 1 : 0);

#endif // FONT_MODE

    // Example
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);
    myScreen.regenerate(); // Clear buffer and screen

    // --- HDC2080
    hV_HAL_Wire_begin();

    bufferWrite[0] = 0x0e;  // HDC20X0_CONFIGURATION
    bufferWrite[1] = 0x00;  // HDC20X0_DEFAULT_SETTINGS
    hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 2, bufferRead, 0);

    bufferWrite[0] = 0x0f;  // HDC20X0_MEASURE_CONFIGURATION
    bufferWrite[1] = 0xfe;  // HDC20X0_MEASURE_SETTINGS
    hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 2, bufferRead, 0);
    // --- End of HDC2080

    // --- Zigbee
    hV_HAL_GPIO_define(myBoard.button, INPUT_PULLUP);
    hV_HAL_delayMilliseconds(100);
    if (hV_HAL_GPIO_read(myBoard.button) == LOW)
    {
        hV_HAL_GPIO_define(LED_BUILTIN, OUTPUT);
        hV_HAL_GPIO_write(LED_BUILTIN, HIGH);
        displayDecommissioning();

        while (true)
        {
            hV_HAL_delayMilliseconds(500);
            hV_HAL_GPIO_write(LED_BUILTIN, 1 - hV_HAL_GPIO_read(LED_BUILTIN));
        }
    }

    displayCommissioning();
    myScreen.clear();
}

///
/// @brief Loop
///
void loop()
{
    // displayIdenfication();

    // --- Menu
    uint8_t action = menuOneButton(myBoard.button, "Weather menu", "Reset", "Exit", "Cancel", "About");

    if (action > 0)
    {
        flagDisplay = true;
        chrono32 = hV_HAL_getMilliseconds();
    }

    switch (action)
    {
        case 1:

            hV_HAL_log(LEVEL_INFO, "> Reset");
            humidity.minimum = humidity.value;
            humidity.maximum = humidity.value;
            temperature.minimum = temperature.value;
            temperature.maximum = temperature.value;

            chrono32 = hV_HAL_getMilliseconds();
            break;

        case 2:

            hV_HAL_log(LEVEL_INFO, "> Exit");
            myScreen.regenerate();
            hV_HAL_exit();
            break;

        case 3:

            hV_HAL_log(LEVEL_INFO, "> Cancel");
            break;

        case 4:

            displayAbout();
            chrono32 = hV_HAL_getMilliseconds();
            break;

        default:

            break;
    }
    // --- End of Menu

    if (hV_HAL_getMilliseconds() > chrono32)
    {
        // HDC2080
        bufferWrite[0] = 0x0f;  // HDC20X0_MEASURE_CONFIGURATION
        bufferWrite[1] = 0xff;  // HDC20X0_DEFAULT_SETTINGS
        hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 2, bufferRead, 0);
        hV_HAL_delayMilliseconds(50);

        // Temperature
        bufferWrite[0] = 0x00;  // HDC20X0_TEMPERATURE_LOW
        hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 1, bufferRead, 2);
        temperature.value = bufferRead[0] + bufferRead[1] * 256.0;
        temperature.value = temperature.value * 165.0 / 65536.0 - 40.0;  // +273.15; // from °C to °K

        // Humidity
        bufferWrite[0] = 0x02;  // HDC20X0_HUMIDITY_LOW
        hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 1, bufferRead, 2);
        humidity.value = bufferRead[0] + bufferRead[1] * 256.0;
        humidity.value = humidity.value / 65536.0 * 100.0;

        // --- Zigbee
        myZigbeeTemperature.set_measured_value_celsius(temperature.value);
        myZigbeeHumidity.set_measured_value_percent(humidity.value);
        countSend += 1;
        // --- End of Zigbee

        // Screen
        countSend %= SEND_BEFORE_FAST;
        if (countSend == 0)
        {

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
        }
        hV_HAL_log(LEVEL_INFO, "countSend = %i", countSend);

        // Serial
        hV_HAL_log(LEVEL_INFO, "Temperature %5.1f < %5.1f < %5.1f oC, Humidity %5.1f < %5.1f < %5.1f %%",
                   temperature.minimum, temperature.value, temperature.maximum,
                   humidity.minimum, humidity.value, humidity.maximum);

        chrono32 = hV_HAL_getMilliseconds() + 1000;  // 10 s
    }

    hV_HAL_delayMilliseconds(100);
}

