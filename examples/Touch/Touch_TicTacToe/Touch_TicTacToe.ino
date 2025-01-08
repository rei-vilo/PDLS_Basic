///
/// @file Touch_TicTacToe.ino
/// @brief Example of features for basic edition
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Jan 2025
/// @version 902
///
/// @copyright (c) Rei Vilo, 2010-2025
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

// SDK
// #include <Arduino.h>
#include "PDLS_Common.h"

// Include application, user and local libraries
// #include <Wire.h>
// #include <SPI.h>

// Driver
#include "Pervasive_Touch_Small.h"

// Screen
#include "PDLS_Basic.h"

#if (SCREEN_EPD_RELEASE < 902)
#error Required SCREEN_EPD_RELEASE 902
#endif // SCREEN_EPD_RELEASE

// Set parameters
#define PLAYER_NONE 0
#define PLAYER_HUMAN 1
#define PLAYER_MCU 2

#define NUMBER 4

// Define structures and classes

// Define variables and constants
// Driver
// Driver_EPD myDriver(eScreen_EPD_370_PS_0C_Touch, boardRaspberryPiPico_RP2040);
Pervasive_Touch_Small myDriver(eScreen_EPD_271_KS_09_Touch, boardRaspberryPiPico_RP2040);

// Screen
Screen_EPD myScreen(&myDriver);

struct coordinates_s
{
    uint8_t i;
    uint8_t j;
};

coordinates_s coordinates;
uint8_t fontVery, fontLarge, fontMedium, fontSmall;

// Define variables and constants
uint16_t x, y, dx, dy;
touch_t myTouch;
uint16_t colourHuman, colourMCU, colourGrid, colourBackground, colourMessage;

#ifndef WITH_TOUCH
#error Required WITH_TOUCH
#endif // WITH_TOUCH

// Prototypes
///
/// @brief Check whether palyer is winner
/// @param player player number
/// @return true if winner, false otherwise
///
bool checkWinnerPlayer(uint8_t player);

///
/// @brief Draw winning line, column or diagonal
/// @param player player number
///
void drawWinnerPlayer(uint8_t player);

///
/// @brief Check the winner
/// @return winner player number, 0 if none
///
uint8_t checkWinner();

///
/// @brief Display the move of player
/// @param coordinates move coordinates
/// @param player player number
///
void drawMovePlayer(coordinates_s coordinates, uint8_t player);

///
/// @brief Let the MCU play
///
void moveMCU();

///
/// @brief Check human play
/// @param coordinates coordinates of the cell to block
/// @return true if cell identified, false otherwise
///
bool checkHuman(coordinates_s & coordinates);

///
/// @brief Print board on serial
///
void printBoard();

///
/// @brief Play the game
///
void playGame();

///
/// @brief Let the human play
///
void moveHuman();

///
/// @brief Reset the game
///
void resetGame();

///
/// @brief Draw start screen
///
void drawStartScreen();

///
/// @brief Draw empty board
///
void drawGameScreen();

///
/// @brief Draw end of game
///
void drawGameOverScreen();

///
/// @brief Get board cell content
/// @param coordinates coordinates i j of cell
/// @return player number
///
uint8_t getBoard(coordinates_s coordinates);

///
/// @brief Set board cell content to player
/// @param coordinates coordinates i j of cell
/// @param player player number
/// @return true is cell updated, false otherwise
/// @note Targeted cell should be empty
///
bool setBoard(coordinates_s coordinates, uint8_t player);

// Utilities
///
/// @brief Display text centered on coordinates
/// @param x0 x coordinate
/// @param y0 y coordinate
/// @param text text
/// @param colour colour
///
void displayCenteredText(uint16_t x0, uint16_t y0, STRING_CONST_TYPE text, uint16_t colour)
{
    uint16_t dz = myScreen.characterSizeY() / 2;
    myScreen.setPenSolid(true);
    myScreen.dRectangle(0, y0 - dz, myScreen.screenSizeX(), 2 * dz, colourBackground);

    uint16_t dX = x0 - myScreen.stringSizeX(text) / 2;
    uint16_t dY = y0 - myScreen.characterSizeY() / 2;
    myScreen.gText(dX, dY, text, colourMessage);
}

