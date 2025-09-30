//
// Screen_EPD.cpp
// Library C++ code
// ----------------------------------
//
// Project Pervasive Displays Library Suite
// Based on highView technology
//
// Created by Rei Vilo, 28 Jun 2016
//
// Copyright (c) Etigues, 2010-2025
// Licence Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
// For exclusive use with Pervasive Displays screens
//
// Release 508: Added support for 969-CS-0B and B98-CS-0B
// Release 511: Improved stability for external SPI SRAM
// Release 529: Added support for pixmap file
// Release 530: Added support for external SRAM
// Release 531: Added getBoardPins
// Release 601: Added support for screens with embedded fast update
// Release 604: Improved optional external power management
// Release 607: Improved screens names consistency
// Release 608: Added screen report
// Release 609: Added temperature management
// Release 610: Removed partial update
// Release 611: Added support for red and yellow colour screens
// Release 700: Refactored screen and board functions
// Release 701: Improved functions names consistency
// Release 702: Added support for 417-QS-0A
// Release 800: Read OTP memory
// Release 801: Improved OTP implementation
// Release 802: Added references to application notes
// Release 802: Refactored CoG functions
// Release 803: Added types for string and frame-buffer
// Release 804: Improved power management
// Release 805: Improved stability
// Release 810: Added support for EXT4
// Release 900: Added new driver library
// Release 901: Added support for screen 340-KS-0G
// Release 902: Improved power management
// Release 904: Added report on frame-buffer
// Release 905: Added support for Wide large screens
// Release 910: Added check on vector coordinates
// Release 912: Added temperature functions to driver
// Release 920: Added check on edition
// Release 921: Added support for BWRY medium and large screens
//

// Library header
#include "Screen_EPD.h"

//
// === Class section
//
Screen_EPD::Screen_EPD(Driver_EPD_Virtual * driver)
{
    s_driver = driver;
    // b_pin = driver->u_board;
    s_newImage = 0; // nullptr
    // COG_data[0] = 0;
}

