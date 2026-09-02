//
// hV_Screen_Buffer.cpp
// Class library C++ code
// ----------------------------------
//
// Project Pervasive Displays Library Suite
// Based on highView technology
//
// Created by Rei Vilo, 28 Jun 2016
//
// Copyright (c) Pervasive Displays Inc., 2021-2026
// Copyright (c) Etigues, 2010-2026
// Licence Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
// For exclusive use with Pervasive Displays screens
//
// See hV_Screen_Buffer.h for references
//
// Release 520: Added use of hV_HAL_Peripherals
// Release 523: Fixed rounded rectangles
// Release 526: Improved touch management
// Release 700: Refactored screen and board functions
// Release 703: Improved orientation function
// Release 801: Improved functions names consistency
// Release 805: Added large variant for gText()
// Release 910: Added check on vector coordinates
// Release 1000: Added support for UTF-8 strings
// Release 1010: Improved circular graphic primitives
//

// Library header
#include "hV_Screen_Buffer.h"

// Code
hV_Screen_Buffer::hV_Screen_Buffer()
{
    f_fontIndex = 0;
    f_fontNumber = 0;
    f_fontSolid = true; // default
    f_fontSpaceX = 0; // Basic edition, font Terminal
    f_fontSpaceY = 0; // Basic edition, font Terminal
    v_penSolid = false; // default
}

void hV_Screen_Buffer::begin()
{
    f_begin(); // hV_Font_...
}

void hV_Screen_Buffer::clear(uint16_t colour)
{
    uint8_t oldOrientation = v_orientation;
    bool oldPenSolid = v_penSolid;
    setOrientation(0);
    setPenSolid();
    rectangle(0, 0, screenSizeX() - 1, screenSizeY() - 1, colour);
    setOrientation(oldOrientation);
    setPenSolid(oldPenSolid);
}

void hV_Screen_Buffer::flush()
{
    ;
}

void hV_Screen_Buffer::setOrientation(uint8_t orientation)
{
    switch (orientation)
    {
        case ORIENTATION_PORTRAIT:

            s_setOrientation(0);
            if (screenSizeX() > screenSizeY())
            {
                s_setOrientation(1);
            }
            break;

        case ORIENTATION_LANDSCAPE:

            s_setOrientation(2);
            if (screenSizeX() < screenSizeY())
            {
                s_setOrientation(3);
            }
            break;

        default:

            s_setOrientation(orientation);
            break;
    }
}

uint8_t hV_Screen_Buffer::getOrientation()
{
    return v_orientation;
}

uint16_t hV_Screen_Buffer::screenSizeX()
{
    switch (v_orientation)
    {
        case 1:
        case 3:

            return v_screenSizeV; // _maxX
            break;

        // case 0:
        // case 2:
        default:

            return v_screenSizeH; // _maxX
            break;
    }
    return 0;
}

uint16_t hV_Screen_Buffer::screenSizeY()
{
    switch (v_orientation)
    {
        case 1:
        case 3:

            return v_screenSizeH; // _maxY
            break;

        // case 0:
        // case 2:
        default:

            return v_screenSizeV; // _maxY
            break;
    }
    return 0;
}

uint16_t hV_Screen_Buffer::screenDiagonal()
{
    return v_screenDiagonal;
}

uint8_t hV_Screen_Buffer::screenColourBits()
{
    return v_screenColourBits;
}

