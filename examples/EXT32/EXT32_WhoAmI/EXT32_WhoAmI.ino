///
/// @file EXT32_WhoAmI.ino
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
#define DISPLAY_WHOAMI 1

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
#if (DISPLAY_WHOAMI == 1)

void displayLine(uint16_t x, uint16_t y, const char * name, const char * value)
{
    myScreen.selectFont(fontMedium);
    uint16_t tx = myScreen.screenSizeY() / 3;
    uint16_t dx = myScreen.characterSizeY();

    myScreen.gText(x + tx - myScreen.stringSizeX(name), y, name);

    myScreen.selectFont(fontSmall);
    uint16_t dy = myScreen.characterSizeY();
    if (dx > dy)
    {
        dy = (dx - dy) / 2;
    }
    myScreen.gText(x + tx + dx, y + dy, value);
}

///
/// @brief Who am I? test screen
///
/// @image html T2_WHOAMI.jpg
/// @image latex T2_WHOAMI.PDF width=10cm
///
void displayWhoAmI()
{
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);

    uint16_t x = 4;
    uint16_t y = 4;

    myScreen.selectFont(fontLarge);
    myScreen.gText(x, y, "Who Am I");
    myScreen.gText(x + 1, y, "Who Am I");
    y += myScreen.characterSizeY();

    myScreen.selectFont(fontMedium);
    uint16_t dy = myScreen.characterSizeY();

    char memory[64] = {0};
#if (SRAM_MODE == USE_INTERNAL_MCU)
    strcat(memory, "MCU");
#elif (SRAM_MODE == USE_EXTERNAL_SPI)
    strcat(memory, "SPI");
#else
    strcat(memory, "?");
#endif // SRAM_MODE
    strcat(memory, " buffer, ");

#if (FONT_MODE == USE_FONT16_HEADER)
    strcat(memory, "Header16");
#elif (FONT_MODE == USE_FONT_HEADER)
    strcat(memory, "Header");
#elif (FONT_MODE == USE_FONT_FLASH)
    strcat(memory, "Flash");
#elif (FONT_MODE == USE_FONT_TERMINAL)
    strcat(memory, "Terminal");
#else
    strcat(memory, "?");
#endif // FONT_MODE
    strcat(memory, " fonts ");

#if (STRING_MODE == USE_STRING_OBJECT)

    // formatString() requires char * as input
    displayLine(x, y, "Screen", myScreen.WhoAmI().c_str());
    y += dy;
    displayLine(x, y, "Size", formatString("%ix%i", myScreen.screenSizeX(), myScreen.screenSizeY()).c_str());
    y += dy;
    displayLine(x, y, "Number", myScreen.screenNumber().c_str());
    y += dy;
    displayLine(x, y, "Memory", memory);
    y += dy;
    displayLine(x, y, "Driver", myDriver.reference().c_str());
    y += dy;
    displayLine(x, y, "PDLS", myScreen.reference().c_str());
    y += dy;

#elif (STRING_MODE == USE_CHAR_ARRAY)

    displayLine(x, y, "Screen", myScreen.WhoAmI());
    y += dy;
    displayLine(x, y, "Size", formatString("%ix%i", myScreen.screenSizeX(), myScreen.screenSizeY()));
    y += dy;
    displayLine(x, y, "Number", myScreen.screenNumber());
    y += dy;
    displayLine(x, y, "Memory", memory);
    y += dy;
    displayLine(x, y, "Driver", myDriver.reference());
    y += dy;
    displayLine(x, y, "PDLS", myScreen.reference());
    y += dy;

#endif // STRING_MODE

    myScreen.selectFont(fontMedium);
    uint16_t tx = myScreen.screenSizeY() / 3;
    displayLine(x, y, "Colours", "");
    x = x + tx + dy;

    myScreen.setPenSolid(true);
    myScreen.dRectangle(x + dy * 0, y, dy - 1, dy - 1, myColours.black);
    myScreen.setPenSolid(false);
    myScreen.dRectangle(x + dy * 1, y, dy - 1, dy - 1, myColours.black);
    myScreen.setPenSolid(true);

    uint8_t number = myScreen.screenColours();

    if (number >= 3)
    {
        myScreen.dRectangle(x + dy * 2, y, dy - 1, dy - 1, myColours.red);

#if defined(WITH_COLOURS_BWRY)
        if (number == 4)
        {
            myScreen.dRectangle(x + dy * 3, y, dy - 1, dy - 1, myColours.yellow);
        }
#endif // WITH_COLOURS_BWRY
    }

    myScreen.setPenSolid(false);
    myScreen.flushFast();
}

#endif // DISPLAY_WHOAMI

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

    // Screen
    myScreen.setPanelPowerPin(myBoard.panelPower);
    myScreen.begin();

    // Fonts
#if (FONT_MODE == USE_FONT_TERMINAL)

    fontSmall = Font_Terminal6x8;
    fontMedium = Font_Terminal8x12;
    fontLarge = Font_Terminal12x16;
    fontVery = Font_Terminal16x24;

#else // FONT_MODE

    fontSmall = myScreen.addFont(Font16_Latin_DejaVuSans12);
    fontSmall -= ((fontSmall > 0) ? 1 : 0);
    fontMedium = myScreen.addFont(Font16_Latin_DejaVuSans14b);
    fontMedium -= ((fontMedium > 0) ? 1 : 0);
    fontLarge = myScreen.addFont(Font16_Latin_DejaVuSans24);
    fontLarge -= ((fontLarge > 0) ? 1 : 0);
    fontVery = myScreen.addFont(Font16_Latin_DejaVuMono48);
    fontVery -= ((fontVery > 0) ? 1 : 0);

#endif // FONT_MODE

    // Example
#if (DISPLAY_WHOAMI == 1)

    hV_HAL_log(LEVEL_INFO, "DISPLAY_WHOAMI");
    myScreen.clear();
    displayWhoAmI();
    wait(8);

#endif // DISPLAY_WHOAMI

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