void Screen_EPD::begin()
{
    // u_eScreen_EPD = eScreen_EPD;
    u_codeSize = SCREEN_SIZE(s_driver->u_eScreen_EPD);
    u_codeFilm = SCREEN_FILM(s_driver->u_eScreen_EPD);
    u_codeDriver = SCREEN_DRIVER(s_driver->u_eScreen_EPD);
    u_codeExtra = SCREEN_EXTRA(s_driver->u_eScreen_EPD);
    v_screenColourBits = 2; // BWR and BWRY

    // Checks
    uint8_t checkCOG = COG_FILM(s_driver->d_COG);

    uint8_t flagCOG = false;
    switch (u_codeFilm)
    {
        case FILM_C: // Standard
        case FILM_H: // Freeze
        case FILM_J: // BWR, "Spectra"
        case FILM_E: // BWR, deprecated
        case FILM_F: // BWR, deprecated
        case FILM_G: // BWY, deprecated

            flagCOG = (checkCOG == FILM_C);
            break;

        case FILM_Q: // BWRY, "Spectra 4"

            flagCOG = (checkCOG == u_codeFilm);
            break;

        case FILM_P: // Embedded fast update
        case FILM_K: // Wide temperature and embedded fast update

            flagCOG = (checkCOG == u_codeFilm);
            flagCOG |= (checkCOG == FILM_T); // Proxy for `P` or `K` with touch
            break;

        default:

            break;
    }

    if (flagCOG == false)
    {
        debugVariant(FILM_Q);
    }

    // Configure board
    s_driver->begin();
    setPowerProfile(POWER_MODE_MANUAL, POWER_SCOPE_NONE);

    // Sizes
    switch (u_codeSize)
    {
        case SIZE_150: // 1.50"
        case SIZE_152: // 1.52"

            v_screenSizeV = 200; // vertical = wide size
            v_screenSizeH = 200; // horizontal = small size
            break;

        case SIZE_154: // 1.54"

            v_screenSizeV = 152; // vertical = wide size
            v_screenSizeH = 152; // horizontal = small size
            break;

        case SIZE_206: // 2.06"

            v_screenSizeV = 248; // vertical = wide size
            v_screenSizeH = 128; // horizontal = small size
            break;

        case SIZE_213: // 2.13"

            v_screenSizeV = 212; // vertical = wide size
            v_screenSizeH = 104; // horizontal = small size
            break;

        case SIZE_266: // 2.66"

            v_screenSizeV = 296; // vertical = wide size
            v_screenSizeH = 152; // horizontal = small size
            break;

        case SIZE_271: // 2.71" and 2.71"-Touch

            v_screenSizeV = 264; // vertical = wide size
            v_screenSizeH = 176; // horizontal = small size
            break;

        case SIZE_287: // 2.87"

            v_screenSizeV = 296; // vertical = wide size
            v_screenSizeH = 128; // horizontal = small size
            break;

        case SIZE_290: // 2.90"

            v_screenSizeV = 384; // vertical = wide size
            v_screenSizeH = 168; // horizontal = small size
            break;

        case SIZE_340: // 3.40"
        case SIZE_343: // 3.43" and 3.43"-Touch

            v_screenSizeV = 392; // vertical = wide size
            v_screenSizeH = 456; // horizontal = small size
            break;

        case SIZE_370: // 3.70" and 3.70"-Touch

            v_screenSizeV = 416; // vertical = wide size
            v_screenSizeH = 240; // horizontal = small size
            break;

        case SIZE_417: // 4.17"

            v_screenSizeV = 300; // vertical = wide size
            v_screenSizeH = 400; // horizontal = small size
            break;

        case SIZE_437: // 4.37"

            v_screenSizeV = 480; // vertical = wide size
            v_screenSizeH = 176; // horizontal = small size
            break;

        case SIZE_565: // 5.65"

            v_screenSizeV = 600; // v = wide size
            v_screenSizeH = 448; // h = small size
            break;

        case SIZE_581: // 5.81"

            v_screenSizeV = 720; // v = wide size
            v_screenSizeH = 256; // h = small size
            break;

        case SIZE_741: // 7.41"

            v_screenSizeV = 800; // v = wide size
            v_screenSizeH = 480; // h = small size
            break;

        case SIZE_969: // 9.69"

            v_screenSizeV = 672; // v = wide size
            v_screenSizeH = 960; // Actually, 960 = 480 x 2, h = small size
            break;

        case SIZE_1198: // 11.98"

            v_screenSizeV = 768; // v = wide size
            v_screenSizeH = 960; // Actually, 960 = 480 x 2, h = small size
            break;

        default:

            hV_HAL_Serial_crlf();
            hV_HAL_log(LEVEL_CRITICAL, "Screen %i-%cS-0%c is not supported", u_codeSize, u_codeFilm, u_codeDriver);
            hV_HAL_exit(RESULT_ERROR);
            break;
    } // u_codeSize
    v_screenDiagonal = u_codeSize;

    //
    // === Large screen section
    //
    // Check panelCSS for large screens
    if (((u_codeSize == SIZE_969) or (u_codeSize == SIZE_1198)) and (s_driver->b_pin.panelCSS == NOT_CONNECTED))
    {
        hV_HAL_log(LEVEL_CRITICAL, "Required pin panelCSS is NOT_CONNECTED");
        hV_HAL_exit(RESULT_ERROR);
    }
    //
    // === End of Large screen section
    //

    // Report
    hV_HAL_log(LEVEL_DEBUG, "Screen %s", WhoAmI().c_str());
    hV_HAL_log(LEVEL_DEBUG, "Size %ix%i", screenSizeX(), screenSizeY());
    hV_HAL_log(LEVEL_DEBUG, "Number %i-%cS-0%c", u_codeSize, u_codeFilm, u_codeDriver);
    hV_HAL_log(LEVEL_DEBUG, "Driver %s", s_driver->reference().c_str());
    hV_HAL_log(LEVEL_DEBUG, "PDLS %s", reference().c_str());
    hV_HAL_Serial_crlf();

    switch (s_driver->d_COG)
    {
        case COG_BWRY_LARGE:
        case COG_BWRY_MEDIUM:
        case COG_BWRY_SMALL:

            u_bufferDepth = 1; // 1 single buffer with 2 bits per pixel
            u_bufferSizeV = v_screenSizeV; // vertical = wide size
            u_bufferSizeH = v_screenSizeH / 4; // horizontal = small size 112 / 4; 2 bits per pixel
            break;

        default:

            u_bufferDepth = 2; // 2 buffers, one per colour ot next/previous
            u_bufferSizeV = v_screenSizeV; // vertical = wide size
            u_bufferSizeH = v_screenSizeH / 8; // horizontal = small size 112 / 8; 1 bit per pixel
            break;
    }

    // Force conversion for two unit16_t multiplication into uint32_t.
    // Actually for 1 colour; BWR requires 2 pages.
    u_pageColourSize = (uint32_t)u_bufferSizeV * (uint32_t)u_bufferSizeH;

    //
    // Specific SRAM section
    //
#if defined(BOARD_HAS_PSRAM) // ESP32 PSRAM specific case

    if (s_newImage == 0)
    {
        hV_HAL_log(LEVEL_DEBUG, "Create frame-buffer [%i]", u_bufferDepth * u_pageColourSize);

        static uint8_t * _newFrameBuffer;
        _newFrameBuffer = (uint8_t *) ps_malloc(u_pageColourSize * u_bufferDepth);
        s_newImage = (uint8_t *) _newFrameBuffer;
    }

#else // default case

    if (s_newImage == 0)
    {
        static uint8_t * _newFrameBuffer;
        _newFrameBuffer = new uint8_t[u_pageColourSize * u_bufferDepth];
        s_newImage = (uint8_t *) _newFrameBuffer;
    }

#endif // ESP32 BOARD_HAS_PSRAM
    //
    // End of Specific SRAM section
    //

    memset(s_newImage, 0x00, u_pageColourSize * u_bufferDepth);

    setTemperatureC(25); // 25 Celsius = 77 Fahrenheit

    // // Report
    // hV_HAL_log(LEVEL_DEBUG, "%24s = %6i", "u_pageColourSize * u_bufferDepth", u_pageColourSize * u_bufferDepth);
    // hV_HAL_log(LEVEL_DEBUG, "%24s = %6i", "u_pageColourSize", u_pageColourSize);
    // hV_HAL_log(LEVEL_DEBUG, "%24s = %6i", "v_screenSizeV", v_screenSizeV);
    // hV_HAL_log(LEVEL_DEBUG, "%24s = %6i", "v_screenSizeH", v_screenSizeH);
    // hV_HAL_log(LEVEL_DEBUG, "%24s = %6i", "u_bufferSizeV", u_bufferSizeV);
    // hV_HAL_log(LEVEL_DEBUG, "%24s = %6i", "u_bufferSizeH", u_bufferSizeH);
    // hV_HAL_log(LEVEL_DEBUG, "%24s = %6i", "u_bufferDepth", u_bufferDepth);

    // Fonts
    hV_Screen_Buffer::begin(); // Standard

    if (f_fontMax() > 0)
    {
        f_selectFont(0);
    }
    f_fontSolid = false;

    // Orientation
    setOrientation(0);

    v_penSolid = false;

    //
    // === Touch section
    //
    v_touchTrim = 0x00; // no touch
    v_touchEvent = false; // no touch event

    v_touchTrim = 0x10; // standard threshold
    v_touchEvent = true;

    // v_touchXmax and v_touchYmax hard-coded
    v_touchXmin = 0;
    v_touchYmin = 0;

    if (SCREEN_EXTRA(s_driver->u_eScreen_EPD) & EXTRA_TOUCH != EXTRA_TOUCH)
    {
        hV_HAL_Serial_crlf();
        hV_HAL_log(LEVEL_CRITICAL, "Screen %i-%cS-0%c does not have touch", u_codeSize, u_codeFilm, u_codeDriver);
        hV_HAL_exit(RESULT_ERROR);;
    }

    switch (SCREEN_SIZE(s_driver->u_eScreen_EPD))
    {
        case SIZE_271:

            v_touchXmax = 176; // Xmax
            v_touchYmax = 264; // Ymax
            break;

        case SIZE_343:

            v_touchXmax = 455; // Xmax, hardware hard-coded in controller
            v_touchYmax = 391; // Ymax, hardware hard-coded in controller
            break;

        case SIZE_370:

            v_touchXmax = 239; // Xmax, hardware hard-coded in controller
            v_touchYmax = 415; // Ymax, hardware hard-coded in controller
            break;

        default:

            break;
    }
    //
    // === End of Touch section
    //
}

