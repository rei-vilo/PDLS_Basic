///
/// @file EXT4_NFC.ino
/// @brief Example of features for EXT4 board and sensors - Basic edition
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @date 21 Jul 2025
/// @version 911
///
/// @copyright (c) Etigues, 2010-2025
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
/// @see
/// * NT3H2111_2211NTAG I2C plus: NFC Forum T2T with I2C interface, password protection and energy harvesting
/// https://www.nxp.com/products/rfid-nfc/nfc-hf/connected-nfc-tags/ntag-ic-plus-2k-nfc-forum-type-2-tag-with-ic-interface:NTAG_I2C
///

// SDK and configuration
#include "PDLS_Common.h"

// Board
pins_t myBoard = boardArduinoNanoMatter;
// pins_t myBoard = boardSiLabsBG24Explorer;

// Driver
#include "Pervasive_Wide_Small.h"
Pervasive_Wide_Small myDriver(eScreen_EPD_290_KS_0F, myBoard);

// Screen
#include "PDLS_Basic.h"
Screen_EPD myScreen(&myDriver);

uint8_t fontMedium;
uint16_t y = 0;

// Checks: Pervasive Displays EXT4 only
#if (USE_EXT_BOARD != BOARD_EXT4)
#error Required USE_EXT_BOARD = BOARD_EXT4
#endif // USE_EXT_BOARD

#if (hV_HAL_PERIPHERALS_RELEASE < 911)
#error Required hV_HAL_PERIPHERALS_RELEASE 911
#endif // hV_HAL_PERIPHERALS_RELEASE

// Set parameters
#define DISPLAY_EXT4_NFC 1

//
// --- NFC section
//
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

// defines some macros get from datasheet
// I²C 8-bit address, to be adapted as PDLS_Common uses 7-bit addresses for I²C
#define NFC_DEFAULT_I2C_ADDR 0xAA

#define NFC_CONFIGURATION_REGISTER_I2C_ADDR (0x3A)
#define NFC_SESSION_REGISTER_I2C_ADDR (0xFE)

#define NFC_BLOCK_SIZE (0x10)

#define NFC_SESSION_REGISTER_NC_REG_OFFSET (0x00)
#define NFC_SESSION_REGISTER_NS_REG_OFFSET (0x06)

// define a flag to print
#define PRINT (0)

// datasheet 9.8 show the set and get register method
static void NFC_SetSessionRegister(uint8_t u8REGA, uint8_t u8MASK, uint8_t u8REGDAT);
static uint8_t NFC_GetSessionRegister(uint8_t u8REGA);
// datasheet 9.7 show the write and read block method
static void NFC_WriteBlock(uint8_t u8BlockAddress, uint8_t * pu8DataBuffer);
static void NFC_ReadBlock(uint8_t u8BlockAddress, uint8_t * pu8DataBuffer);

void NFC_PowerOn() {};
void NFC_PowerOff() {};

void NFC_SetSessionRegister(uint8_t u8REGA, uint8_t u8MASK, uint8_t u8REGDAT)
{
    uint8_t pu8WriteBuffer[4];
    pu8WriteBuffer[0] = NFC_SESSION_REGISTER_I2C_ADDR;
    pu8WriteBuffer[1] = u8REGA;
    pu8WriteBuffer[2] = u8MASK;
    pu8WriteBuffer[3] = u8REGDAT;

    hV_HAL_Wire_transfer(NFC_DEFAULT_I2C_ADDR >> 1, pu8WriteBuffer, sizeof(pu8WriteBuffer));
}

uint8_t NFC_GetSessionRegister(uint8_t u8REGA)
{
    uint8_t u8ReadBuffer;
    uint8_t pu8WriteBuffer[2];
    pu8WriteBuffer[0] = NFC_SESSION_REGISTER_I2C_ADDR;
    pu8WriteBuffer[1] = u8REGA;

    hV_HAL_Wire_transfer(NFC_DEFAULT_I2C_ADDR >> 1,
                         pu8WriteBuffer, sizeof(pu8WriteBuffer),
                         &u8ReadBuffer, 1,
                         100);

    return u8ReadBuffer;
}