void hV_Screen_Buffer::circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour)
{
    if (radius == 0)
    {
        s_pointClipped((int32_t)x0, (int32_t)y0, colour);
        return;
    }

    if (v_penSolid == false)
    {
        // Rim only, single Bresenham circle
        int16_t f = 1 - radius;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * radius;
        int16_t x = 0;
        int16_t y = radius;

        while (true)
        {
            int32_t dx8[8] = { +x, +y, +y, +x, -x, -y, -y, -x };
            int32_t dy8[8] = { -y, -x, +x, +y, +y, +x, -x, -y };

            for (uint8_t i = 0; i < 8; i++)
            {
                s_pointClipped((int32_t)x0 + dx8[i], (int32_t)y0 + dy8[i], colour);
            }

            if (x >= y)
            {
                break;
            }

            if (f >= 0)
            {
                y--;
                ddF_y += 2;
                f += ddF_y;
            }

            x++;
            ddF_x += 2;
            f += ddF_x;
        }

        return;
    }

    // Solid disc, scan-line by scan-line, as concentric chords from Bresenham would leave gaps
    int32_t squareRadius = (int32_t)radius * radius;
    int16_t limitY = (int16_t)radius;
    int16_t halfWidth = 0;

    for (int16_t dy = -limitY; dy <= limitY; dy++)
    {
        int32_t limit = squareRadius - (int32_t)dy * dy;

        while ((int32_t)(halfWidth + 1) * (halfWidth + 1) <= limit)
        {
            halfWidth++;
        }
        while ((halfWidth > 0) and ((int32_t)halfWidth * halfWidth > limit))
        {
            halfWidth--;
        }

        s_lineClipped((int32_t)x0 - halfWidth, (int32_t)y0 + dy, (int32_t)x0 + halfWidth, (int32_t)y0 + dy, colour);
    }
}

void hV_Screen_Buffer::dLine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour)
{
    if ((dx == 0) or (dy == 0))
    {
        return;
    }

    line(x0, y0, x0 + dx - 1, y0 + dy - 1, colour);
}

void hV_Screen_Buffer::line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if ((x1 == x2) and (y1 == y2))
    {
        s_setPoint(x1, y1, colour);
    }
    else if (x1 == x2)
    {
        if (y1 > y2)
        {
            hV_HAL_swap(y1, y2);
        }
        for (uint16_t y = y1; y <= y2; y++)
        {
            s_setPoint(x1, y, colour);
        }
    }
    else if (y1 == y2)
    {
        if (x1 > x2)
        {
            hV_HAL_swap(x1, x2);
        }
        for (uint16_t x = x1; x <= x2; x++)
        {
            s_setPoint(x, y1, colour);
        }
    }
    else
    {
        int16_t wx1 = (int16_t)x1;
        int16_t wx2 = (int16_t)x2;
        int16_t wy1 = (int16_t)y1;
        int16_t wy2 = (int16_t)y2;

        bool flag = abs(wy2 - wy1) > abs(wx2 - wx1);
        if (flag)
        {
            hV_HAL_swap(wx1, wy1);
            hV_HAL_swap(wx2, wy2);
        }

        if (wx1 > wx2)
        {
            hV_HAL_swap(wx1, wx2);
            hV_HAL_swap(wy1, wy2);
        }

        int16_t dx = wx2 - wx1;
        int16_t dy = abs(wy2 - wy1);
        int16_t err = dx / 2;
        int16_t ystep;

        if (wy1 < wy2)
        {
            ystep = 1;
        }
        else
        {
            ystep = -1;
        }

        for (; wx1 <= wx2; wx1++)
        {
            if (flag)
            {
                s_setPoint(wy1, wx1, colour);
            }
            else
            {
                s_setPoint(wx1, wy1, colour);
            }

            err -= dy;
            if (err < 0)
            {
                wy1 += ystep;
                err += dx;
            }
        }
    }
}

void hV_Screen_Buffer::setPenSolid(bool flag)
{
    v_penSolid = flag;
}

void hV_Screen_Buffer::point(uint16_t x1, uint16_t y1, uint16_t colour)
{
    s_setPoint(x1, y1, colour);
}

void hV_Screen_Buffer::rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if (v_penSolid == false)
    {
        line(x1, y1, x1, y2, colour);
        line(x1, y1, x2, y1, colour);
        line(x1, y2, x2, y2, colour);
        line(x2, y1, x2, y2, colour);
    }
    else
    {
        if (x1 > x2)
        {
            hV_HAL_swap(x1, x2);
        }
        if (y1 > y2)
        {
            hV_HAL_swap(y1, y2);
        }
        for (uint16_t x = x1; x <= x2; x++)
        {
            for (uint16_t y = y1; y <= y2; y++)
            {
                s_setPoint(x, y, colour);
            }
        }
    }
}