uint8_t fsmScreen = 1;
uint8_t moves = 1;

uint8_t winner = 0; // 0 = Draw, 1 = Human, 2 = MCU

uint8_t board[NUMBER][NUMBER]; // holds position data 0 is blank, 1 human, 2 is MCU

uint16_t minX, maxX, minY, maxY, sizeTable, sizeCell;

// Utilities
bool setBoard(coordinates_s coordinates, uint8_t player)
{
    if (board[coordinates.i][coordinates.j] == PLAYER_NONE)
    {
        board[coordinates.i][coordinates.j] = player;
        return true;
    }
    else
    {
        return false;
    }
}

void resetGame()
{
    for (uint8_t i = 0; i < NUMBER; i++)
    {
        for (uint8_t j = 0; j < NUMBER; j++)
        {
            board[i][j] = 0;
        }
    }
    moves = 1;
    winner = PLAYER_NONE;
    fsmScreen = 2;
}

void drawStartScreen()
{
    // White background
    myScreen.clear(colourBackground);

    // Print "Tic Tac Toe" Text
    displayCenteredText(minX + sizeTable / 2, minY + sizeTable * 1 / 4, "Tic Tac Toe", colourMessage);
    displayCenteredText(minX + sizeTable / 2, minY + sizeTable * 3 / 4, "Touch to start", colourMessage);

    // Draw frame
    myScreen.setPenSolid(false);
    myScreen.rectangle(minX, minY, maxX - 1, maxY - 1, colourGrid);

    myScreen.flushFast();
}

void drawGameScreen()
{
    // Draw white frame
    myScreen.clear(colourBackground);

    for (uint8_t i = 0; i <= NUMBER; i++)
    {
        uint16_t z = i * sizeCell;

        myScreen.dLine(minX + z, minY + 0, 1, maxY - minY, colourGrid);
        myScreen.dLine(minX + 0, minY + z, maxX - minX, 1, colourGrid);
    }

    // Draw frame
    myScreen.setPenSolid(false);
    myScreen.rectangle(minX, minY, maxX - 1, maxY - 1, colourGrid);

    myScreen.flushFast();
}

void drawGameOverScreen()
{
    char text[24] = {0};
    uint16_t colour;

    switch (winner)
    {
        case PLAYER_NONE:

            strcpy(text, "DRAW");
            colour = colourGrid;
            break;

        case PLAYER_HUMAN:

            strcpy(text, "HUMAN WINS");
            colour = colourHuman;
            break;

        case PLAYER_MCU:

            strcpy(text, "MCU WINS");
            colour = colourMCU;
            break;

        default:
            break;
    }

    myScreen.clear(colourBackground);

    displayCenteredText(minX + sizeTable / 2, minY + sizeTable * 3 / 4, (STRING_CONST_TYPE)text, colour);
    displayCenteredText(minX + sizeTable / 2, minY + sizeTable * 1 / 4, "GAME OVER", colourMessage);
    displayCenteredText(minX + sizeTable / 2, minY + sizeTable * 2 / 4, "Touch to start", colourMessage);

    // Draw frame
    myScreen.setPenSolid(false);
    myScreen.rectangle(minX, minY, maxX - 1, maxY - 1, colourGrid);

    myScreen.flushFast();
}

void playGame()
{
    do
    {
        if (moves % 2 == 1)
        {
            displayCenteredText(maxX / 2, maxY / 12, "MCU plays", colourMessage);
            myScreen.flushFast();

            moveMCU();
        }
        else
        {
            displayCenteredText(maxX / 2, maxY / 12, "Human plays", colourMessage);
            myScreen.flushFast();

            moveHuman();
        }
        printBoard();
        winner = checkWinner();
        // drawWinnerPlayer(winner);
        moves++;
    }
    while ((winner == PLAYER_NONE) and (moves < NUMBER * NUMBER + 1));

    mySerial.print("--- ");
    hV_HAL_delayMilliseconds(100);
    switch (winner)
    {
        case PLAYER_HUMAN:
            hV_HAL_log(LEVEL_INFO, "HUMAN WINS");

            break;

        case PLAYER_MCU:
            hV_HAL_log(LEVEL_INFO, "MCU WINS");
            break;

        default:
            hV_HAL_log(LEVEL_INFO, "DRAW");
            break;
    }
    hV_HAL_delayMilliseconds(100);
    fsmScreen = 3;
    drawGameOverScreen();
}