void Screen_EPD::clear(uint16_t colour)
{
    uint8_t pattern;

    switch (u_codeFilm)
    {
        case FILM_Q: // BWRY, "Spectra 4"

            if (colour == myColours.grey)
            {
                // black = 0-1, white = 0-0
                for (uint16_t i = 0; i < u_bufferSizeV; i++)
                {
                    pattern = (i % 2) ? 0b01000100 : 0b00010001; // black-white : white-black
                    for (uint16_t j = 0; j < u_bufferSizeH; j++)
                    {
                        s_newImage[i * u_bufferSizeH + j] = pattern;
                    }
                }
            }
            else if (colour == myColours.white)
            {
                // physical black = 0-1
                memset(s_newImage, 0b01010101, u_pageColourSize);
            }
            else if (colour == myColours.black)
            {
                // physical white = 0-0
                memset(s_newImage, 0b00000000, u_pageColourSize);
            }
            else if (colour == myColours.red)
            {
                // physical red = 1-1
                memset(s_newImage, 0b11111111, u_pageColourSize);
            }
            else if (colour == myColours.darkRed)
            {
                // red = 1-1, black = 0-1
                for (uint16_t i = 0; i < u_bufferSizeV; i++)
                {
                    uint8_t pattern = (i % 2) ? 0b11011101 : 0b01110111; // red-black : black-red
                    for (uint16_t j = 0; j < u_bufferSizeH; j++)
                    {
                        s_newImage[i * u_bufferSizeH + j] = pattern;
                    }
                }
            }
            else if (colour == myColours.lightRed)
            {
                // red = 1-1, white = 0-0
                for (uint16_t i = 0; i < u_bufferSizeV; i++)
                {
                    uint8_t pattern = (i % 2) ? 0b11001100 : 0b00110011; // red-white : white-red
                    for (uint16_t j = 0; j < u_bufferSizeH; j++)
                    {
                        s_newImage[i * u_bufferSizeH + j] = pattern;
                    }
                }
            }
            else if (colour == myColours.yellow)
            {
                // physical yellow = 1-0
                memset(s_newImage, 0b10101010, u_pageColourSize);
            }
            else if (colour == myColours.darkYellow)
            {
                // yellow = 1-0, black = 0-1
                for (uint16_t i = 0; i < u_bufferSizeV; i++)
                {
                    uint8_t pattern = (i % 2) ? 0b10011001 : 0b01100110; // yellow-black : black-yellow
                    for (uint16_t j = 0; j < u_bufferSizeH; j++)
                    {
                        s_newImage[i * u_bufferSizeH + j] = pattern;
                    }
                }
            }
            else if (colour == myColours.lightYellow)
            {
                // yellow = 1-0, white = 0-0
                for (uint16_t i = 0; i < u_bufferSizeV; i++)
                {
                    uint8_t pattern = (i % 2) ? 0b10001000 : 0b00100010; // yellow-white : white-yellow
                    for (uint16_t j = 0; j < u_bufferSizeH; j++)
                    {
                        s_newImage[i * u_bufferSizeH + j] = pattern;
                    }
                }
            }
            else if (colour == myColours.orange)
            {
                // yellow = 1-0, red = 1-1
                for (uint16_t i = 0; i < u_bufferSizeV; i++)
                {
                    uint8_t pattern = (i % 2) ? 0b10111011 : 0b11101110; // yellow-red : red-yellow
                    for (uint16_t j = 0; j < u_bufferSizeH; j++)
                    {
                        s_newImage[i * u_bufferSizeH + j] = pattern;
                    }
                }
            }
            break;

        case FILM_K: // Wide temperature and rmbedded fast update
        case FILM_P: // Embedded fast update

            if (colour == myColours.grey)
            {
                // black = 0-1, white = 0-0
                for (uint16_t i = 0; i < u_bufferSizeV; i++)
                {
                    uint8_t pattern = (i % 2) ? 0b10101010 : 0b01010101;
                    for (uint16_t j = 0; j < u_bufferSizeH; j++)
                    {
                        s_newImage[i * u_bufferSizeH + j] = pattern;
                    }
                }
            }
            else if (colour == myColours.white)
            {
                // physical black 0-0
                memset(s_newImage, 0x00, u_pageColourSize);
            }
            else
            {
                // physical white 1-0
                memset(s_newImage, 0xff, u_pageColourSize);
            }
            break;

        default: // Normal update and deprecated

            if (colour == myColours.red)
            {
                // physical red 0-1
                memset(s_newImage, 0x00, u_pageColourSize);
                memset(s_newImage + u_pageColourSize, 0xff, u_pageColourSize);
            }
            else if (colour == myColours.grey)
            {
                for (uint16_t i = 0; i < u_bufferSizeV; i++)
                {
                    uint8_t pattern = (i % 2) ? 0b10101010 : 0b01010101;
                    for (uint16_t j = 0; j < u_bufferSizeH; j++)
                    {
                        s_newImage[i * u_bufferSizeH + j] = pattern;
                    }
                }
                memset(s_newImage + u_pageColourSize, 0x00, u_pageColourSize);
            }
            else if (colour == myColours.darkRed)
            {
                // red = 0-1, black = 1-0, white 0-0
                for (uint16_t i = 0; i < u_bufferSizeV; i++)
                {
                    uint16_t pattern1 = (i % 2) ? 0b10101010 : 0b01010101; // black
                    uint16_t pattern2 = (i % 2) ? 0b01010101 : 0b10101010; // red
                    for (uint16_t j = 0; j < u_bufferSizeH; j++)
                    {
                        s_newImage[i * u_bufferSizeH + j] = pattern1;
                        s_newImage[i * u_bufferSizeH + j + u_pageColourSize] = pattern2;
                    }
                }
            }
            else if (colour == myColours.lightRed)
            {
                // red = 0-1, black = 1-0, white 0-0
                for (uint16_t i = 0; i < u_bufferSizeV; i++)
                {
                    uint16_t pattern1 = (i % 2) ? 0b00000000 : 0b00000000; // white
                    uint16_t pattern2 = (i % 2) ? 0b01010101 : 0b10101010; // red
                    for (uint16_t j = 0; j < u_bufferSizeH; j++)
                    {
                        s_newImage[i * u_bufferSizeH + j] = pattern1;
                        s_newImage[i * u_bufferSizeH + j + u_pageColourSize] = pattern2;
                    }
                }
            }
            else if (colour == myColours.white)
            {
                // physical black 0-0
                memset(s_newImage, 0x00, u_pageColourSize);
                memset(s_newImage + u_pageColourSize, 0x00, u_pageColourSize);
            }
            else
            {
                // physical white 1-0
                memset(s_newImage, 0xff, u_pageColourSize);
                memset(s_newImage + u_pageColourSize, 0x00, u_pageColourSize);
            }
            break;
    }
}

