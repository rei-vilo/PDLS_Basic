///
/// @file hV_Screen_Buffer.h
/// @brief Generic buffered screen class library - Basic edition
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @date 21 Nov 2025
/// @version 1000
///
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

// SDK and configuration
#include "PDLS_Common.h"

#if (PDLS_COMMON_RELEASE < 1000)
#error Required PDLS_COMMON_RELEASE 1000
#endif // PDLS_COMMON_RELEASE

#ifndef hV_SCREEN_BUFFER_RELEASE
///
/// @brief Library release number
///
#define hV_SCREEN_BUFFER_RELEASE 1000

// Colours
#include "hV_Colours565.h"

#if (hV_COLOURS_RELEASE < 1000)
#error Required hV_COLOURS_RELEASE 1000
#endif // hV_COLOURS_RELEASE

// Utilities
#include "hV_Utilities.h"

#if (hV_UTILITIES_RELEASE < 1000)
#error Required hV_UTILITIES_RELEASE 1000
#endif // hV_UTILITIES_RELEASE

#if (FONT_MODE == USE_FONT_TERMINAL)
#include "hV_Font_Terminal.h"

#else
#error Required FONT_MODE == USE_FONT_TERMINAL
#endif // FONT_MODE

///
/// @brief Generic buffered screen class
/// @details This class provides the text and graphic primitives for the buffered screen
///
// #warning USE_hV_SCREEN == USE_hV_SCREEN_SELF
#warning FONT_MODE == USE_FONT_TERMINAL

class hV_Screen_Buffer : protected hV_Font_Terminal
{
  public:

    ///
    /// @brief Constructor
    ///
    hV_Screen_Buffer();

    /// @name General
    /// @{

    ///
    /// @brief Initialisation
    ///
    virtual void begin();

    ///
    /// @brief Request information about the screen
    /// @return string with hardware version
    ///
    virtual STRING_TYPE WhoAmI() = 0; // compulsory

    ///
    /// @brief Clear the screen
    /// @param colour 16-bit colour, default = white
    ///
    virtual void clear(uint16_t colour = myColours.black);

    ///
    /// @brief Send frame-buffer to display
    ///
    virtual void flush() = 0; // compulsory

    ///
    /// @brief Set orientation
    /// @param orientation orientation,
    /// * `0` = default
    /// * `1` = right rotated
    /// * `2` = reverse default
    /// * `3` = left rotated
    /// * ORIENTATION_PORTRAIT = `6` = check portrait
    /// * ORIENTATION_LANDSCAPE = `7` = check landscape
    ///
    /// @note Run the Common_Orientation.ino example to identify the options
    ///
    virtual void setOrientation(uint8_t orientation);

    ///
    /// @brief Get orientation
    /// @return orientation orientation:
    /// * `0` = portrait
    /// * `1` = right rotated landscape
    /// * `2` = reverse portrait
    /// * `3` = left rotated landscape
    ///
    /// @note Run the Common_Orientation.ino example to identify the options
    ///
    uint8_t getOrientation();

    ///
    /// @brief Screen size, x-axis of logical screen
    /// @return x-axis size of the screen, in pixels
    /// @note 240 means 240 pixels and thus 0..239 coordinates (decimal)
    ///
    virtual uint16_t screenSizeX();

    ///
    /// @brief Screen size, y-axis of logical screen
    /// @return y-axis size of the screen, in pixels
    /// @note 240 means 240 pixels and thus 0..239 coordinates (decimal)
    ///
    virtual uint16_t screenSizeY();

    ///
    /// @brief Screen size, diagonal
    /// @return diagonal size of the screen, in inches x 100
    /// @note 320 means 3.20"
    ///
    virtual uint16_t screenDiagonal();
    ///

    ///
    /// @brief Screen colour depth
    /// @return colour depth, in bits
    /// @note Examples: BW=1, BWR=2, BWRY=2, RGB565=16
    ///
    /// @n @b More: @ref Coordinate
    ///
    virtual uint8_t screenColourBits();

