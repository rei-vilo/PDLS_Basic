//
// hV_Haptics_GUI.cpp
// Library C++ code
// ----------------------------------
//
// Project Pervasive Displays Library Suite
// Based on highView technology
//
// Created by Rei Vilo, 14 Jun 2013
// Last updated by Rei Vilo, 25 Jan 2022
//
// Copyright (c) Rei Vilo, 2010-2025
// Licence All rights reserved
//
// See hV_GUI.h and ReadMe.txt for references
//
// Release 531: Improved performance
// Release 605: Improved elements link to GUI
// Release 608: Shared common debouncing module
//

// Library header
#include "hV_GUI.h"

#if (HAPTICS_MODE != USE_HAPTICS_NONE)
DRV2605L myDRV2605L(HAPTICS_MODE);
#endif // HAPTICS_MODE

GUI::GUI(Screen_EPD * screen)
{
    g_pScreen = screen;
}

void GUI::begin()
{
    g_colourFront = myColours.black;
    g_colourBack = myColours.white;
    g_delegate = true;

#if (HAPTICS_MODE != USE_HAPTICS_NONE)
    myDRV2605L.begin();
#endif // HAPTICS_MODE
}

void GUI::setColours(uint16_t frontColour, uint16_t backColour)
{
    g_colourFront = frontColour;
    g_colourBack = backColour;
}

void GUI::delegate(bool delegate)
{
    g_delegate = delegate;
}

// --- Text
Text::Text(GUI * gui)
{
    _pGUI = gui;
}

void Text::dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                   uint8_t size)
{
    _x0 = x0;
    _y0 = y0;
    _dx = dx;
    _dy = dy;
    _fontSize = size;
}

void Text::draw(STRING_CONST_TYPE text)
{
    _pGUI->g_pScreen->selectFont(_fontSize);
    uint8_t k = _pGUI->g_pScreen->stringLengthToFitX(text, _dx - 8);

    String _text = text.substring(0, k);

    uint16_t _xt = _x0 + (_dx - _pGUI->g_pScreen->stringSizeX(_text)) / 2;
    uint16_t _yt = _y0 + (_dy - _pGUI->g_pScreen->characterSizeY()) / 2;

    _pGUI->g_pScreen->setPenSolid(true);
    _pGUI->g_pScreen->dRectangle(_x0, _y0, _dx, _dy, _pGUI->g_colourBack);
    _pGUI->g_pScreen->gText(_xt, _yt, _text, _pGUI->g_colourFront);

    if (_pGUI->g_delegate)
    {
        _pGUI->g_pScreen->flush();
    }
}

// ---- Button
Button::Button(GUI * gui)
{
    _pGUI = gui;
}

void Button::dStringDefine(uint16_t x0, uint16_t y0,
                           uint16_t dx, uint16_t dy,
                           STRING_CONST_TYPE text0,
                           uint8_t size0)
{
    Text::dDefine(x0, y0, dx, dy, size0);
    Text::draw(text0);
    draw(fsmReleased);
}

void Button::draw(fsmGUI_e fsm)
{
    // All cases
    _pGUI->g_pScreen->setPenSolid(false);
    _pGUI->g_pScreen->dRectangle(_x0 + 1, _y0 + 1, _dx - 2, _dy - 2, _pGUI->g_colourFront);

    switch (fsm)
    {
        case fsmTouched:

            _pGUI->g_pScreen->dRectangle(_x0, _y0, _dx, _dy, _pGUI->g_colourFront);
            _pGUI->g_pScreen->dRectangle(_x0 + 2, _y0 + 2, _dx - 4, _dy - 4, _pGUI->g_colourFront);
            break;

        default: // fsmReleased

            _pGUI->g_pScreen->dRectangle(_x0, _y0, _dx, _dy, _pGUI->g_colourBack);
            _pGUI->g_pScreen->dRectangle(_x0 + 2, _y0 + 2, _dx - 4, _dy - 4, _pGUI->g_colourBack);
            break;
    }

    if (_pGUI->g_delegate)
    {
        _pGUI->g_pScreen->flush();
    }
}

bool Button::check(uint8_t mode)
{
    touch_t _touch;
    bool flag = false;

    // down
    if (_pGUI->g_pScreen->getTouch(_touch))
    {
        // pressed
        if ((_touch.x >= _x0) and (_touch.x < _x0 + _dx) and (_touch.y >= _y0) and (_touch.y < _y0 + _dy))
        {
            if (mode == checkInstant)
            {
                return true;
            }

            draw(fsmTouched);

            // do
            // {
            //     delay(100);
            //     _pGUI->g_pScreen->getTouch(x, y, z, t);
            // }
            // while (t != TOUCH_EVENT_RELEASE);
            while (_pGUI->g_pScreen->getTouch(_touch)) // Debounce
            {
                hV_HAL_delayMilliseconds(100);
            }

            // Released
            if ((_touch.x >= _x0) and (_touch.x < _x0 + _dx) and (_touch.y >= _y0) and (_touch.y < _y0 + _dy))
            {
                flag = true;
            }

            draw(fsmReleased);
        } // if pressed
    } // if down

    return flag;
}