void Screen_EPD::flush()
{
    resume(); // GPIO

    if ((u_codeSize == SIZE_969) or (u_codeSize == SIZE_B98)) // Large
    {
        // 9.69 and 11.98 combine two half-screens, hence two frames with adjusted (u_pageColourSize >> 1) size
        uint32_t u_subPageColourSize = (u_pageColourSize >> 1);

        FRAMEBUFFER_TYPE nextBuffer = s_newImage; // size = u_pageColourSize
        FRAMEBUFFER_TYPE previousBuffer = s_newImage + u_pageColourSize; // size = u_pageColourSize

        // FRAMEBUFFER_TYPE frameM1 = nextBuffer;
        // FRAMEBUFFER_TYPE frameM2 = previousBuffer;
        // FRAMEBUFFER_TYPE frameS1 = nextBuffer + u_subPageColourSize;
        // FRAMEBUFFER_TYPE frameS2 = previousBuffer + u_subPageColourSize;

        FRAMEBUFFER_TYPE blackBuffer = s_newImage;
        FRAMEBUFFER_TYPE redBuffer = s_newImage + u_pageColourSize;
        FRAMEBUFFER_TYPE frameM1 = blackBuffer;
        FRAMEBUFFER_TYPE frameM2 = redBuffer;
        FRAMEBUFFER_TYPE frameS1 = blackBuffer + u_subPageColourSize;
        FRAMEBUFFER_TYPE frameS2 = redBuffer + u_subPageColourSize;

        switch (u_codeFilm)
        {
            case FILM_Q: // BWRY, "Spectra 4"

                s_driver->updateNormal(frameM1, frameS1, u_subPageColourSize);
                break;

            case FILM_K: // Wide temperature and embedded fast update
            case FILM_P: // Embedded fast update

                s_driver->updateFast(frameM1, frameM2, frameS1, frameS2, u_subPageColourSize);
                memcpy(previousBuffer, nextBuffer, u_pageColourSize); // Copy displayed next to previous
                break;

            default:

                s_driver->updateNormal(frameM1, frameM2, frameS1, frameS2, u_subPageColourSize);
                break;
        }
    }
    else // Small and medium
    {
        FRAMEBUFFER_TYPE nextBuffer = s_newImage;
        FRAMEBUFFER_TYPE previousBuffer = s_newImage + u_pageColourSize;
        FRAMEBUFFER_TYPE blackBuffer = s_newImage;
        FRAMEBUFFER_TYPE redBuffer = s_newImage + u_pageColourSize;

        switch (u_codeFilm)
        {
            case FILM_Q:

                s_driver->updateNormal(s_newImage, u_pageColourSize);
                break;

            case FILM_K: // Wide temperature and rmbedded fast update
            case FILM_P: // Embedded fast update

                s_driver->updateFast(s_newImage, s_newImage + u_pageColourSize, u_pageColourSize);
                memcpy(previousBuffer, nextBuffer, u_pageColourSize); // Copy displayed next to previous
                break;

            default:

                s_driver->updateNormal(s_newImage, s_newImage + u_pageColourSize, u_pageColourSize);
                break;
        }
    }

    if (u_suspendMode == POWER_MODE_AUTO)
    {
        suspend(u_suspendScope); // GPIO
    }
}