void hV_Screen_Buffer::dRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour)
{
    if ((dx == 0) or (dy == 0))
    {
        return;
    }

    rectangle(x0, y0, x0 + dx - 1, y0 + dy - 1, colour);
}

void hV_Screen_Buffer::triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour)
{
    int32_t ax = (int32_t)x1;
    int32_t ay = (int32_t)y1;
    int32_t bx = (int32_t)x2;
    int32_t by = (int32_t)y2;
    int32_t cx = (int32_t)x3;
    int32_t cy = (int32_t)y3;

    // Twice signed area; positive if ABC is counter-clockwise
    int32_t area = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);

    if (v_penSolid and (area != 0))
    {
        // Clockwise vertices would reverse all three half-planes; swap to make area > 0
        if (area < 0)
        {
            hV_HAL_swap(bx, cx);
            hV_HAL_swap(by, cy);
            area = -area;
        }

        int32_t xMin = ax;
        int32_t xMax = ax;
        int32_t yMin = ay;
        int32_t yMax = ay;

        if (bx < xMin) { xMin = bx; }
        if (bx > xMax) { xMax = bx; }
        if (by < yMin) { yMin = by; }
        if (by > yMax) { yMax = by; }
        if (cx < xMin) { xMin = cx; }
        if (cx > xMax) { xMax = cx; }
        if (cy < yMin) { yMin = cy; }
        if (cy > yMax) { yMax = cy; }

        int32_t xLeft = 0;
        int32_t yTop = 0;
        int32_t xRight = (int32_t)screenSizeX() - 1;
        int32_t yBottom = (int32_t)screenSizeY() - 1;

        if (xMin < xLeft) { xMin = xLeft; }
        if (yMin < yTop) { yMin = yTop; }
        if (xMax > xRight) { xMax = xRight; }
        if (yMax > yBottom) { yMax = yBottom; }

        if ((xMin <= xMax) and (yMin <= yMax))
        {
            // e(A,B,P) = (B-A) x (P-A); de/dx = Ay-By, de/dy = Bx-Ax
            int32_t dAB_dx = ay - by;
            int32_t dBC_dx = by - cy;
            int32_t dCA_dx = cy - ay;

            auto edge = [](int32_t xA, int32_t yA, int32_t xB, int32_t yB, int32_t x, int32_t y) -> int32_t
            {
                return ((xB - xA) * (y - yA) - (yB - yA) * (x - xA));
            };

            for (int32_t y = yMin; y <= yMax; y++)
            {
                int32_t wAB = edge(ax, ay, bx, by, xMin, y);
                int32_t wBC = edge(bx, by, cx, cy, xMin, y);
                int32_t wCA = edge(cx, cy, ax, ay, xMin, y);

                bool flagRun = false;
                int32_t runStart = xMin;

                for (int32_t x = xMin; x <= xMax; x++)
                {
                    bool flagInside = ((wAB >= 0) and (wBC >= 0) and (wCA >= 0));

                    if (flagInside and (flagRun == false))
                    {
                        runStart = x;
                        flagRun = true;
                    }

                    if (flagRun and ((flagInside == false) or (x == xMax)))
                    {
                        int32_t runEnd = flagInside ? x : (x - 1);
                        s_lineClipped(runStart, y, runEnd, y, colour);
                        flagRun = false;
                    }

                    wAB += dAB_dx;
                    wBC += dBC_dx;
                    wCA += dCA_dx;
                }
            }
        }
    }

    // Outline, and the only drawing for a transparent pen or a degenerate triangle
    s_lineClipped(ax, ay, bx, by, colour);
    s_lineClipped(bx, by, cx, cy, colour);
    s_lineClipped(cx, cy, ax, ay, colour);
}

void hV_Screen_Buffer::s_pointClipped(int32_t x1, int32_t y1, uint16_t colour)
{
    if ((x1 >= 0) and (y1 >= 0) and (x1 < (int32_t)screenSizeX()) and (y1 < (int32_t)screenSizeY()))
    {
        point((uint16_t)x1, (uint16_t)y1, colour);
    }
}

