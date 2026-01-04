///
/// @file hV_Documentation.h
/// @brief Documentation for the Pervasive Displays Library Suite
///
/// @details Project Pervasive Displays Library Suite - Basic edition
/// @n Based on highView technology
///
/// @mainpage Software Reference Manual
///
/// @details The software includes the **drivers** and the **Pervasive Displays Library Suite - Basic edition**.
///
/// They are specifically designed for the [Pervasive Displays](https://www.pervasivedisplays.com/) e-paper screens, extension boards and development kits.
///
/// <p><center><img src="Pervasive_Displays_logo_gray.png" alt="Pervasive Displays" style="align=middle;border:0px;padding:4px;background-color:transparent;" /></center></p>
///
/// ## Drivers
///
/// The driver library manages the panel and the board. It sends the content of a C-array image to the screen and refreshes the panel. It is based on the film and the size of the screen.
///
/// * @ref Pervasive_BWRY_Small
/// * @ref Pervasive_BWRY_Medium
/// * @ref Pervasive_BWRY_Large
/// * @ref Pervasive_Wide_Small
/// * @ref Pervasive_Wide_Medium
/// * @ref Pervasive_Wide_Large
/// * @ref Pervasive_Touch_Small
///
/// The drivers are available at the [Pervasive Displays GitHub page](https://github.com/PervasiveDisplays).
///
/// ## Pervasive Displays Library Suite - Basic edition
///
/// The Pervasive Displays Library Suite provides the text and graphics functions and the fonts to generate the image to be displayed through the driver.
///
/// * @ref Screen_EPD, derived from
/// * @ref hV_Screen_Buffer
///
/// Release 10 introduces direct support for text coded in UTF-8. The `utf2iso()` function is no longer required.
///
/// The software supports the screens
///
/// * iTC monochrome [with wide temperature and embedded fast update](https://www.pervasivedisplays.com/products/?_sft_etc_itc=pu&_sft_temperature_range=m15c-to-p60c&_sft_product_colour=black-white) (film `K`),
/// * iTC monochrome [with touch](https://www.pervasivedisplays.com/products/?_sft_etc_itc=tp),
/// * iTC colour [black-white-red-yellow](https://www.pervasivedisplays.com/products/?_sft_product_colour=black-white-red-yellow) (film `Q`);
///
/// the extension boards
///
/// * [EPD Extension Kit Gen 3 (EXT3 or EXT3.1)](https://www.pervasivedisplays.com/product/epd-extension-kit-gen-3-EXT3/),
/// * [Touch Expansion Board for EXT3 (EXT3-Touch)](https://www.pervasivedisplays.com/product/touch-expansion-board-ext3-touch/) for EXT3.1,
/// * [EPD Extension Kit Gen 4 (EXT4)](https://www.pervasivedisplays.com/product/epd-extension-kit-gen-4-EXT4/);
///
/// and the development kits
///
/// * [EPD Pico Kit (EPDK-266)](https://www.pervasivedisplays.com/product/epd-pico-kit-epdk/), which includes a Raspberry Pi Pico RP2040, an EXT3.1 extension board and a 2.66&quot; monochrome panel,
/// * [EPD Pico Kit Touch (EPDK-271-Touch)](https://www.pervasivedisplays.com/product/touch-expansion-board-ext3-touch/#tab-3), which includes an EXT3.1 extension board, an EXT3-Touch expansion board, and a 2.71&quot;-Touch monochrome panel with embedded fast update, and
/// * [E-Paper Development Kit for Matter (EPDK-Matter)](https://www.pervasivedisplays.com/product/epdk-matter/#tab-3), which bundles an Arduino Nano Matter, an EXT4 board and a 2.90&quot; high-definition monochrome panel with embedded fast update and wide temperature.
///
/// ## Copyright, licence and disclaimer
///
/// @date 21 Jan 2026
/// @version 1002
///
/// @copyright &copy; Pervasive Displays Inc., 2021-2026
/// @copyright &copy; Etigues, 2010-2026
/// @copyright All rights reserved
/// @copyright *For exclusive use with Pervasive Displays screens*
///
/// * **Basic edition**: for hobbyists and for basic usage
/// @n Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
/// ([summary](https://creativecommons.org/licenses/by-sa/4.0/deed.en) and [legal code](https://creativecommons.org/licenses/by-sa/4.0/legalcode.en))
///
/// Consider the Evaluation or Commercial editions for professionals or organisations and for commercial usage
///
/// * **Evaluation edition**: for professionals or organisations, evaluation only, no commercial usage
/// @n All rights reserved
///
/// * **Commercial edition**: for professionals or organisations, commercial usage
/// @n All rights reserved
///
/// * **Viewer edition**: for professionals or organisations
/// @n All rights reserved
///
/// * **Documentation**
/// @n All rights reserved
///
/// ## Disclaimer
///
/// The software, including the related documentation, is provided to you "as is," and we make no express or implied warranties whatsoever with respect to its functionality, operability, or use, including, without limitation, any implied warranties of merchantability, fitness for a particular purpose, or infringement. We expressly disclaim any liability whatsoever for any direct, indirect, consequential, incidental or special damages, including, without limitation, lost revenues, lost profits, losses resulting from business interruption or loss of data, regardless of the form of action or legal theory under which the liability may be asserted, even if advised of the possibility or likelihood of such damages.
///
/// @page Release10 New release 10
///
/// Release 10 of the  Basic edition supports UTF-8 text and keeps the 8-bit Terminal font with 255 characters, based on ISO-8859-1 or Latin 1.
///
/// `hV_Screen_Buffer::gText()` now accepts text with characters coded in UTF-8 and UTF-16. The `utf2iso()` function is no longer required.
///
/// @warning The following libraries are legacy and remain available.
/// * `PDLS_EXT4_Basic_Matter`
///
/// @deprecated The following features are deprecated.
/// * The `utf2iso()` function is deprecated.
///
/// @note The technical note [Migrate from release 9 to release 10](https://pdls.pervasivedisplays.com/userguide/Volume5/Chapter1/Part6/Section23/) provides more information.
///
/// @page Release9 Legacy release 9
///
/// Release 9 inaugurates a new structure based on drivers developed by Pervasive Displays.
///
/// @warning The following libraries are legacy and remain available.
/// * `PDLS_EXT3_Advanced_Global`
/// * `PDLS_EXT3_Advanced_Fast`, and
/// * `PDLS_EXT4_Advanced_Matter`.
///
/// @deprecated The following libraries and deprecated and replaced by `PDLS_Advanced` with a driver (9.0.0).
/// * `PDLS_EXT4_Advanced_BWRY` is replaced by `PDLS_Advanced` with the driver `Pervasive_BWRY_Small`,
/// * `PDLS_EXT4_Advanced_Wide` is replaced by `PDLS_Advanced` with the drivers `Pervasive_Wide_Small` or `Pervasive_Wide_Medium`, and
/// * `PDLS_EXT4_Advanced_Touch` is replaced by `PDLS_Advanced` with the driver `Pervasive_Touch_Small`.
///
/// @note The technical note [Migrate from release 8 to release 9](https://docs.pervasivedisplays.com/knowledge/Software/Volume5/Chapter1/Part6/Section22.html) provides more information.
///
/// The drivers are available at the [Pervasive Displays](https://github.com/PervasiveDisplays) GitHub repository.
///
/// @page Release8 Legacy release 8
///
/// Legacy release 8 supports the following screens, boards and kits.
///
/// Library | Supported screens, boards and kits
/// ---- | ----
/// [PDLS_EXT3_Basic_Global](https://github.com/rei-vilo/PDLS_EXT3_Basic_Global) | iTC monochrome screens [with normal update](https://www.pervasivedisplays.com/products/?_sft_product_colour=black-white) (film `C`)<br>iTC colour screens [black-white-red](https://www.pervasivedisplays.com/products/?_sft_product_colour=black-white-red) (film `J`);
/// [PDLS_EXT3_Basic_Fast](https://github.com/rei-vilo/PDLS_EXT3_Basic_Fast) | iTC monochrome screens [with embedded fast update](https://www.pervasivedisplays.com/products/?_sft_etc_itc=pu&_sft_product_colour=black-white) (film `P`);
/// [PDLS_EXT4_Basic_Matter](https://github.com/rei-vilo/PDLS_EXT4_Basic_Matter) | [EPDK-Matter](https://www.pervasivedisplays.com/product/epdk-matter/) and<br>[EXT4 Extension Board](https://www.pervasivedisplays.com/product/epd-extension-kit-gen-4-ext4/) and<br>iTC monochrome screens [with wide temperature and embedded fast update](https://www.pervasivedisplays.com/products-e-ink-display/?_sft_etc_itc=pu&_sft_temperature_range=m15c-to-p60c&_sft_product_colour=black-white) (film `K`).
///
/// @page Examples Examples
///
/// Examples are under the examples folders.
///
/// **Common**
///
/// * Common_Colours.ino
/// @image html T2_PALET.jpg
/// @image latex T2_PALET.jpg width=8cm
/// * Common_Fonts.ino
/// @image html T2_FONTS.jpg
/// @image latex T2_FONTS.jpg width=8cm
/// * Common_Orientation.ino
/// @image html T2_ORIEN.jpg
/// @image latex T2_ORIEN.jpg width=8cm
/// * Common_Forms.ino
/// @image html T2_FORMS.jpg
/// @image latex T2_FORMS.jpg width=8cm
/// * Common_Text.ino
/// @image html T2_CHARA.jpg
/// @image latex T2_CHARA.jpg width=8cm
/// * Common_WhoAmI.ino
/// @image html T2_WHOAMI.jpg
/// @image latex T2_WHOAMI.jpg width=8cm
///
/// **Black-White-Red-Yellow**
///
/// The examples below require a colour Black-White-Red-Yellow screen (film `Q`).
///
/// * BWRY_Colours.ino
/// @image html BWRY_Colours.jpg
/// @image latex BWRY_Colours.jpg width=8cm
/// * BWRY_Palette.ino
/// @image html BWRY_Palette.jpg
/// @image latex BWRY_Palette.jpg width=8cm
/// * BWRY_Contrasts.ino
/// @image html BWRY_Contrasts.jpg
/// @image latex BWRY_Contrasts.jpg width=8cm
///
/// **Wide**
///
/// The examples below require a screen with wide temperature and embedded fast update (film `K`).
///
/// * Fast_Line.ino
/// * Normal_Speed.ino
/// @image html Global.jpg
/// @image latex Global.jpg width=8cm
/// * Fast_Orientation.ino
/// * Fast_Speed.ino
/// @image html Fast.jpg
/// @image latex Fast.jpg width=8cm
/// * Fast_Temperature.ino
///
/// **EXT4**
///
/// The examples below require the EXT4 extension board.
///
/// * EXT4_Accelerometer.ino
/// @image html EXT4_8.gif
/// @image latex EXT4_8.gif width=8cm
/// * EXT4_LargeFont.ino
/// @image html EXT4_7.jpg
/// @image latex EXT4_7.jpg width=8cm
/// * EXT4_Weather.ino
/// @image html EXT4_3.jpg
/// @image latex EXT4_3.jpg width=8cm
/// * EXT4_WhoAmI.ino
/// * EXT4_WS2813C.ino
///
/// **EXT4 Matter**
///
/// The examples below require the EXT4 extension board and a controller board supported by the Silicon Labs Core for Arduino, like the Arduino Nano Matter.
///
/// * EXT4_Matter_RGB.ino
/// @image html EXT4_4.jpg
/// @image latex EXT4_4.jpg width=8cm
/// * EXT4_Matter_Weather.ino
/// @image html EXT4_3.jpg
/// @image latex EXT4_3.jpg width=8cm
///

