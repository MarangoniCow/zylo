/***********************************************************
 *                      SDL_BOARD.H
 * 
 *  Generates the board, deals with error checking, etc., etc.
 * 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
// ...

// EXTERNAL INCLUDES
#include <SDL2/SDL.h>
#include <iostream>
#include <string>

// ENUMERATIONS
enum WINDOW_SIZES
{
    WINDOW_HEIGHT = 640,
    WINDOW_WIDTH = 640
};

enum BOARD_VALUES
{   
    SQUARE_WHITE_R = 252,
    SQUARE_WHITE_G = 245,
    SQUARE_WHITE_B = 232,
    SQUARE_BLACK_R = 117,
    SQUARE_BLACK_G = 79,
    SQUARE_BLACK_B = 18,
    SQUARE_WIDTH = 75,
    SQUARE_HEIGHT = 75,
    BOARD_X = 20,
    BOARD_Y = 20,
    BOARD_BORDER_R = 28,
    BOARD_BORDER_G = 27,
    BOARD_BORDER_B = 34
};

class SDL_Board
{
    protected:
        SDL_Window* window;                     // Window handle
        SDL_Renderer* windowRenderer;           // Window renderer 
        int SDL_coordinates[8][8][2];           // Chess board coordinates list: 8 by 8 coordiates (bottom left to top right) and x,y window coordinates of square
        
    public: 
        SDL_Board();                            // Constructor
        ~SDL_Board();                           // Destructor
        
        void GenerateBoard();                   // Handle initial board generation 
        SDL_Texture* LoadTexture(); 
        

};