void hV_Screen_Buffer::s_lineClipped(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t colour)
{
    // Signed coordinates, as line() would wrap negative values to huge unsigned ones
    int32_t dx = abs(x2 - x1);
    int32_t dy = -abs(y2 - y1);
    int32_t signx = (x1 < x2) ? 1 : -1;
    int32_t signy = (y1 < y2) ? 1 : -1;
    int32_t error = dx + dy;

    while (true)
    {
        s_pointClipped(x1, y1, colour);

        if ((x1 == x2) and (y1 == y2))
        {
            break;
        }

        int32_t error2 = 2 * error;

        if (error2 >= dy)
        {
            error += dy;
            x1 += signx;
        }
        if (error2 <= dx)
        {
            error += dx;
            y1 += signy;
        }
    }
}

//
// === Touch section
//
bool hV_Screen_Buffer::isTouch()
{
    return (v_touchTrim > 0);
}
bool hV_Screen_Buffer::isTouchEvent()
{
    return v_touchEvent;
}

bool hV_Screen_Buffer::getTouch(touch_t & touch)
{
    if (v_touchTrim == 0)
    {
        return false;
    }

    bool _result = false;
    touch_t _touch0;

    hV_HAL_delayMilliseconds(16); // non-blocking delay to prevent freeze from I²C acquisition
    s_getRawTouch(_touch0);
    touch.z = _touch0.z;
    touch.t = _touch0.t;

    if (touch.z > v_touchTrim)
    {
        _touch0.x = checkRange((uint16_t)(_touch0.x), (uint16_t)(v_touchXmin), (uint16_t)(v_touchXmax));
        _touch0.y = checkRange((uint16_t)(_touch0.y), (uint16_t)(v_touchYmin), (uint16_t)(v_touchYmax));

        // Raw coordinates on physical screen to logical screen coordinates
        switch (v_orientation)
        {
            case 0: // ok

                touch.x = hV_HAL_map(_touch0.x, v_touchXmin, v_touchXmax, 0, v_screenSizeH - 1);
                touch.y = hV_HAL_map(_touch0.y, v_touchYmin, v_touchYmax, 0, v_screenSizeV - 1);
                break;

            case 1: // ok

                touch.x = hV_HAL_map(_touch0.y, v_touchYmin, v_touchYmax, 0, v_screenSizeV - 1);
                touch.y = hV_HAL_map(_touch0.x, v_touchXmin, v_touchXmax, v_screenSizeH - 1, 0);
                break;

            case 2: // ok

                touch.x = hV_HAL_map(_touch0.x, v_touchXmin, v_touchXmax, v_screenSizeH - 1, 0);
                touch.y = hV_HAL_map(_touch0.y, v_touchYmin, v_touchYmax, v_screenSizeV - 1, 0);
                break;

            case 3: // ok

                touch.x = hV_HAL_map(_touch0.y, v_touchYmin, v_touchYmax, v_screenSizeV - 1, 0);
                touch.y = hV_HAL_map(_touch0.x, v_touchXmin, v_touchXmax, 0, v_screenSizeH - 1);
                break;
        }
        _result = true;
    }

    return _result;
}

bool hV_Screen_Buffer::getTouchInterrupt()
{
    return s_getInterruptTouch();
}

void hV_Screen_Buffer::clearTouch()
{
    touch_t touch;
    // while (getTouchInterrupt())
    while (getTouch(touch))
    {
        hV_HAL_delayMilliseconds(10);
    }
    v_touchEvent = TOUCH_EVENT_NONE;
}

void hV_Screen_Buffer::s_getRawTouch(touch_t & touch)
{
    touch.x = 0;
    touch.y = 0;
    touch.z = 0;
    touch.t = 0;
}

bool hV_Screen_Buffer::s_getInterruptTouch()
{
    return false;
}
//
// === End of Touch section
//

//
// === Font section
//
void hV_Screen_Buffer::setFontSolid(bool flag)
{
    f_setFontSolid(flag);
}

uint8_t hV_Screen_Buffer::addFont(font_s fontName)
{
    return f_addFont(fontName);
}

void hV_Screen_Buffer::selectFont(uint8_t fontIndex)
{
    f_selectFont(fontIndex);
}

