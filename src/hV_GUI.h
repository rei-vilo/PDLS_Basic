///
/// @file hV_GUI.h
/// @brief Basic GUI with text and button
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @date 21 Jan 2025
/// @version 902
///
/// @copyright (c) Pervasive Displays Inc., 2021-2025
/// @copyright (c) Etigues, 2010-2025
/// @copyright All rights reserved
/// @copyright For exclusive use with Pervasive Displays screens
///
/// * Basic edition: for hobbyists and for basic usage
/// @n Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
/// @see https://creativecommons.org/licenses/by-sa/4.0/
///
/// @n Consider the Evaluation or Commercial editions for professionals or organisations and for commercial usage
///
/// * Evaluation edition: for professionals or organisations, evaluation only, no commercial usage
/// @n All rights reserved
///
/// * Commercial edition: for professionals or organisations, commercial usage
/// @n All rights reserved
///
/// * Viewer edition: for professionals or organisations
/// @n All rights reserved
///
/// * Documentation
/// @n All rights reserved
///
/// @warning Starting release 6.0.5,
/// + Names of the Text and Button elements are now capitalised;
/// + Text and Button constructors require a GUI object to which the element belongs as parameter.

#ifndef hV_GUI_BASIC_RELEASE
///
/// @brief Library release number
///
#define hV_GUI_RELEASE 902

// SDK and configuration
#include "PDLS_Common.h"

#if (PDLS_COMMON_RELEASE < 902)
#error Required PDLS_COMMON_RELEASE 902
#endif // PDLS_COMMON_RELEASE

// Other libraries
#include "Screen_EPD.h"

#if (SCREEN_EPD_RELEASE < 902)
#error Required SCREEN_EPD_RELEASE 902
#endif // SCREEN_EPD_RELEASE

#ifndef WITH_TOUCH
#warning Required WITH_TOUCH
#endif // WITH_TOUCH

///
/// @name Modes for check()
/// @{
#define checkNormal 0 ///< Normal mode
#define checkInstant 2 ///< Instant mode

///
/// @class GUI
/// @brief GUI
/// @details Shared settings
///
class GUI
{
    friend class Button;
    friend class Text;

  public:
    ///
    /// @brief Constructor
    ///
    /// @param screen &screen to which the GUI elements belong
    ///
    GUI(Screen_EPD * screen);

    ///
    /// @brief Initialise the GUI
    ///
    void begin();

    ///
    /// @brief Set the colours palette
    ///
    /// @param frontColour 16-bit colours for text and rectangles
    /// @param backColour 16-bit colours for background
    ///
    void setColours(uint16_t frontColour = myColours.black,
                    uint16_t backColour = myColours.white);

    ///
    /// @brief Delegate the refresh policy
    ///
    /// @param delegate true = default = refresh managed by the GUI element with fast update
    /// @note If false, refresh managed by the caller
    ///
    void delegate(bool delegate = true);

  private:
    Screen_EPD * g_pScreen;
    uint16_t g_colourFront, g_colourBack, g_colourMiddle;
    bool g_delegate;
    bool g_enable;
    uint8_t g_style;
};

///
/// @brief Finite state machine
/// @details Used for Button, CheckBox, Cursor, BarGraph
///
enum fsmGUI_e
{
    fsmReleased, ///< No touch
    fsmTouched ///< First touch
};

///
/// @class Text
/// @brief Text GUI element
/// @details Text displays a text with format options
///
class Text
{
  public:
    ///
    /// @brief Constructor
    /// @param gui &gui to which the element belongs
    ///
    Text(GUI * gui);

    Text() = default;

    ///
    /// @brief Define a text box, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param size size of the font, default = 0 = small, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void dDefine(uint16_t x0, uint16_t y0,
                 uint16_t dx, uint16_t dy,
                 uint8_t size = 0);

    ///
    /// @brief Display the text
    /// @param text text to be displayed
    ///
    void draw(STRING_CONST_TYPE text);

  protected:
    /// @cond
    GUI * _pGUI;
    uint16_t _x0, _y0, _dx, _dy;
    uint8_t _fontSize;
    /// @endcond
};

///
/// @class Button
/// @brief Button
/// @details Button is an active text
///
class Button : public Text
{
  public:
    ///
    /// @brief Constructor
    /// param gui &gui to which the element belongs
    ///
    Button(GUI * gui);

    ///
    /// @brief Define button, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param text0 text
    /// @param size0 size for text, default = 0
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void dStringDefine(uint16_t x0, uint16_t y0,
                       uint16_t dx, uint16_t dy,
                       STRING_CONST_TYPE text0,
                       uint8_t size0 = 0);

    ///
    /// @brief Draw the button
    /// @param fsm state
    /// @n fsm = fsmReleased or fsmTouched
    ///
    void draw(fsmGUI_e fsm = fsmReleased);

    ///
    /// @brief Check button is pressed
    /// @param mode default = checkNormal waits for release, checkInstant = element is activated by press only
    /// @return true if button pressed
    ///
    bool check(uint8_t mode = checkNormal);
};

#endif // hV_GUI_BASIC_RELEASE