void Screen_EPD::flushFast()
{
    flush();
}

void Screen_EPD::regenerate(uint8_t mode)
{
    switch (u_codeFilm)
    {
        case FILM_K: // Wide temperature and embedded fast update
        case FILM_P: // Embedded fast update

            clear(myColours.black);
            flushFast();
            hV_HAL_delayMilliseconds(100);

            clear(myColours.white);
            flushFast();
            hV_HAL_delayMilliseconds(100);
            break;

        default:

            hV_HAL_delayMilliseconds(100);
            clear();
            flush();
            break;
    }
}

//
// Protected section
//
void Screen_EPD::s_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    // Orient and check coordinates are within screen
    if (s_orientCoordinates(x1, y1) == RESULT_ERROR)
    {
        return;
    }

    // Convert combined colours into basic colours
    bool flagOdd = ((x1 + y1) % 2 == 0);

    // Coordinates
    uint32_t z1 = s_getZ(x1, y1);
    uint16_t b1 = s_getB(x1, y1);

    switch (u_codeFilm)
    {
        case FILM_Q: // BWRY, "Spectra 4"

            // Combined colours
            if (colour == myColours.grey)
            {
                if (flagOdd)
                {
                    colour = myColours.black; // black
                }
                else
                {
                    colour = myColours.white; // white
                }
            }
            else if (colour == myColours.darkRed)
            {
                if (flagOdd)
                {
                    colour = myColours.red; // red
                }
                else
                {
                    colour = myColours.black; // white
                }
            }
            else if (colour == myColours.lightRed)
            {
                if (flagOdd)
                {
                    colour = myColours.red; // red
                }
                else
                {
                    colour = myColours.white; // black
                }
            }
            else if (colour == myColours.darkYellow)
            {
                if (flagOdd)
                {
                    colour = myColours.yellow; // yellow
                }
                else
                {
                    colour = myColours.black; // black
                }
            }
            else if (colour == myColours.lightYellow)
            {
                if (flagOdd)
                {
                    colour = myColours.yellow; // yellow
                }
                else
                {
                    colour = myColours.white; // white
                }
            }
            else if (colour == myColours.orange)
            {
                if (flagOdd)
                {
                    colour = myColours.yellow; // yellow
                }
                else
                {
                    colour = myColours.red; // red
                }
            }

            // Basic colours
            if (colour == myColours.black)
            {
                // physical white = 0-0
                bitClear(s_newImage[z1], b1 + 1); // MSB
                bitClear(s_newImage[z1], b1);
            }
            else if (colour == myColours.white)
            {
                // physical black = 0-1
                bitClear(s_newImage[z1], b1 + 1); // MSB
                bitSet(s_newImage[z1], b1);
            }
            else if (colour == myColours.yellow)
            {
                // physical yellow = 1-0
                bitSet(s_newImage[z1], b1 + 1); // MSB
                bitClear(s_newImage[z1], b1);
            }
            else if (colour == myColours.red)
            {
                // physical red = 1-1
                bitSet(s_newImage[z1], b1 + 1); // MSB
                bitSet(s_newImage[z1], b1);
            }
            break;

        case FILM_K: // Wide temperature and rmbedded fast update
        case FILM_P: // Embedded fast update

            // Combined colours
            if (colour == myColours.grey)
            {
                if (flagOdd)
                {
                    colour = myColours.black; // black
                }
                else
                {
                    colour = myColours.white; // white
                }
            }

            // Basic colours
            if (colour == myColours.white)
            {
                // physical black 0-0
                bitClear(s_newImage[z1], b1);
            }
            else if (colour == myColours.black)
            {
                // physical white 1-0
                bitSet(s_newImage[z1], b1);
            }
            break;

        default:

            // Combined colours
            if (colour == myColours.darkRed)
            {
                if (flagOdd)
                {
                    colour = myColours.red; // red
                }
                else
                {
                    colour = myColours.black; // white
                }
            }
            else if (colour == myColours.lightRed)
            {
                if (flagOdd)
                {
                    colour = myColours.red; // red
                }
                else
                {
                    colour = myColours.white; // black
                }
            }
            else if (colour == myColours.grey)
            {
                if (flagOdd)
                {
                    colour = myColours.black; // black
                }
                else
                {
                    colour = myColours.white; // white
                }
            }

            // Basic colours
            if (colour == myColours.red)
            {
                // physical red 0-1
                bitClear(s_newImage[z1], b1);
                bitSet(s_newImage[u_pageColourSize + z1], b1);
            }
            else if (colour == myColours.white)
            {
                // physical black 0-0
                bitClear(s_newImage[z1], b1);
                bitClear(s_newImage[u_pageColourSize + z1], b1);
            }
            else if (colour == myColours.black)
            {
                // physical white 1-0
                bitSet(s_newImage[z1], b1);
                bitClear(s_newImage[u_pageColourSize + z1], b1);
            }
            break;
    }
}