void moveHuman()
{
    bool validMove = false;
    coordinates_s nextMove;
    touch_t _touch;

    hV_HAL_log(LEVEL_INFO, "--- PLAYER_HUMAN");

    myTouch.t = TOUCH_EVENT_NONE;
    _touch.t = TOUCH_EVENT_NONE;
    while (!validMove)
    {
        // Touch acquisition
        while (myTouch.t != TOUCH_EVENT_PRESS)
        {
            myScreen.getTouch(myTouch);
            hV_HAL_delayMilliseconds(10);
        }
        // Wrong coordinates with TOUCH_EVENT_RELEASE
        while (_touch.t != TOUCH_EVENT_RELEASE)
        {
            myScreen.getTouch(_touch);
            hV_HAL_delayMilliseconds(10);
        }

        nextMove.i = (myTouch.x - minX) / sizeCell;
        nextMove.j = (myTouch.y - minY) / sizeCell;

        if ((nextMove.i < NUMBER) and (nextMove.j < NUMBER))
        {
            validMove = setBoard(nextMove, PLAYER_HUMAN);
        }
        hV_HAL_delayMilliseconds(10);
    }
    drawMovePlayer(nextMove, PLAYER_HUMAN);
}

void printBoard()
{
    // mySerial.println("Board:");
    for (uint8_t i = 0; i < NUMBER; i++)
    {
        mySerial.print("[ ");
        for (uint8_t j = 0; j < NUMBER; j++)
        {
            mySerial.print(board[j][i]);
            mySerial.print(" ");
        }

        mySerial.println("]");
    }
    mySerial.println("");
}

bool checkHuman(coordinates_s & coordinates)
{
    uint8_t k = 0;

    // Check lines
    for (uint8_t i = 0; i < NUMBER; i++)
    {
        k = 0;
        for (uint8_t j = 0; j < NUMBER; j++)
        {
            k += (board[i][j] == PLAYER_HUMAN) ? 1 : 0;
        }
        if (k == NUMBER - 1)
        {
            for (uint8_t j = 0; j < NUMBER; j++)
            {
                if (board[i][j] == PLAYER_NONE)
                {
                    coordinates.i = i;
                    coordinates.j = j;
                    return true;
                }
            }
        }
    }

    // Check diagonal
    k = 0;
    for (uint8_t i = 0; i < NUMBER; i++)
    {
        k += (board[i][i] == PLAYER_HUMAN) ? 1 : 0;
    }
    if (k == NUMBER - 1)
    {
        for (uint8_t i = 0; i < NUMBER; i++)
        {
            if (board[i][i] == PLAYER_NONE)
            {
                coordinates.i = i;
                coordinates.j = i;
                return true;
            }
        }
    }

    return false;
}

