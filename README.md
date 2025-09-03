# Pervasive Displays Library Suite - Basic edition

[![Release](https://img.shields.io/github/v/release/rei-vilo/PDLS_Basic)](https://github.com/rei-vilo/PDLS_Basic/releases) [![Codacy Badge](https://app.codacy.com/project/badge/Grade/9f09780839f7479285cb931ffe4954d7)](https://app.codacy.com/gh/rei-vilo/PDLS_Basic/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade) ![Arduino lint](https://github.com/rei-vilo/PDLS_Basic/actions/workflows/main.yml/badge.svg) [![GitHub issues](https://img.shields.io/github/issues/pervasivedisplays/PDLS_Basic)](https://github.com/rei-vilo/PDLS_Basic/issues)

[![](https://img.shields.io/badge/-Knowledge_Base-orange)](https://docs.pervasivedisplays.com/knowledge/)

The Pervasive Displays Library Suite is specifically designed for the [Pervasive Displays](https://www.pervasivedisplays.com) e-paper screens, extension boards and evaluation kits.

![](img/Logo_PDI_text_320.png)

Release 9 inaugurates a new structure based on drivers developed by Pervasive Displays.

The **PDLS_Basic** edition supports the screens

* iTC [monochrome with wide temperature and embedded fast update](https://www.pervasivedisplays.com/products/?_sft_etc_itc=pu&_sft_temperature_range=m15c-to-p60c) (film `K`),
* iTC monochrome [with touch](https://www.pervasivedisplays.com/products/?_sft_etc_itc=tp), 
* iTC [black-white-red-yellow colour](https://www.pervasivedisplays.com/products/?_sft_product_colour=black-white-red-yellow) screens (film `Q`);

the extension boards

* [EPD Extension Kit Gen 3 (EXT3 or EXT3.1)](https://www.pervasivedisplays.com/product/epd-extension-kit-gen-3-EXT3/),
* [Touch Expansion Board for EXT3 (EXT3-Touch)](https://www.pervasivedisplays.com/product/touch-expansion-board-ext3-touch/) for EXT3.1,
* ![](https://img.shields.io/badge/-New-orange) Pervasive Displays [EPD Extension Kit Gen 4 (EXT4)](https://www.pervasivedisplays.com/product/epd-extension-kit-gen-4-EXT4/) and EXT4-to-Nano adapter boards;

and the development kits

* [EPD Pico Kit (EPDK)](https://www.pervasivedisplays.com/product/epd-pico-kit-epdk/), which includes a Raspberry Pi Pico RP2040, an EXT3-1 extension board and a 2.66" monochrome panel,
* [EPD Pico Kit Touch (EPDK-271-Touch)](https://www.pervasivedisplays.com/product/touch-expansion-board-ext3-touch/#tab-3), which includes an EXT3.1 extension board, an EXT3-Touch expansion board, and a 2.71"-Touch monochrome panel with embedded fast update and capacitive touch,
* ![](https://img.shields.io/badge/-New-orange) [E-Paper Development Kit for Matter (EPDK-Matter)](https://www.pervasivedisplays.com), which includes a Pervasive Displays [EPD Extension Kit Gen 4 (EXT4)](https://www.pervasivedisplays.com/product/epd-extension-kit-gen-4-EXT4/) and a Pervasive Displays EXT4-to-Nano adapter boards, a Pervasive Displays [2.9″ 384x168 E-ink monochrome high-definition](https://www.pervasivedisplays.com/product/2-9-e-ink-displays/) with embedded fast update and wide temperature display, an [Arduino Nano Matter](https://store.arduino.cc/pages/nano-matter) board based on [Silicon Labs MGM240S](https://www.silabs.com/wireless/zigbee/efr32mg24-series-2-modules).

Legacy release 8 supports the screens

* iTC monochrome [with normal update](https://www.pervasivedisplays.com/products/?_sft_product_colour=black-white) (film `C`), 
* iTC monochrome [with embedded fast update](https://www.pervasivedisplays.com/products/?_sft_etc_itc=pu&_sft_product_colour=black-white) (film `P`), and
* iTC colour [black-white-red](https://www.pervasivedisplays.com/products/?_sft_product_colour=black-white-red) (film `J`).

## Features

The Pervasive Displays Library Suite provides a high-level interface to drive the e-paper screens.

* Normal and fast update
* Graphics routines
* Text routines
* Four extended fonts with double-sized variants

## Documentation

* [Knowledge Base](https://docs.pervasivedisplays.com/knwoledge/)

## Support

* Please refer to the [Support](https://docs.pervasivedisplays.com/knowledge/Software/Volume5/index.html) page with FAQ, technical notes and reference designs.

* Report issues at the [PDLS_Basic](https://github.com/pervasivedisplays/PDLS_Basic/issues) repository.

## Installation

Software

* [Arduino IDE](https://www.arduino.cc/en/software) or [Arduino CLI](https://arduino.github.io/arduino-cli/)
* [Installing Additional Arduino Libraries](https://www.arduino.cc/en/guide/libraries)

    + Using the Library Manager
    + Importing a `.zip` Library

Hardware

* [Connecting the Pervasive Displays e-Paper EPD Extension Kit Gen 3](https://embeddedcomputing.weebly.com/connecting-the-e-paper-epd-extension-kit-gen-3.html)

## Configuration

Software

* Arduino SDK
* SPI library
* Wire library

Hardware

* Pervasive Displays EPD Extension Kit Gen 3 (EXT3 or EXT3.1)
* ![](https://img.shields.io/badge/-New-orange) Pervasive Displays EPD Extension Kit Gen 4 (EXT4)
* Pervasive Displays iTC black-white-red-yellow colour screens (film `Q`)
* Pervasive Displays iTC monochrome screens with wide temperature and embedded fast update (film `K`).

## Licence

**Copyright** &copy; Pervasive Displays Inc., 2021-2025, under licence by Rei Vilo

*For exclusive use with Pervasive Displays screens*

**Licence** [Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)](./LICENCE.md)

![](img/by-sa.svg)

**Copyright** &copy; Rei Vilo, 2010-2025