void Screen_EPD::s_setOrientation(uint8_t orientation)
{
    v_orientation = orientation % 4;
}

bool Screen_EPD::s_orientCoordinates(uint16_t & x, uint16_t & y)
{
    bool _flagResult = RESULT_ERROR;

    switch (v_orientation)
    {
        case 3: // checked, previously 1

            if ((x < v_screenSizeV) and (y < v_screenSizeH))
            {
                x = v_screenSizeV - 1 - x;
                _flagResult = RESULT_SUCCESS;
            }
            break;

        case 2: // checked

            if ((x < v_screenSizeH) and (y < v_screenSizeV))
            {
                x = v_screenSizeH - 1 - x;
                y = v_screenSizeV - 1 - y;
                hV_HAL_swap(x, y);
                _flagResult = RESULT_SUCCESS;
            }
            break;

        case 1: // checked, previously 3

            if ((x < v_screenSizeV) and (y < v_screenSizeH))
            {
                y = v_screenSizeH - 1 - y;
                _flagResult = RESULT_SUCCESS;
            }
            break;

        default: // checked

            if ((x < v_screenSizeH) and (y < v_screenSizeV))
            {
                hV_HAL_swap(x, y);
                _flagResult = RESULT_SUCCESS;
            }
            break;
    }

    return _flagResult;
}

uint32_t Screen_EPD::s_getZ(uint16_t x1, uint16_t y1)
{
    uint32_t z1 = 0;

    switch (s_driver->d_COG)
    {
        case COG_BWRY_LARGE:

            if (y1 >= (v_screenSizeH >> 1))
            {
                y1 -= (v_screenSizeH >> 1); // rebase y1
                z1 += (u_pageColourSize >> 1); // buffer second half
            }
            z1 += (uint32_t)x1 * (u_bufferSizeH >> 1) + (y1 >> 2); // 4 pixels per byte
            break;

        case COG_BWRY_MEDIUM:
        case COG_BWRY_SMALL:

            z1 = (uint32_t)x1 * u_bufferSizeH + (y1 >> 2); // 4 pixels per byte

            break;

        case COG_FAST_LARGE:
        case COG_WIDE_LARGE:
        case COG_NORMAL_LARGE:

            if (y1 >= (v_screenSizeH >> 1))
            {
                y1 -= (v_screenSizeH >> 1); // rebase y1
                z1 += (u_pageColourSize >> 1); // buffer second half
            }
            z1 += (uint32_t)x1 * (u_bufferSizeH >> 1) + (y1 >> 3);
            break;

        default:

            z1 = (uint32_t)x1 * u_bufferSizeH + (y1 >> 3); // 8 pixels per byte 
            break;
    }

    return z1;
}

uint16_t Screen_EPD::s_getB(uint16_t x1, uint16_t y1)
{
    uint16_t b1 = 0;

    switch (s_driver->d_COG)
    {
        case COG_BWRY_LARGE:
        case COG_BWRY_MEDIUM:
        case COG_BWRY_SMALL:

            b1 = 6 - 2 * (y1 % 4); // 4 pixels per byte
            break;

        default:

            b1 = 7 - (y1 % 8);
            break;
    }
    return b1;
}

uint16_t Screen_EPD::s_getPoint(uint16_t x1, uint16_t y1)
{
    return 0x0000;
}
//
// === End of Protected section
//

//
// === Power section
//
void Screen_EPD::setPanelPowerPin(uint8_t panelPowerPin)
{
    s_driver->b_pin.panelPower = panelPowerPin;
    if (s_driver->b_pin.flashCS == panelPowerPin)
    {
        s_driver->b_pin.flashCS = NOT_CONNECTED;
    }
}