void moveMCU()
{
    bool flagPlayed = false;
    uint8_t counter = 0;
    uint8_t movesPlayed = 0;
    coordinates_s nextMove;
    hV_HAL_log(LEVEL_INFO, "--- PLAYER_MCU");

    // Four corners
    uint8_t firstMovesI[4] = { 0, 0, NUMBER - 1, NUMBER - 1 };
    uint8_t firstMovesJ[4] = { 0, NUMBER - 1, 0, NUMBER - 1 };
    // will use these positions first

    for (counter = 0; counter < 4; counter++) //Count first moves played
    {
        if (board[firstMovesI[counter]][firstMovesJ[counter]] != 0) // First move is played by someone
        {
            movesPlayed++;
        }
    }

    do
    {
        if (moves <= 2) // First two moves
        {
            uint8_t randomMove = random(4);
            nextMove.i = firstMovesI[randomMove];
            nextMove.j = firstMovesI[randomMove];
        }
        else
        {
            bool result = checkHuman(nextMove);
            if (result == false)
            {
                if (movesPlayed == 4) // After two first moves
                {
                    nextMove.i = random(NUMBER);
                    nextMove.j = random(NUMBER);
                }
                else
                {
                    uint8_t randomMove = random(4);
                    nextMove.i = firstMovesI[randomMove];
                    nextMove.j = firstMovesJ[randomMove];
                }
            }
            else
            {
                nextMove.i = random(NUMBER);
                nextMove.j = random(NUMBER);
            }
        }
        flagPlayed = setBoard(nextMove, PLAYER_MCU);
    }
    while (flagPlayed == false);

    drawMovePlayer(nextMove, PLAYER_MCU);
}

void drawMovePlayer(coordinates_s move, uint8_t player)
{
    uint16_t x0 = minX + move.i * sizeCell;
    uint16_t y0 = minY + move.j * sizeCell;
    uint16_t dz = sizeCell / 2 - 8;

    myScreen.setPenSolid(true);
    switch (player)
    {
        case PLAYER_MCU:

            myScreen.circle(x0 + sizeCell / 2, y0 + sizeCell / 2, dz, colourMCU);
            break;

        case PLAYER_HUMAN:

            myScreen.dRectangle(x0 + 8, y0 + 8, sizeCell - 16, sizeCell - 16, colourHuman);
            break;

        default:

            break;
    }
    myScreen.setPenSolid(false);

    myScreen.flushFast();
}

bool checkWinnerPlayer(uint8_t player)
{
    bool flag = false;
    uint8_t k1 = 0;
    uint8_t k2 = 0;

    // Check lines and columns
    if (!flag)
    {
        for (uint8_t i = 0; i < NUMBER; i++)
        {
            k1 = 0;
            k2 = 0;
            for (uint8_t j = 0; j < NUMBER; j++)
            {
                k1 += (board[i][j] == player) ? 1 : 0;
                k2 += (board[j][i] == player) ? 1 : 0;
            }
            flag |= (k1 == NUMBER) | (k2 == NUMBER);
        }
    }

    // Check diagonal and inverse diagonal
    if (!flag)
    {
        k1 = 0;
        k2 = 0;
        for (uint8_t i = 0; i < NUMBER; i++)
        {
            k1 += (board[i][i] == player) ? 1 : 0;
            k2 += (board[NUMBER - 1 - i][i] == player) ? 1 : 0;
        }
        flag |= (k1 == NUMBER) | (k2 == NUMBER);
    }
    return flag;
}