uint8_t hV_Screen_Buffer::getFont()
{
    return f_fontIndex;
}

uint8_t hV_Screen_Buffer::fontMax()
{
    return f_fontMax();
}

uint16_t hV_Screen_Buffer::characterSizeX(STRING_CONST_TYPE character)
{
    uint16_t result = 0;
    if ((f_font.kind & 0x40) == 0x40) // Monospaced font
    {
        result = f_font.maxWidth + f_fontSpaceX;
    }
    else
    {
        uint16_t buffer2[2] = {0};
        uint16_t _size16 = 0;

#if (STRING_MODE == USE_STRING_OBJECT)

        _size16 = utf8to16(character.c_str(), buffer2, 2);

#elif (STRING_MODE == USE_CHAR_ARRAY)

        _size16 = utf8to16(character, buffer2, 2);

#endif // STRING_MODE

        result = characterSizeX(buffer2[0]);
    }

    return result;
}

uint16_t hV_Screen_Buffer::characterSizeX(uint16_t character)
{
    uint16_t result = 0;

    if ((f_font.kind & 0x40) == 0x40) // Monospaced font
    {
        result = f_font.maxWidth + f_fontSpaceX;
    }
    else
    {
        result = f_characterSizeX(character);
    }

    return result;
}

uint16_t hV_Screen_Buffer::characterSizeY()
{
    return f_characterSizeY();
}

uint16_t hV_Screen_Buffer::stringSizeX(STRING16_CONST_TYPE text16)
{
    return f_stringSizeX(text16);
}

uint16_t hV_Screen_Buffer::stringSizeX(STRING_CONST_TYPE text8)
{
    return f_stringSizeX(text8);
}

uint8_t hV_Screen_Buffer::stringLengthToFitX(STRING_CONST_TYPE text8, uint16_t pixels)
{
    return f_stringLengthToFitX(text8, pixels);
}

uint8_t hV_Screen_Buffer::stringLengthToFitX(STRING16_CONST_TYPE text16, uint16_t pixels)
{
    return f_stringLengthToFitX(text16, pixels);
}

void hV_Screen_Buffer::setFontSpaceX(uint8_t number)
{
    f_setFontSpaceX(number);
}

void hV_Screen_Buffer::setFontSpaceY(uint8_t number)
{
    f_setFontSpaceY(number);
}

uint8_t hV_Screen_Buffer::s_getCharacter(uint8_t character, uint8_t index)
{
    return f_getCharacter(character, index);
}

void hV_Screen_Buffer::gText(uint16_t x0, uint16_t y0,
                             STRING_CONST_TYPE text8,
                             uint16_t textColour,
                             uint16_t backColour)
{
    uint16_t _buffer16[BUFFER_LENGTH] = {0};
    uint16_t _size16 = utf8to16(text8.c_str(), _buffer16);

    if (_size16 == 0)
    {
        return;
    }

    gText(x0, y0, _buffer16, textColour, backColour);
}

void hV_Screen_Buffer::gText(uint16_t x0, uint16_t y0,
                             STRING16_CONST_TYPE text16,
                             uint16_t textColour,
                             uint16_t backColour)
{
    uint16_t _size16 = 0;
    while (text16[++_size16] != 0x0000);
    _size16 = (text16[0] == 0x000) ? 0 : _size16;

    if (_size16 == 0)
    {
        return;
    }

#if (FONT_MODE == USE_FONT_TERMINAL)

    uint8_t character8;
    uint8_t line, line1, line2; // , line3;
    uint16_t x, y;
    uint8_t i, j, k;

#if (MAX_FONT_SIZE > 0)

    if (f_fontIndex == 0)
    {
        for (k = 0; k < _size16; k++)
        {
            x = x0 + (6 + f_fontSpaceX) * k;
            y = y0;
            character8 = (text16[k] == 0x20ac) ? 0x80 - ' ' : (text16[k] & 0xff) - ' ';

            for (i = 0; i < 6; i++)
            {
                line = f_getCharacter(character8, i);

                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        point(x + i, y + j, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        point(x + i, y + j, backColour);
                    }
                }
            }
        }
    }