void Screen_EPD::setPowerProfile(uint8_t mode, uint8_t scope)
{
    u_suspendMode = mode;
    u_suspendScope = POWER_SCOPE_GPIO_ONLY;

    if (s_driver->b_pin.panelPower == NOT_CONNECTED)
    {
        u_suspendMode = POWER_MODE_MANUAL;
        u_suspendScope = POWER_SCOPE_NONE;
    }
}

void Screen_EPD::suspend(uint8_t suspendScope)
{
    // s_driver->b_suspend(); // GPIO
    if (s_driver->b_pin.panelPower != NOT_CONNECTED)
    {
        if ((suspendScope & FSM_GPIO_MASK) == FSM_GPIO_MASK)
        {
            if ((s_driver->b_fsmPowerScreen & FSM_GPIO_MASK) == FSM_GPIO_MASK)
            {
                s_driver->b_suspend(); // GPIO
                s_driver->b_fsmPowerScreen &= ~FSM_GPIO_MASK;
            }
        }
    }
}

void Screen_EPD::resume()
{
    s_driver->b_resume(); // GPIO
    s_driver->b_fsmPowerScreen |= FSM_GPIO_MASK;
}
//
// === End of Power section
//

//
// === Temperature section
//
void Screen_EPD::setTemperatureC(int8_t temperatureC)
{
    // s_driver->u_temperature = temperatureC;
    s_driver->setTemperatureC(temperatureC);
}

void Screen_EPD::setTemperatureF(int16_t temperatureF)
{
    // int8_t temperatureC = ((temperatureF - 32) * 5) / 9; // C = (F - 32) * 5 / 9
    // setTemperatureC(temperatureC);
    s_driver->setTemperatureF(temperatureF);
}

uint8_t Screen_EPD::checkTemperatureMode(uint8_t updateMode)
{
    switch (u_codeFilm)
    {
        case FILM_P: // Embedded fast update

            // Fast 	PS 	Embedded fast update 	FU: +15 to +30 °C 	GU: 0 to +50 °C
            if (updateMode == UPDATE_FAST) // Fast update
            {
                if ((s_driver->u_temperature < 15) or (s_driver->u_temperature > 30))
                {
                    updateMode = UPDATE_NORMAL;
                }
            }
            if (updateMode == UPDATE_NORMAL) // Normal update
            {
                if ((s_driver->u_temperature < 0) or (s_driver->u_temperature > 50))
                {
                    updateMode = UPDATE_NONE;
                }
            }
            break;

        case FILM_K: // Wide temperature and embedded fast update

            // Wide 	KS 	Wide temperature and embedded fast update 	FU: 0 to +50 °C 	GU: -15 to +60 °C
            if (updateMode == UPDATE_FAST) // Fast update
            {
                if ((s_driver->u_temperature < 0) or (s_driver->u_temperature > 50))
                {
                    updateMode = UPDATE_NORMAL;
                }
            }
            if (updateMode == UPDATE_NORMAL) // Normal update
            {
                if ((s_driver->u_temperature < -15) or (s_driver->u_temperature > 60))
                {
                    updateMode = UPDATE_NONE;
                }
            }
            break;

        case FILM_H: // HS series

            // Freezer 	HS 	Normal update below 0 °C 	FU: - 	GU: -25 to +30 °C
            updateMode = UPDATE_NORMAL;
            if ((s_driver->u_temperature < -25) or (s_driver->u_temperature > 30))
            {
                updateMode = UPDATE_NONE;
            }
            break;

        case FILM_J: // BWR, "Spectra"
        case FILM_E: // BWR, deprecated
        case FILM_F: // BWR, deprecated
        case FILM_G: // BWY, deprecated
        case FILM_Q: // BWRY, "Spectra 4"

            // BWR  JS 	Red colour 	FU: - 	GU: 0 to +40 °C
            // BWRY  QS 	Red and yellow colours 	FU: - 	GU: 0 to +40 °C
            if ((s_driver->u_temperature < 0) or (s_driver->u_temperature > 40))
            {
                updateMode = UPDATE_NONE;
            }
            break;

        default: // Standard

            // Normal 	CS 	Normal update above 0 °C 	FU: - 	GU: 0 to +50 °C
            updateMode = UPDATE_NORMAL;
            if ((s_driver->u_temperature < 0) or (s_driver->u_temperature > 50))
            {
                updateMode = UPDATE_NONE;
            }
            break;
    }

    return updateMode;
}
//
// === End of Temperature section
//