void drawWinnerPlayer(uint8_t player)
{
    bool flag = false;
    uint8_t k1 = 0;
    uint8_t k2 = 0;
    uint16_t x0, y0;
    uint16_t dz = sizeCell / 2 - 8;

    myScreen.setPenSolid();
    // Check lines and columns
    if (!flag)
    {
        for (uint8_t i = 0; i < NUMBER; i++)
        {
            k1 = 0;
            k2 = 0;
            for (uint8_t j = 0; j < NUMBER; j++)
            {
                k1 += (board[i][j] == player) ? 1 : 0;
                k2 += (board[j][i] == player) ? 1 : 0;
            }

            if (k1 == NUMBER)
            {
                flag = true;
                x0 = i * sizeCell;

                switch (player)
                {
                    case PLAYER_MCU:

                        for (uint8_t k = 1; k < NUMBER; k++)
                        {
                            y0 = k * sizeCell - sizeCell / 3;
                            myScreen.circle(minX + x0 + sizeCell / 2, minY + y0 + sizeCell / 2, dz, colourMCU);
                            y0 = k * sizeCell - sizeCell * 2 / 3;
                            myScreen.circle(minX + x0 + sizeCell / 2, minY + y0 + sizeCell / 2, dz, colourMCU);
                        }
                        break;

                    case PLAYER_HUMAN:

                        for (uint8_t k = 1; k < NUMBER; k++)
                        {
                            y0 = k * sizeCell - sizeCell / 2;

                            myScreen.dRectangle(minX + x0 + 8, minY + y0 + 8, sizeCell - 16, sizeCell - 16, colourHuman);
                        }
                        break;

                    default:
                        break;
                }
            }
            if (k2 == NUMBER)
            {
                flag = true;
                y0 = i * sizeCell;

                switch (player)
                {
                    case PLAYER_MCU:

                        for (uint8_t k = 1; k < NUMBER; k++)
                        {
                            x0 = k * sizeCell - sizeCell / 3;
                            myScreen.circle(minX + x0 + sizeCell / 2, minY + y0 + sizeCell / 2, dz, colourMCU);
                            x0 = k * sizeCell - sizeCell * 2 / 3;
                            myScreen.circle(minX + x0 + sizeCell / 2, minY + y0 + sizeCell / 2, dz, colourMCU);
                        }

                        break;

                    case PLAYER_HUMAN:

                        for (uint8_t k = 1; k < NUMBER; k++)
                        {
                            x0 = k * sizeCell - sizeCell / 2;

                            myScreen.dRectangle(minX + x0 + 8, minY + y0 + 8, sizeCell - 16, sizeCell - 16, colourHuman);
                        }
                        break;

                    default:
                        break;
                }

            }
        }
    }

    // Check diagonal and inverse diagonal
    if (!flag)
    {
        k1 = 0;
        k2 = 0;
        for (uint8_t i = 0; i < NUMBER; i++)
        {
            k1 += (board[i][i] == player) ? 1 : 0;
            k2 += (board[NUMBER - 1 - i][i] == player) ? 1 : 0;
        }
        if (k1 == NUMBER)
        {
            flag = true;

            switch (player)
            {
                case PLAYER_MCU:

                    for (uint8_t k = 1; k < NUMBER; k++)
                    {
                        x0 = k * sizeCell - sizeCell / 3;
                        y0 = k * sizeCell - sizeCell / 3;
                        myScreen.circle(minX + x0 + sizeCell / 2, minY + y0 + sizeCell / 2, dz, colourMCU);
                        x0 = k * sizeCell - sizeCell * 2 / 3;
                        y0 = k * sizeCell - sizeCell * 2 / 3;
                        myScreen.circle(minX + x0 + sizeCell / 2, minY + y0 + sizeCell / 2, dz, colourMCU);
                    }
                    break;

                case PLAYER_HUMAN:

                    for (uint8_t k = 1; k < 2 * NUMBER; k++)
                    {
                        x0 = k * sizeCell - sizeCell / 2;
                        y0 = k * sizeCell - sizeCell / 2;
                        myScreen.dRectangle(minX + x0 + 8, minY + y0 + 8, sizeCell - 16, sizeCell - 16, colourHuman);
                        x0 = k * sizeCell - sizeCell * 2 / 3;
                        y0 = k * sizeCell - sizeCell * 2 / 3;
                        myScreen.dRectangle(minX + x0 + 8, minY + y0 + 8, sizeCell - 16, sizeCell - 16, colourHuman);
                    }
                    break;

                default:
                    break;
            }
        }
        if (k2 == NUMBER)
        {
            flag = true;

            switch (player)
            {
                case PLAYER_MCU:

                    for (uint8_t k = 1; k < NUMBER; k++)
                    {
                        x0 = k * sizeCell - sizeCell / 3;
                        y0 = (NUMBER - k) * sizeCell - sizeCell / 3;
                        myScreen.circle(minX + x0 + sizeCell / 2, minY + y0 + sizeCell / 2, dz, colourMCU);
                        x0 = k * sizeCell - sizeCell * 2 / 3;
                        y0 = (NUMBER - k) * sizeCell - sizeCell * 2 / 3;
                        myScreen.circle(minX + x0 + sizeCell / 2, minY + y0 + sizeCell / 2, dz, colourMCU);
                    }
                    break;

                case PLAYER_HUMAN:

                    for (uint8_t k = 1; k < NUMBER; k++)
                    {
                        x0 = k * sizeCell - sizeCell / 3;
                        y0 = (NUMBER - k) * sizeCell - sizeCell / 3;
                        myScreen.dRectangle(minX + x0 + 8, minY + y0 + 8, sizeCell - 16, sizeCell - 16, colourHuman);
                        x0 = k * sizeCell - sizeCell * 2 / 3;
                        y0 = (NUMBER - k) * sizeCell - sizeCell * 2 / 3;
                        myScreen.dRectangle(minX + x0 + 8, minY + y0 + 8, sizeCell - 16, sizeCell - 16, colourHuman);
                    }
                    break;

                default:
                    break;
            }

        }
    }
    myScreen.setPenSolid(false);

    myScreen.flushFast();
}

