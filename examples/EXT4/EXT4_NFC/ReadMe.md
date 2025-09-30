
# Example EXT4_NFC

  Project: PDLS_Basic

  Created by: Rei Vilo on 03 Jul 2025

  Copyright: © 2010 - 2025
  
  Licence: All rights reserved


## Configuration

+ Silicon Labs BG24 Explorer Kit with Mikroe NFC
+ Arduino Nano Matter with Pervasive Displays EXT4
+ Application on iOS or Android, one among
  + STMicrolectronics NFC Tap   
  + NXP TagWrite and NFC TagInfo  
  + NFC Tools 

The code provided uses I²C 8-bit address, while PDLS_Common uses 7-bit.

``` cpp
// I²C 8-bit address, PDLS_Common uses 7-bit addresses for I²C
#define NFC_DEFAULT_I2C_ADDR                0xAA
```

but

``` cpp
hV_HAL_Wire_transfer(NFC_DEFAULT_I2C_ADDR >> 1, pu8WriteBuffer, sizeof(pu8WriteBuffer));
```

Modified `hV_HAL_Wire_transfer()` now includes an optional delay between write and read and a non-blocking overtime check (release 9.1.1).

``` cpp
///
/// @brief Combined write and read with optional delay
///
/// @param[in] address I2C device address
/// @param[in] dataWrite buffer to write
/// @param[in] sizeWrite number of bytes
/// @param[out] dataRead buffer to read
/// @param[in] sizeRead number of bytes
/// @param[in] us delay in microseconds, default = 0 = no delay
/// @note If sizeRead = 0, no read performed
/// @warning No check for previous initialisation
///
void hV_HAL_Wire_transfer(uint8_t address, uint8_t * dataWrite, size_t sizeWrite, uint8_t * dataRead = 0, size_t sizeRead = 0, uint32_t us = 0);
```

## Results

As expected

```bash
hV = Begin

hV . Example_Basic_EXT4_NFC.ino
hV . Jul 10 2025 16:25:50
```

NFC controller has address 55h7 = AAh8.

```bash
hV - NFC_SetConfigurationAndSessionRegister
hV - NFC_SetCCAndLockChip
```

NFC controller intialisation.

```bash
hV = SPI ended
hV . OTP check passed - embedded PSR
hV - Screen iTC 2.90"-Wide iH
hV - Size 168x384
hV - Number 290-KS-0F
hV - Memory MCU buffer, Header fonts
hV - Driver Wide small v9.0.4
hV - PDLS Advanced v9.1.0

hV - Create frame-buffer [16128]
hV = SPI started at 8 MHz
hV = SPI changed to 16 MHz
```

EXT4 and 2.90 initialisation.

```bash
hV - NFC_ReadNDEF 'Send a text using NFC on your smartphone!'
hV - NFC_ReadNDEF 'This is a test of a text sent from an iPhone through NFC.'
hV . Regerenate screen
```

Read the tag from the iPhone.
Write the tag from the iPhone.
Press the green button to regenerate the screen.