    /// @}

    /// @name Graphics
    ///
    /// @{

    ///
    /// @brief Draw circle
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param radius radius
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour);

    ///
    /// @brief Draw line, rectangle coordinates
    /// @param x1 top left coordinate, x-axis
    /// @param y1 top left coordinate, y-axis
    /// @param x2 bottom right coordinate, x-axis
    /// @param y2 bottom right coordinate, y-axis
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);

    ///
    /// @brief Draw line, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void dLine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour);

    ///
    /// @brief Set pen opaque
    /// @param flag default = `true` = opaque = solid, `false` = wire frame
    ///
    virtual void setPenSolid(bool flag = true);

    ///
    /// @brief Draw triangle, rectangle coordinates
    /// @param x1 first point coordinate, x-axis
    /// @param y1 first point coordinate, y-axis
    /// @param x2 second point coordinate, x-axis
    /// @param y2 second point coordinate, y-axis
    /// @param x3 third point coordinate, x-axis
    /// @param y3 third point coordinate, y-axis
    /// @param colour 16-bit colour
    /// @note If two points are equal, draw a line
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour);

    ///
    /// @brief Draw rectangle, rectangle coordinates
    /// @param x1 top left coordinate, x-axis
    /// @param y1 top left coordinate, y-axis
    /// @param x2 bottom right coordinate, x-axis
    /// @param y2 bottom right coordinate, y-axis
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);

    ///
    /// @brief Draw rectangle, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void dRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour);

    ///
    /// @brief Draw pixel
    /// @param x1 point coordinate, x-axis
    /// @param y1 point coordinate, y-axis
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void point(uint16_t x1, uint16_t y1, uint16_t colour);

    /// @}

    /// @name Text
    /// @{

    ///
    /// @brief Select font
    /// @param font default = 0, 0..fontMax()-1
    ///
    virtual void selectFont(uint8_t fontIndex);

    ///
    /// @brief Get font index
    /// @return Index of the font, 0..fontMax()-1
    ///
    virtual uint8_t getFont();

    ///
    /// @brief Add a font
    /// @param fontName name of the font
    /// @return number of fonts, 0 otherwise
    /// @note The index of the font added is `number - 1`
    /// @note Previously selectFont()
    /// @n @b More: @ref Fonts
    ///
    virtual uint8_t addFont(font_s fontName);

    ///
    /// @brief Set transparent or opaque text
    /// @param flag default = `true` = opaque = solid, `false` = transparent
    ///
    virtual void setFontSolid(bool flag = true);

    ///
    /// @brief Set additional spaces between two characters, horizontal axis
    /// @param number of spaces default = 1 pixel
    ///
    virtual void setFontSpaceX(uint8_t number = 1);

    ///
    /// @brief Set additional spaces between two characters, vertical axis
    /// @param number of spaces default = 1 pixel
    ///
    virtual void setFontSpaceY(uint8_t number = 1);

    ///
    /// @brief Character size, x-axis
    /// @param character character to evaluate, UTF-8 coded, default = `0` = font general size
    /// @return horizontal size of the font for the specified character, in pixels
    /// @warning Required UTF-8 coded
    /// @note Up to 3 bytes to code a character in UTF-8. Only the first UTF-16 is returned.
    /// @note With setSpaceX included
    /// @note In variable font, each character has a specific size.
    /// The general size is indicative.
    /// @note Previously fontSizeX()
    /// @n @b More: @ref Fonts
    ///
    virtual uint16_t characterSizeX(STRING_CONST_TYPE character);

    ///
    /// @brief Character size, x-axis
    /// @param character character to evaluate, UTF-16 coded, default = `0` = font general size
    /// @return horizontal size of the font for the specified character, in pixels
    /// @warning Required UTF-16 coded
    /// @note With setSpaceX included
    /// @note In variable font, each character has a specific size.
    ///
    /// @n @b More: @ref Fonts
    ///
    uint16_t characterSizeX(uint16_t character = 0x00);

    ///
    /// @brief Character size, y-axis
    /// @return vertical size for current font, in pixels
    /// @note Previously fontSizeY()
    /// @n @b More: @ref Fonts
    ///
    virtual uint16_t characterSizeY();

    ///
    /// @brief String size, x-axis
    /// @param text8 string to evaluate, UTF-8 coded
    /// @return horizontal size of the string for current font, in pixels
    /// @warning Required UTF-8 coded text
    /// @n @b More: @ref Fonts
    ///
    virtual uint16_t stringSizeX(STRING_CONST_TYPE text8);

    ///
    /// @brief String size, x-axis
    /// @param text16 string to evaluate, UTF-16 coded
    /// @return horizontal size of the string for current font, in pixels
    /// @warning Required UTF-16 coded text
    /// @n @b More: @ref Fonts
    ///
    virtual uint16_t stringSizeX(STRING16_CONST_TYPE text16);

    ///
    /// @brief Number of characters to fit a size, x-axis
    /// @param[in] text8 string to evaluate, UTF-8 coded
    /// @param[in] pixels number of pixels to fit in
    /// @return number of characters to be displayed inside the pixels
    /// @warning Required UTF-8 coded text
    /// @n @b More: @ref Fonts
    ///
    virtual uint8_t stringLengthToFitX(STRING_CONST_TYPE text8, uint16_t pixels);

    ///
    /// @brief Number of characters to fit a size, x-axis
    /// @param[in] text16 string to evaluate, UTF-16 coded
    /// @param[in] pixels number of pixels to fit in
    /// @return number of characters to be displayed inside the pixels
    /// @warning Required UTF-16 coded text
    /// @n @b More: @ref Fonts
    ///
    virtual uint8_t stringLengthToFitX(STRING16_CONST_TYPE text16, uint16_t pixels);

    ///
    /// @brief Number of fonts
    /// @return number of fonts available
    /// @note First font is numbered 0, second 1, ...
    /// @n The latest font is numbered fontMax()-1
    ///
    virtual uint8_t fontMax();

    ///
    /// @brief Draw UTF-8 coded text (pixel coordinates)
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param text UTF-8 coded text (uint8_t)
    /// @param textColour 16-bit colour, default = white
    /// @param backColour 16-bit colour, default = black
    /// @note Text converted into UTF-16 before display
    /// @warning Required UTF-8 coded text
    /// @deprecated ISO-8859-1 or Latin 1 coded text is deprecated, use UTF-8 coded text instead (10.0.0)
    ///
    /// @n @b More: @ref Colour, @ref Fonts, @ref Coordinate
    ///
    virtual void gText(uint16_t x0, uint16_t y0,
                       STRING_CONST_TYPE text,
                       uint16_t textColour = myColours.black,
                       uint16_t backColour = myColours.white);

    virtual void gText(uint16_t x0, uint16_t y0,
                       STRING16_CONST_TYPE text,
                       uint16_t textColour = myColours.black,
                       uint16_t backColour = myColours.white);

    ///
    /// @brief Draw UTF-16 coded text (pixel coordinates)
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param text UTF-8 coded text (uint8_t)
    /// @param textColour 16-bit colour, default = white
    /// @param backColour 16-bit colour, default = black
    /// @warning UTF-8 coded text is required
    /// @deprecated ISO-8859-1 or Latin 1 coded text is deprecated, use UTF-8 coded text instead (10.0.0)
    ///
    /// @n @b More: @ref Colour, @ref Fonts, @ref Coordinate
    ///
    virtual void gTextLarge(uint16_t x0, uint16_t y0,
                            STRING_CONST_TYPE text,
                            uint16_t textColour = myColours.black,
                            uint16_t backColour = myColours.white);

    virtual void gTextLarge(uint16_t x0, uint16_t y0,
                            STRING16_CONST_TYPE text,
                            uint16_t textColour = myColours.black,
                            uint16_t backColour = myColours.white);
    /// @}

    //
    // === Touch section
    //
    /// @name Touch
    /// @note Those functions are optional, and thus defined as empty and not compulsory.
    /// @{

    ///
    /// @brief Is touch available?
    /// @return true is touch available, false otherwise
    ///
    bool isTouch();

    ///
    /// @brief Are touch events available?
    /// @return true if touch events available, false otherwise
    ///
    bool isTouchEvent();

    ///
    /// @brief Poll touch
    /// @param[out] touch touch structure
    /// @return true if touch pressed or moved
    ///
    /// @n @b More: @ref Coordinate, @ref Touch
    ///
    bool getTouch(touch_t & touch);

    ///
    /// @brief Clear touch
    ///
    void clearTouch();

    ///
    /// @brief Check touch interrupt
    ///
    /// @return true interrupt raised, false otherwise
    ///
    bool getTouchInterrupt();

    ///
    /// @brief Calibrate the touch
    ///
    void calibrateTouch() { };

    /// @}
    //
    // === End of Touch section
    //

  protected:

    /// @cond NOT_PUBLIC
    ///
    // * Virtual = 0 compulsory functions
    // Orientation
    ///
    /// @brief Set orientation
    /// @param orientation same as setOrientation()
    ///
    virtual void s_setOrientation(uint8_t orientation) = 0; // compulsory

    ///
    /// @brief Orient coordinates and check within screen
    /// @param[out] x1 x coordinate
    /// @param[out] y1 y coordinate
    /// @return `RESULT_SUCCESS` = false = success, `RESULT_ERROR` = true = error
    ///
    virtual bool s_orientCoordinates(uint16_t & x1, uint16_t & y1) = 0; // compulsory

    // Position
    ///
    /// @brief Set point
    /// @param x1 x coordinate
    /// @param y1 y coordinate
    /// @param colour 16-bit colour
    /// @n @b More: @ref Colour, @ref Coordinate
    ///
    virtual void s_setPoint(uint16_t x1, uint16_t y1, uint16_t colour) = 0; // compulsory

    // Touch
    virtual void s_getRawTouch(touch_t & touch); // compulsory
    virtual bool s_getInterruptTouch(); // compulsory

    // Other functions
    // required by triangle()
    ///
    /// @brief Triangle utility
    /// @param x1 first point coordinate, x-axis
    /// @param y1 first point coordinate, y-axis
    /// @param x2 second point coordinate, x-axis
    /// @param y2 second point coordinate, y-axis
    /// @param x3 third point coordinate, x-axis
    /// @param y3 third point coordinate, y-axis
    /// @param colour 16-bit colour
    ///
    void s_triangleArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour);

    // required by gText()
    ///
    /// @brief Get definition for line of character
    /// @param character character 32~255
    /// @param index column index
    /// @return definition for line of character
    ///
    uint8_t s_getCharacter(uint8_t character, uint8_t index);

    // Frame-buffer
    FRAMEBUFFER_TYPE s_newImage;

    // Variables provided by hV_Screen_Virtual
    bool v_penSolid;
    bool v_flagRead = true; // from frame-buffer
    bool v_flagStorage = false;
    bool v_flagEnergy = false;
    uint8_t v_orientation, v_intensity;
    uint16_t v_screenSizeH, v_screenSizeV, v_screenDiagonal, v_screenMarginH, v_screenMarginV;
    uint16_t v_screenColourBits;
    uint8_t v_fsmArea = 0x00; // no area
    uint8_t v_touchTrim = 0x00; // no touch
    bool v_touchEvent = false; // no touch event
    uint16_t v_touchXmin, v_touchXmax, v_touchYmin, v_touchYmax;

    /// @endcond
};

#endif // hV_SCREEN_BUFFER_RELEASE