uint8_t checkWinner()
// checks board to see if there is a winner
// places result in the global variable 'winner'
{
    if (checkWinnerPlayer(PLAYER_HUMAN))
    {
        return PLAYER_HUMAN;
    }
    if (checkWinnerPlayer(PLAYER_MCU))
    {
        return PLAYER_MCU;
    }
    return PLAYER_NONE;
}

///
/// @brief Setup
///
void setup()
{
    // hV_HAL_Serial = Serial by default, otherwise edit hV_HAL_Peripherals.h
    hV_HAL_begin(); // with Serial at 115200

    hV_HAL_Serial_crlf();
    hV_HAL_log(LEVEL_INFO, __FILE__);
    hV_HAL_log(LEVEL_INFO, __DATE__ " " __TIME__);
    hV_HAL_Serial_crlf();

    // Screen
    myScreen.begin();

    myScreen.regenerate();

    // Fonts
#if (FONT_MODE == USE_FONT_TERMINAL)

    fontSmall = Font_Terminal6x8;
    fontMedium = Font_Terminal8x12;
    fontLarge = Font_Terminal12x16;
    fontVery = Font_Terminal16x24;

#else // FONT_MODE

    fontSmall = myScreen.addFont(Font_DejaVuSans12);
    fontSmall -= ((fontSmall > 0) ? 1 : 0);
    fontMedium = myScreen.addFont(Font_DejaVuSans16);
    fontMedium -= ((fontMedium > 0) ? 1 : 0);
    fontLarge = myScreen.addFont(Font_DejaVuSans24);
    fontLarge -= ((fontLarge > 0) ? 1 : 0);
    fontVery = myScreen.addFont(Font_DejaVuMono48);
    fontVery -= ((fontVery > 0) ? 1 : 0);

#endif // FONT_MODE

    // Example
    myScreen.regenerate();
    myScreen.setOrientation(ORIENTATION_PORTRAIT);
    myScreen.selectFont(fontMedium);

    minX = 0;
    maxX = myScreen.screenSizeX();
    minY = 0;
    maxY = myScreen.screenSizeY();

    sizeTable = hV_HAL_min(maxX - minX, maxY - minY);
    sizeCell = sizeTable / NUMBER;

    uint16_t w;
    w = (maxX - minX - NUMBER * sizeCell) / 2;
    minX += w;
    maxX -= w;
    w = (maxY - minY - NUMBER * sizeCell) / 2;
    minY += w;
    maxY -= w;

    colourHuman = myColours.black;
    colourMCU = myColours.grey;
    colourGrid = myColours.grey;
    colourBackground = myColours.white;
    colourMessage = myColours.black;

    printBoard();
    randomSeed(analogRead(0));
    drawStartScreen();
}

///
/// @brief Loop, empty
///
void loop()
{
    myScreen.getTouch(myTouch);

    if (myTouch.t == TOUCH_EVENT_PRESS)
    {
        hV_HAL_log(LEVEL_INFO, "=== START");
        myScreen.regenerate();
        resetGame();
        drawGameScreen();
        playGame();
        hV_HAL_delayMilliseconds(10);
    }
    hV_HAL_delayMilliseconds(100);
}