#if (MAX_FONT_SIZE > 1)

    else if (f_fontIndex == 1)
    {
        for (k = 0; k < _size16; k++)
        {
            x = x0 + (8 + f_fontSpaceX) * k;
            y = y0;
            character8 = (text16[k] == 0x20ac) ? 0x80 - ' ' : (text16[k] & 0xff) - ' ';

            for (i = 0; i < 8; i++)
            {
                line = f_getCharacter(character8, 2 * i);
                line1 = f_getCharacter(character8, 2 * i + 1);

                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        point(x + i, y + j, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        point(x + i, y + j, backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        point(x + i, y + 8 + j, textColour);
                    }
                    else if ((f_fontSolid) and (j < 4))
                    {
                        point(x + i, y + 8 + j, backColour);
                    }
                }
            }
        }
    }

#if (MAX_FONT_SIZE > 2)

    else if (f_fontIndex == 2)
    {
        for (k = 0; k < _size16; k++)
        {
            x = x0 + (12 + f_fontSpaceX) * k;
            y = y0;
            character8 = (text16[k] == 0x20ac) ? 0x80 - ' ' : (text16[k] & 0xff) - ' ';

            for (i = 0; i < 12; i++)
            {
                line = f_getCharacter(character8, 2 * i);
                line1 = f_getCharacter(character8, 2 * i + 1);

                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        point(x + i, y + j, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        point(x + i, y + j, backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        point(x + i, y + 8 + j, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        point(x + i, y + 8 + j, backColour);
                    }
                }
            }
        }
    }

#if (MAX_FONT_SIZE > 3)

    else if (f_fontIndex == 3)
    {
        for (k = 0; k < _size16; k++)
        {
            x = x0 + (16 + f_fontSpaceX) * k;
            y = y0;
            character8 = (text16[k] == 0x20ac) ? 0x80 - ' ' : (text16[k] & 0xff) - ' ';

            for (i = 0; i < 16; i++)
            {
                line = f_getCharacter(character8, 3 * i);
                line1 = f_getCharacter(character8, 3 * i + 1);
                line2 = f_getCharacter(character8, 3 * i + 2);
                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        point(x + i, y + j, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        point(x + i, y + j, backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        point(x + i, y + 8 + j, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        point(x + i, y + 8 + j, backColour);
                    }
                    if (bitRead(line2, j))
                    {
                        point(x + i, y + 16 + j, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        point(x + i, y + 16 + j, backColour);
                    }
                }
            }
        }
    }
#endif // end MAX_FONT_SIZE > 3
#endif // end MAX_FONT_SIZE > 2
#endif // end MAX_FONT_SIZE > 1
#endif // end MAX_FONT_SIZE > 0

#endif // FONT_MODE
}

void hV_Screen_Buffer::gTextLarge(uint16_t x0, uint16_t y0,
                                  STRING_CONST_TYPE text8,
                                  uint16_t textColour,
                                  uint16_t backColour)
{
    uint16_t _buffer16[BUFFER_LENGTH] = {0};
    uint16_t _size16 = 0;

    _size16 = utf8to16(text8.c_str(), _buffer16);

    if (_size16 == 0)
    {
        return;
    }

    gTextLarge(x0, y0, _buffer16, textColour, backColour);
}

void hV_Screen_Buffer::gTextLarge(uint16_t x0, uint16_t y0,
                                  STRING16_CONST_TYPE text16,
                                  uint16_t textColour,
                                  uint16_t backColour)
{
    uint16_t _size16 = 0;
    while (text16[++_size16] != 0x0000);
    if (_size16 == 0)
    {
        return;
    }

#if (FONT_MODE == USE_FONT_TERMINAL)

    uint8_t character8;
    uint8_t line, line1, line2; //, line3;
    uint16_t x, y;
    uint8_t i, j, k;

    uint8_t ix = 2;
    uint8_t iy = 2;

    bool oldPenSolid = v_penSolid;
    setPenSolid(true);

#if (MAX_FONT_SIZE > 0)

    if (f_fontIndex == 0)
    {
        for (k = 0; k < _size16; k++)
        {
            x = x0 + (6 + f_fontSpaceX) * k * ix;
            y = y0;
            character8 = (text16[k] == 0x20ac) ? 0x80 - ' ' : (text16[k] & 0xff) - ' ';

            for (i = 0; i < 6; i++)
            {
                line = f_getCharacter(character8, i);

                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        dRectangle(x + i * ix, y + j * iy, ix, iy, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        dRectangle(x + i * ix, y + j * iy, ix, iy, backColour);
                    }
                }
            }
        }
    }

#if (MAX_FONT_SIZE > 1)

    else if (f_fontIndex == 1)
    {
        for (k = 0; k < _size16; k++)
        {
            x = x0 + (8 + f_fontSpaceX) * k * ix;
            y = y0;
            character8 = (text16[k] == 0x20ac) ? 0x80 - ' ' : (text16[k] & 0xff) - ' ';

            for (i = 0; i < 8; i++)
            {
                line = f_getCharacter(character8, 2 * i);
                line1 = f_getCharacter(character8, 2 * i + 1);

                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        dRectangle(x + i * ix, y0 + j * iy, ix, iy, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        dRectangle(x + i * ix, y0 + j * iy, ix, iy, backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        dRectangle(x + i * ix, y0 + (8 + j) * iy, ix, iy, textColour);
                    }
                    else if ((f_fontSolid) and (j < 4))
                    {
                        dRectangle(x + i * ix, y0 + (8 + j) * iy, ix, iy, backColour);
                    }
                }
            }
        }
    }

#if (MAX_FONT_SIZE > 2)

    else if (f_fontIndex == 2)
    {
        for (k = 0; k < _size16; k++)
        {
            x = x0 + (12 + f_fontSpaceX) * k * ix;
            y = y0;
            character8 = (text16[k] == 0x20ac) ? 0x80 - ' ' : (text16[k] & 0xff) - ' ';

            for (i = 0; i < 12; i++)
            {
                line = f_getCharacter(character8, 2 * i);
                line1 = f_getCharacter(character8, 2 * i + 1);

                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        dRectangle(x + i * ix, y0 + j * iy, ix, iy, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        dRectangle(x + i * ix, y0 + j * iy, ix, iy, backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        dRectangle(x + i * ix, y0 + (8 + j) * iy, ix, iy, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        dRectangle(x + i * ix, y0 + (8 + j) * iy, ix, iy, backColour);
                    }
                }
            }
        }
    }

#if (MAX_FONT_SIZE > 3)

    else if (f_fontIndex == 3)
    {
        for (k = 0; k < _size16; k++)
        {
            x = x0 + (16 + f_fontSpaceX) * k * ix;
            y = y0;
            character8 = (text16[k] == 0x20ac) ? 0x80 - ' ' : (text16[k] & 0xff) - ' ';

            for (i = 0; i < 16; i++)
            {
                line = f_getCharacter(character8, 3 * i);
                line1 = f_getCharacter(character8, 3 * i + 1);
                line2 = f_getCharacter(character8, 3 * i + 2);
                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        dRectangle(x + i * ix, y0 + j * iy, ix, iy, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        dRectangle(x + i * ix, y0 + j * iy, ix, iy, backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        dRectangle(x + i * ix, y0 + (8 + j) * iy, ix, iy, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        dRectangle(x + i * ix, y0 + (8 + j) * iy, ix, iy, backColour);
                    }
                    if (bitRead(line2, j))
                    {
                        dRectangle(x + i * ix, y0 + (16 + j) * iy, ix, iy, textColour);
                    }
                    else if (f_fontSolid)
                    {
                        dRectangle(x + i * ix, y0 + (16 + j) * iy, ix, iy, backColour);
                    }
                }
            }
        }
    }
#endif // end MAX_FONT_SIZE > 3
#endif // end MAX_FONT_SIZE > 2
#endif // end MAX_FONT_SIZE > 1
#endif // end MAX_FONT_SIZE > 0

    setPenSolid(oldPenSolid);

#endif // FONT_MODE
}
//
// === End of Font section
//