void NFC_WriteBlock(uint8_t u8BlockAddress, uint8_t * pu8DataBuffer)
{
    uint8_t pu8WriteBuffer[NFC_BLOCK_SIZE + 1];

    pu8WriteBuffer[0] = u8BlockAddress;
    memcpy(pu8WriteBuffer + 1, pu8DataBuffer, 16);

    hV_HAL_Wire_transfer(NFC_DEFAULT_I2C_ADDR >> 1,
                         pu8WriteBuffer, sizeof(pu8WriteBuffer));

    hV_HAL_delayMicroseconds(5000);
}

void NFC_ReadBlock(uint8_t u8BlockAddress, uint8_t * pu8DataBuffer)
{
    hV_HAL_Wire_transfer(NFC_DEFAULT_I2C_ADDR >> 1,
                         &u8BlockAddress, 1,
                         pu8DataBuffer, NFC_BLOCK_SIZE,
                         100);
}


void NFC_SetConfigurationAndSessionRegister(void)
{
    NFC_PowerOn();

    uint8_t pu8ConfigurationRegisterDefaultValue[NFC_BLOCK_SIZE] = {0x19, 0x00, 0xF8, 0x48, 0x08, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    NFC_WriteBlock(NFC_CONFIGURATION_REGISTER_I2C_ADDR, pu8ConfigurationRegisterDefaultValue);
    NFC_SetSessionRegister(NFC_SESSION_REGISTER_NS_REG_OFFSET, 0xFF, 0x00);
    hV_HAL_delayMicroseconds(10 * 1000);

    NFC_PowerOff();
}

void NFC_SetCCAndLockChip(void)
{
    NFC_PowerOn();

    uint8_t pu8Block0x00[NFC_BLOCK_SIZE] = {0};
    NFC_ReadBlock(0x00, pu8Block0x00);
    // "capability container" (CC) configuration: 0xE1, 0x10, 0x6D, 0x00
    // "static lock bytes" configuration: 0x0F, 0x00
    // set pu8Block0x00 the default I2C address at offset 0
    pu8Block0x00[0]  = NFC_DEFAULT_I2C_ADDR;
    // the "static lock bytes" at offset 10, 11; for more information can see "8.3.6 Static Lock Bytes"
    pu8Block0x00[10] = 0x0F;
    pu8Block0x00[11] = 0x00;
    // the "capability container" at offset 12, 13, 14, 15; for more information can see "8.3.8 Capability Container (CC)" and "8.3.10 Memory content at delivery"
    pu8Block0x00[12] = 0xE1;
    pu8Block0x00[13] = 0x10;
    pu8Block0x00[14] = 0x6D;
    pu8Block0x00[15] = 0x00;
    // set "capability container" and "static lock bytes"
    NFC_WriteBlock(0x00, pu8Block0x00);

    uint8_t pu8Block0x38[NFC_BLOCK_SIZE] = {0};
    NFC_ReadBlock(0x38, pu8Block0x38);
    // the "dynamic lock bytes" at offset 8, 9, 10; for more information can see "8.3.7 Dynamic Lock Bytes"
    pu8Block0x38[8]  = 0x00;
    pu8Block0x38[9]  = 0x00;
    pu8Block0x38[10] = 0x7F;
    // set "dynamic lock bytes"
    NFC_WriteBlock(0x38, pu8Block0x38);

    NFC_PowerOff();
}

// the mask of SR flag in u8RecordFlags
#define SR_FLAG_MSK                  (0x10)

// Language Code length
#define LANGUAGE_CODE_LENGTH         (3)

// NDEF format length
#define SHORT_NDEF_FORMAT_LENGTH     (4)
#define LONG_NDEF_FORMAT_LENGTH      (7)

// TLV format length
#define SHORT_TLV_FORMAT_LENGTH      (2)
#define LONG_TLV_FORMAT_LENGTH       (4)

// TLV fortmat terminator length
#define TLV_FORMAT_TERMINATOR_LENGTH (1)

// max TLV field(include TLV terminator) size is 888 bytes, for page 4-225
#define TLV_FIELD_MAX_SIZE           (888)
// short NDEF message max size
#define SHORT_NDEF_MESSAGE_MAX_SIZE  (255)
// long NDEF message max size
// max TLV field(include TLV terminator) size is 888. For long TLV format,
// there are 4 bytes TLV header; for long NDEF message, there are 7 bytes
// NDEF header; for terminator, there is 1 byte.
#define LONG_NDEF_MESSAGE_MAX_SIZE \
  (TLV_FIELD_MAX_SIZE - LONG_TLV_FORMAT_LENGTH - LONG_NDEF_FORMAT_LENGTH - TLV_FORMAT_TERMINATOR_LENGTH)

// If NDEF message length <= 0xFF, use short NDEF format, the value of u8PayloadLength is the NDEF message length.
// SR flag in u8RecordFlags will be set to 1.
// If NDEF message length > 0xFF, use long NDEF format, the value of u8PayloadLength[1-4] indicates the NDEF message length.
// SR flag in u8RecordFlags will be set to 0.

// u8NdefMessage[0] is Language Code length, if it's 0x02 means that u8NdefMessage[1] and u8NdefMessage[2] are Language
// Code, so get NDEF message begin from u8NdefMessage[3].

typedef struct
{
    uint8_t u8RecordFlags;
    uint8_t u8TypeLength;
    uint8_t u8PayloadLength;
    uint8_t u8Type;
    uint8_t u8NdefMessage[SHORT_NDEF_MESSAGE_MAX_SIZE + TLV_FORMAT_TERMINATOR_LENGTH]; // plus 1 byte for terminator 0xFE
} T_SHORT_NDEF_FORMAT;

typedef struct
{
    uint8_t u8RecordFlags;
    uint8_t u8TypeLength;
    uint8_t u8PayloadLength[4];
    uint8_t u8Type;
    uint8_t u8NdefMessage[LONG_NDEF_MESSAGE_MAX_SIZE + TLV_FORMAT_TERMINATOR_LENGTH];  // plus 1 byte for terminator 0xFE
} T_LONG_NDEF_FORMAT;

// TLV format is Tag Field(1 bytes) + Length Field(1 or 3 bytes) + Value Field + TLV format terminator(0xFE).
// The value of LengthField is the size of ValueField. In our struct, put TLV terminator in u8NdefMessage, but
// when calculate NDEF format size or NDEF message size ignore it.

// If NDEF format length < 0xFF, use 1 byte Length Field TLV format, the value of u8LengthField is the NDEF format length.
// If NDEF format length >= 0xFF, use 3 bytes Length Field TLV format, set u8LengthField[0] to 0xFF and the value of
// u8LengthField[1-2] indicates the NDEF format length.

// NDEF format length < 0xFF
typedef union
{
    struct
    {
        uint8_t u8TagField;
        uint8_t u8LengthField;
        // value field
        T_SHORT_NDEF_FORMAT tShortNdefFormat;
    } tTlvField;
    // max size is 2+4+255+1=262
    uint8_t u8Data[SHORT_TLV_FORMAT_LENGTH + SHORT_NDEF_FORMAT_LENGTH + SHORT_NDEF_MESSAGE_MAX_SIZE + TLV_FORMAT_TERMINATOR_LENGTH];
} T_SHORT_NDEF_SHORT_TLV;

// NDEF format length >= 0xFF and NDEF message length <= 0xFF
typedef union
{
    struct
    {
        uint8_t u8TagField;
        uint8_t u8LengthField[3];
        T_SHORT_NDEF_FORMAT tShortNdefFormat;
    } tTlvField;
    // max size is 4+4+255+1=264
    uint8_t u8Data[LONG_TLV_FORMAT_LENGTH + SHORT_NDEF_FORMAT_LENGTH + SHORT_NDEF_MESSAGE_MAX_SIZE + TLV_FORMAT_TERMINATOR_LENGTH];
} T_SHORT_NDEF_LONG_TLV;

// NDEF message > 0xFF
typedef union
{
    struct
    {
        uint8_t u8TagField;
        uint8_t u8LengthField[3];
        T_LONG_NDEF_FORMAT tLongNdefFormat;
    } tTlvField;
    // max size is 4+7+(888-4-7-1)+1=888
    uint8_t u8Data[LONG_TLV_FORMAT_LENGTH + LONG_NDEF_FORMAT_LENGTH + LONG_NDEF_MESSAGE_MAX_SIZE + TLV_FORMAT_TERMINATOR_LENGTH];
} T_LONG_NDEF_LONG_TLV;

T_SHORT_NDEF_SHORT_TLV tShortNdefAndTlv;
T_SHORT_NDEF_LONG_TLV tShortNdefAndLongTlv;
T_LONG_NDEF_LONG_TLV tLongNdefAndTlv;

uint32_t i;

void NFC_WriteNDEF(char const * pu8NdefMessage, uint32_t u32UserNdefMessageLength)
{
    // check user NDEF message length
    if (u32UserNdefMessageLength + LANGUAGE_CODE_LENGTH > LONG_NDEF_MESSAGE_MAX_SIZE)
    {
        return;
    }
    NFC_PowerOn();

    uint8_t pu8WriteBuffer[NFC_BLOCK_SIZE] = {0};
    uint32_t u32BytesToWrite;
    uint32_t u32BlocksToWrite;

    uint32_t u32NdefMessageLength =
        u32UserNdefMessageLength + LANGUAGE_CODE_LENGTH;  // plus Language Code(0x02, 0x65, 0x6E) length
    uint32_t u32NedfFormatLength;

    // short NDEF format
    if (u32NdefMessageLength <= 0xFF)
    {
        u32NedfFormatLength = u32NdefMessageLength + SHORT_NDEF_FORMAT_LENGTH;  // plus short NDEF header length
        // short TLV format
        if (u32NedfFormatLength < 0xFF)
        {
            memset(tShortNdefAndTlv.u8Data, 0x00, sizeof(tShortNdefAndTlv.u8Data));
            tShortNdefAndTlv.tTlvField.u8TagField                        = 0x03;
            tShortNdefAndTlv.tTlvField.u8LengthField                     = (uint8_t)(u32NedfFormatLength & 0xFF);
            tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8RecordFlags    = 0xD1;
            tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8TypeLength     = 0x01;
            tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8PayloadLength  = (uint8_t)(u32NdefMessageLength & 0xFF);
            tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8Type           = 0x54;  // 0x54 indicates a Text Record
            tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8NdefMessage[0] = 0x02;
            tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8NdefMessage[1] = 0x65;
            tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8NdefMessage[2] = 0x6E;
            memcpy(&tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8NdefMessage[3],
                   pu8NdefMessage,
                   u32UserNdefMessageLength);
            tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8NdefMessage[3 + u32UserNdefMessageLength] = 0xFE;

            u32BytesToWrite = u32NedfFormatLength + SHORT_TLV_FORMAT_LENGTH +
                              TLV_FORMAT_TERMINATOR_LENGTH;  // plus short TLV header length and terminator length

            memcpy(tLongNdefAndTlv.u8Data,
                   tShortNdefAndTlv.u8Data,
                   u32BytesToWrite);  // copy to tLongNdefAndTlv.u8Data for writing
        }
        // long TLV format
        else
        {
            memset(tShortNdefAndLongTlv.u8Data, 0x00, sizeof(tShortNdefAndLongTlv.u8Data));
            tShortNdefAndLongTlv.tTlvField.u8TagField       = 0x03;
            tShortNdefAndLongTlv.tTlvField.u8LengthField[0] = 0xFF;
            tShortNdefAndLongTlv.tTlvField.u8LengthField[1] = (uint8_t)((u32NedfFormatLength >> 8) & 0xFF);
            tShortNdefAndLongTlv.tTlvField.u8LengthField[2] = (uint8_t)(u32NedfFormatLength & 0xFF);
            tShortNdefAndLongTlv.tTlvField.tShortNdefFormat.u8RecordFlags = 0xD1;
            tShortNdefAndLongTlv.tTlvField.tShortNdefFormat.u8TypeLength  = 0x01;
            tShortNdefAndLongTlv.tTlvField.tShortNdefFormat.u8PayloadLength =
                (uint8_t)(u32NdefMessageLength & 0xFF);
            tShortNdefAndLongTlv.tTlvField.tShortNdefFormat.u8Type = 0x54;  // 0x54 indicates a Text Record
            tShortNdefAndLongTlv.tTlvField.tShortNdefFormat.u8NdefMessage[0] = 0x02;
            tShortNdefAndLongTlv.tTlvField.tShortNdefFormat.u8NdefMessage[1] = 0x65;
            tShortNdefAndLongTlv.tTlvField.tShortNdefFormat.u8NdefMessage[2] = 0x6E;
            memcpy(&tShortNdefAndLongTlv.tTlvField.tShortNdefFormat.u8NdefMessage[3],
                   pu8NdefMessage,
                   u32UserNdefMessageLength);
            tShortNdefAndLongTlv.tTlvField.tShortNdefFormat.u8NdefMessage[3 + u32UserNdefMessageLength] = 0xFE;

            u32BytesToWrite = u32NedfFormatLength + LONG_TLV_FORMAT_LENGTH +
                              TLV_FORMAT_TERMINATOR_LENGTH;  // plus long TLV header length and terminator length

            memcpy(tLongNdefAndTlv.u8Data,
                   tShortNdefAndLongTlv.u8Data,
                   u32BytesToWrite);  // copy to tLongNdefAndTlv.u8Data for writing
        }
    }
    // long NDEF format and TLV format
    else
    {
        u32NedfFormatLength = u32NdefMessageLength + LONG_NDEF_FORMAT_LENGTH;  // plus long NDEF header length

        memset(tLongNdefAndTlv.u8Data, 0x00, sizeof(tLongNdefAndTlv.u8Data));
        tLongNdefAndTlv.tTlvField.u8TagField                    = 0x03;
        tLongNdefAndTlv.tTlvField.u8LengthField[0]              = 0xFF;
        tLongNdefAndTlv.tTlvField.u8LengthField[1]              = (uint8_t)((u32NedfFormatLength >> 8) & 0xFF);
        tLongNdefAndTlv.tTlvField.u8LengthField[2]              = (uint8_t)(u32NedfFormatLength & 0xFF);
        tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8RecordFlags = 0xC1;
        tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8TypeLength  = 0x01;
        tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8PayloadLength[0] =
            (uint8_t)((u32NdefMessageLength >> 24) & 0xFF);
        tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8PayloadLength[1] =
            (uint8_t)((u32NdefMessageLength >> 16) & 0xFF);
        tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8PayloadLength[2] =
            (uint8_t)((u32NdefMessageLength >> 8) & 0xFF);
        tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8PayloadLength[3] = (uint8_t)(u32NdefMessageLength & 0xFF);
        tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8Type             = 0x54;  // 0x54 indicates a Text Record
        tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8NdefMessage[0]   = 0x02;
        tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8NdefMessage[1]   = 0x65;
        tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8NdefMessage[2]   = 0x6E;
        memcpy(&tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8NdefMessage[3],
               pu8NdefMessage,
               u32UserNdefMessageLength);
        tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8NdefMessage[3 + u32UserNdefMessageLength] = 0xFE;

        u32BytesToWrite = u32NedfFormatLength + LONG_TLV_FORMAT_LENGTH +
                          TLV_FORMAT_TERMINATOR_LENGTH;  // plus long TLV header length and terminator length
    }

    u32BlocksToWrite = (u32BytesToWrite / NFC_BLOCK_SIZE) + ((u32BytesToWrite % NFC_BLOCK_SIZE) > 0 ? 1 : 0);
    for (i = 0; i < u32BlocksToWrite; i++)
    {
        memcpy(pu8WriteBuffer, tLongNdefAndTlv.u8Data + NFC_BLOCK_SIZE * i, NFC_BLOCK_SIZE);
        NFC_WriteBlock(0x01 + i, pu8WriteBuffer);
        memset(pu8WriteBuffer, 0x00, NFC_BLOCK_SIZE);
    }

    NFC_PowerOff();
}

uint16_t NFC_ReadNDEF(char * buffer)
{
    uint8_t result = 0;
    NFC_PowerOn();

    uint8_t pu8ReadBuffer[NFC_BLOCK_SIZE] = {0};
    NFC_ReadBlock(0x01, pu8ReadBuffer);

    uint32_t u32BytesToRead =
        (pu8ReadBuffer[1] < 0xFF ? (SHORT_TLV_FORMAT_LENGTH + pu8ReadBuffer[1]) :
         (LONG_TLV_FORMAT_LENGTH + pu8ReadBuffer[2] * 256 + pu8ReadBuffer[3])) +
        TLV_FORMAT_TERMINATOR_LENGTH;  // plus TLV header length and terminator length
    uint8_t u32BlocksToRead =
        (u32BytesToRead / NFC_BLOCK_SIZE) + ((u32BytesToRead % NFC_BLOCK_SIZE) > 0 ? 1 : 0);
    // read all TLV field data and put it in tLongNdefAndTlv temporarily
    memset(tLongNdefAndTlv.u8Data, 0x00, sizeof(tLongNdefAndTlv.u8Data));
    for (i = 0; i < u32BlocksToRead; i++)
    {
        memset(pu8ReadBuffer, 0x00, NFC_BLOCK_SIZE);
        NFC_ReadBlock(0x01 + i, pu8ReadBuffer);
        memcpy(tLongNdefAndTlv.u8Data + NFC_BLOCK_SIZE * i, pu8ReadBuffer, NFC_BLOCK_SIZE);
    }

    // short TLV format
    if (tLongNdefAndTlv.tTlvField.u8LengthField[0] < 0xFF)
    {
        memset(tShortNdefAndTlv.u8Data, 0x00, sizeof(tShortNdefAndTlv.u8Data));
        memcpy(tShortNdefAndTlv.u8Data, tLongNdefAndTlv.u8Data, u32BytesToRead);
#if PRINT
        uint32_t u32NdefMessageLength =
            u32BytesToRead - SHORT_TLV_FORMAT_LENGTH - SHORT_NDEF_FORMAT_LENGTH -
            TLV_FORMAT_TERMINATOR_LENGTH;  // subtract TLV header, NDEF header and Terminator length

        for (i = tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8NdefMessage[0] + 1; i < u32NdefMessageLength; i++)
        {
            hV_HAL_log(LEVEL_INFO, "0x%02x '%c'", tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8NdefMessage[i], tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8NdefMessage[i]);
        }
#endif

        if (tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8Type == 'T') // Text record
        {
            uint8_t * u32NdefMessageStart = tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8NdefMessage + tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8NdefMessage[0] + 1;
            uint32_t u32NdefMessageLength = u32BytesToRead - SHORT_TLV_FORMAT_LENGTH - SHORT_NDEF_FORMAT_LENGTH - TLV_FORMAT_TERMINATOR_LENGTH; // subtract TLV header, NDEF header and Terminator length
            u32NdefMessageLength -= LANGUAGE_CODE_LENGTH;
            memcpy(buffer, u32NdefMessageStart, u32NdefMessageLength);
            // buffer[u32NdefMessageLength - 1] = 0x00;
            result = u32NdefMessageLength;
        }

        // !!! Add URI and WiFi records

        // if (tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8Type == 'U') // URI record
        // {
        //     uint8_t * u32NdefMessageStart = tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8NdefMessage + tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8NdefMessage[0];
        //     uint32_t u32NdefMessageLength = u32BytesToRead - SHORT_TLV_FORMAT_LENGTH - SHORT_NDEF_FORMAT_LENGTH - TLV_FORMAT_TERMINATOR_LENGTH; // subtract TLV header, NDEF header and Terminator length
        //     u32NdefMessageLength -= LANGUAGE_CODE_LENGTH;
        //     strcpy(buffer, "http://www.");
        //     strcat(buffer, (const char*)u32NdefMessageStart);
        // }
    }
    // long TLV format
    else
    {
        // short NDEF format
        if ((tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8RecordFlags & SR_FLAG_MSK) == SR_FLAG_MSK)
        {
            memset(tShortNdefAndLongTlv.u8Data, 0x00, sizeof(tShortNdefAndLongTlv.u8Data));
            memcpy(tShortNdefAndLongTlv.u8Data, tLongNdefAndTlv.u8Data, u32BytesToRead);
#if PRINT
            uint32_t u32NdefMessageLength =
                u32BytesToRead - LONG_TLV_FORMAT_LENGTH - SHORT_NDEF_FORMAT_LENGTH -
                TLV_FORMAT_TERMINATOR_LENGTH;  // subtract TLV header, NDEF header and Terminator length

            for (i = tShortNdefAndLongTlv.tTlvField.tShortNdefFormat.u8NdefMessage[0] + 1; i < u32NdefMessageLength;
                    i++)
            {
                hV_HAL_log(LEVEL_INFO, "0x%02x '%c'", tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8NdefMessage[i], tShortNdefAndTlv.tTlvField.tShortNdefFormat.u8NdefMessage[i]);
            }
#endif
            hV_HAL_log(LEVEL_INFO, "'%s'", tShortNdefAndLongTlv.u8Data);
        }
        // long NDEF format
        else
        {
#if PRINT
            uint32_t u32NdefMessageLength =
                u32BytesToRead - LONG_TLV_FORMAT_LENGTH - LONG_NDEF_FORMAT_LENGTH -
                TLV_FORMAT_TERMINATOR_LENGTH;  // subtract TLV header, NDEF header and Terminator length

            for (i = tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8NdefMessage[0] + 1; i < u32NdefMessageLength; i++)
            {
                hV_HAL_log(LEVEL_INFO, "%c", tLongNdefAndTlv.tTlvField.tLongNdefFormat.u8NdefMessage[i]);
            }
#endif
            hV_HAL_log(LEVEL_INFO, "'%s'", tShortNdefAndLongTlv.u8Data);
        }
    }

    NFC_PowerOff();
    return result;
}

static const char kNedfMessage[] = "Send a text using NFC on your smartphone!";

void NTAG_TestNdef(void)
{
    NFC_SetConfigurationAndSessionRegister();
    NFC_SetCCAndLockChip();
    NFC_WriteNDEF(kNedfMessage, strlen(kNedfMessage));
    char buffer[64] = {0};
    uint8_t result = NFC_ReadNDEF(buffer);
    if (result > 0)
    {
        hV_HAL_log(LEVEL_INFO, "'%s'", buffer);
    }
    // while (1);

    hV_HAL_exit();
}

void NFC_ResetI2CAddress(void)
{
    uint8_t u8DataBuff[NFC_BLOCK_SIZE] = {0};
    uint8_t u8BlockAddr                = 0x00;
    uint8_t u8NtagI2CAddress           = NFC_DEFAULT_I2C_ADDR; // 8-bit address
    bool bResetI2CAddr               = false;
    bool bNtagI2CAck                 = true;

    hV_HAL_log(LEVEL_DEBUG, "Check address");

    do
    {
        bNtagI2CAck = true;
        // Try to read block 0
        // bNtagI2CAck &= I2C_Write(u8NtagI2CAddress, &u8BlockAddr, 1);
        // bNtagI2CAck &= I2C_Read(u8NtagI2CAddress, u8DataBuff, NFC_BLOCK_SIZE);

        Wire.beginTransmission(u8NtagI2CAddress >> 1);
        Wire.write(u8BlockAddr);
        uint8_t error = Wire.endTransmission();

        memset(u8DataBuff, 0x00, sizeof(u8DataBuff));
        Wire.requestFrom(u8NtagI2CAddress >> 1, NFC_BLOCK_SIZE);
        for (uint8_t index = 0; index < NFC_BLOCK_SIZE; index++)
        {
            u8DataBuff[index] = Wire.read();
        }

        // check if the NTAG did not respond with ACK
        // if (!bNtagI2CAck)
        if (error != 0)
        {
            // try next I2C address (7 bit SA + '0')
            u8NtagI2CAddress += 2;
        }
        else
        {
            hV_HAL_log(LEVEL_DEBUG, "Found at %02h7, %02xh8", u8NtagI2CAddress >> 1, u8NtagI2CAddress);
            // Reset I2C addr if u8NtagI2CAddress is not the default one
            if (NFC_DEFAULT_I2C_ADDR != u8NtagI2CAddress)
            {
                bResetI2CAddr = true;
            }
            break;
        }
        // loop until we have wrapped around to default I2C address again
    }
    while (u8NtagI2CAddress != NFC_DEFAULT_I2C_ADDR);

    bResetI2CAddr = true;
    u8NtagI2CAddress = 0x2a * 2;

    if (bResetI2CAddr)
    {
        uint8_t error = 0;
        // read block
        hV_HAL_Wire_transfer(u8NtagI2CAddress >> 1, &u8BlockAddr, 1, u8DataBuff, NFC_BLOCK_SIZE, 100);

        u8DataBuff[0] = NFC_DEFAULT_I2C_ADDR;

        // write block
        uint8_t pu8WriteBuffer[NFC_BLOCK_SIZE + 1];
        pu8WriteBuffer[0] = u8BlockAddr;
        memcpy(pu8WriteBuffer + 1, u8DataBuff, 16);
        hV_HAL_Wire_transfer(u8NtagI2CAddress >> 1, pu8WriteBuffer, sizeof(pu8WriteBuffer));

        // Timer_DelayMicroSeconds(5000);
        hV_HAL_delayMilliseconds(5);

        hV_HAL_log(LEVEL_DEBUG, "Set at %02h7, %02h8", u8DataBuff[0] >> 1, u8DataBuff[0]);
    }
}
//
// --- End of NFC section
//

void setup()
{
    hV_HAL_begin();
    hV_HAL_Wire_begin();

    hV_HAL_Serial_crlf();
    hV_HAL_log(LEVEL_INFO, __FILE__);
    hV_HAL_log(LEVEL_INFO, __DATE__ " " __TIME__);
    hV_HAL_Serial_crlf();

    // NFC_ResetI2CAddress();
    // NTAG_TestNdef();

    // configure NTAG
    NFC_SetConfigurationAndSessionRegister();
    hV_HAL_log(LEVEL_DEBUG, "NFC_SetConfigurationAndSessionRegister");
    NFC_SetCCAndLockChip();
    hV_HAL_log(LEVEL_DEBUG, "NFC_SetCCAndLockChip");

    // If EXT4 has written NDEF message to NTAG, use TagInfo App can read it directly, no further operation required for EXT4
    NFC_WriteNDEF(kNedfMessage, strlen(kNedfMessage));
    hV_HAL_log(LEVEL_DEBUG, "NFC_WriteNDEF '%s'", kNedfMessage);
    hV_HAL_Serial_crlf();

    // Screen
    myScreen.begin();
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);
    myScreen.regenerate();

    // Fonts
    myScreen.selectFont(Font_Terminal8x12);

    y = 0;
    myScreen.gText(0, y, utf2iso(kNedfMessage));
    y += myScreen.characterSizeY();
    myScreen.flushFast();
}

void loop()
{
    if (hV_HAL_GPIO_read(myBoard.nfcFD) == LOW)
    {
        while (hV_HAL_GPIO_read(myBoard.nfcFD) == LOW)
        {
            hV_HAL_delayMilliseconds(100);
        }

        char buffer[64] = {0};
        uint8_t result = NFC_ReadNDEF(buffer);
        if (result > 0)
        {
            hV_HAL_log(LEVEL_DEBUG, "NFC_ReadNDEF '%s'", buffer);

            if (y > myScreen.screenSizeY())
            {
                myScreen.clear();
                y = 0;
            }
            myScreen.gText(0, y, utf2iso(buffer));
            y += myScreen.characterSizeY();
            myScreen.flushFast();
        }
    }

    if (digitalRead(myBoard.button) == LOW)
    {
        myScreen.regenerate();
        hV_HAL_log(LEVEL_INFO, "Regerenate screen");
        y = 0;
        NFC_WriteNDEF(kNedfMessage, strlen(kNedfMessage));

        myScreen.gText(0, y, utf2iso(kNedfMessage));
        y += myScreen.characterSizeY();
    }
}