//
// === Miscellaneous section
//
STRING_TYPE Screen_EPD::WhoAmI()
{
    char work[64] = {0};

    switch (u_codeFilm)
    {
        case FILM_P: // Embedded fast update

            strcat(work, "-Fast");
            break;

        case FILM_K: // Wide temperature and embedded fast update

            strcat(work, "-Wide");
            break;

        case FILM_H: // Freezer

            strcat(work, "-Freezer");
            break;

        case FILM_J: // BWR, "Spectra"
        case FILM_E: // BWR, deprecated
        case FILM_F: // BWR, deprecated

            strcat(work, "-BWR");
            break;

        case FILM_G: // BWY, deprecated

            strcat(work, "-BWY");
            break;

        case FILM_Q: // BWRY, "Spectra 4"

            strcat(work, "-BWRY");
            break;

        case FILM_C: ///< Film C, Standard

            strcat(work, "-BW");
            break;

        default: // Unknwon

            strcat(work, "-?");
            break;
    }

    switch (u_codeExtra)
    {
        case EXTRA_TOUCH:

            strcat(work, "+Touch");
            break;

        case EXTRA_DEMO:

            strcat(work, "+Demo");
            break;

        default:

            break;
    }

    strcat(work, " ");

#ifndef SRAM_MODE
    strcat(work, "i");
#elif (SRAM_MODE == USE_INTERNAL_MCU)
    strcat(work, "i");
#elif (SRAM_MODE == USE_EXTERNAL_SPI)
    strcat(work, "e");
#else
    strcat(work, ".");
#endif // SRAM_MODE

#if (FONT_MODE == USE_FONT_HEADER)
    strcat(work, "H");
#elif (FONT_MODE == USE_FONT_FLASH)
    strcat(work, "F");
#elif (FONT_MODE == USE_FONT_TERMINAL)
    strcat(work, "T");
#else
    strcat(work, ".");
#endif // FONT_MODE

    return formatString("iTC %i.%02i\"%s", v_screenDiagonal / 100, v_screenDiagonal % 100, work);
}

STRING_CONST_TYPE Screen_EPD::reference()
{
    return formatString("%s v%i.%i.%i", SCREEN_EPD_VARIANT, SCREEN_EPD_RELEASE / 100, (SCREEN_EPD_RELEASE / 10) % 10, SCREEN_EPD_RELEASE % 10);
}

uint8_t Screen_EPD::screenColours()
{
    uint8_t result;

    switch (u_codeFilm)
    {
        case FILM_C: // Standard
        case FILM_P: // Embedded fast update
        case FILM_K: // Wide temperature and embedded fast update
        case FILM_H: // Freeze

            result = 2;
            break;

        case FILM_J: // BWR, "Spectra"
        case FILM_E: // BWR, deprecated
        case FILM_F: // BWR, deprecated
        case FILM_G: // BWY, deprecated

            result = 3;
            break;

        case FILM_Q: // BWRY, "Spectra 4"

            result = 4;
            break;

        default:

            result = 0; // Error
            break;
    }

    return result;
}

STRING_TYPE Screen_EPD::screenNumber()
{
    char work[64] = {0};

    sprintf(work, "%i-%cS-0%c", u_codeSize, u_codeFilm, u_codeDriver);

    switch (u_codeExtra)
    {
        case EXTRA_TOUCH:

            strcat(work, "-Touch");
            break;

        case EXTRA_DEMO:

            strcat(work, "-Demo");
            break;

        default:

            break;
    }

    return formatString("%s", work);
}

void Screen_EPD::debugVariant(uint8_t contextFilm)
{
    hV_HAL_Serial_crlf();

    switch (contextFilm)
    {
        case FILM_P: // BW, Embedded fast update

            hV_HAL_log(LEVEL_CRITICAL, "Screen %i-%cS-0%c with no fast update", u_codeSize, u_codeFilm, u_codeDriver);
            break;

        case FILM_K: // BW, wide temperature and embedded fast update

            hV_HAL_log(LEVEL_CRITICAL, "Screen %i-%cS-0%c with no wide temperature and embedded fast update", u_codeSize, u_codeFilm, u_codeDriver);
            break;

        case FILM_Q: // BWRY, "Spectra 4"

            hV_HAL_log(LEVEL_CRITICAL, "Screen %i-%cS-0%c is not black-white-red-yellow", u_codeSize, u_codeFilm, u_codeDriver);
            break;

        default:

            hV_HAL_log(LEVEL_CRITICAL, ", Screen %i-%cS-0%c is not supported", u_codeSize, u_codeFilm, u_codeDriver);
            break;
    } // u_codeFilm

    switch (u_codeFilm)
    {
        case FILM_P: // BW, Embedded fast update

            hV_HAL_log(LEVEL_CRITICAL, "Use PDLS_EXT3_%s_%s instead", "Basic", "Fast");
            break;

        case FILM_K: // BW, Wide temperature and embedded fast update

            hV_HAL_log(LEVEL_CRITICAL, "Use PDLS_EXT3_%s_%s instead", "Basic", "Wide");
            break;

        case FILM_Q: // BWRY, "Spectra 4"

            hV_HAL_log(LEVEL_CRITICAL, "Use PDLS_EXT3_%s_%s instead", "Basic", "BWRY");
            break;

        default: // Normal update and deprecated

            hV_HAL_log(LEVEL_CRITICAL, "Use PDLS_EXT3_%s_%s instead", "Basic", "Normal");
            break;
    } // u_codeFilm

    hV_HAL_Serial_crlf();
    while (0x01);
}
//
// === End of Miscellaneous section
//

//
// === Touch section
//
void Screen_EPD::s_getRawTouch(touch_t & touch)
{
    s_driver->d_getRawTouch(touch);
}

bool Screen_EPD::s_getInterruptTouch()
{
    return s_driver->d_getInterruptTouch();
};
//
// === End of Touch section
//

