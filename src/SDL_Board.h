/***********************************************************
 *                      SDL_BOARD.H
 * 
 *  Deals with all rendering tasks:
 *      - Background
 *      - Updating and moving pieces
 *      - Transparent overlay 
 * 
 ***********************************************************/

#pragma once


// INTERNAL INCLUDES
#include "Coords.h"
#include "Board.h"

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
    SQUARE_GREEN_R = 130,
    SQUARE_GREEN_G = 180,
    SQUARE_GREEN_B = 126,
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
        // MEMBER VARIABLES
        SDL_Window* window;                     // Window handle
        SDL_Renderer* windowRenderer;           // Window renderer
        int SDL_coordinates[8][8][2];           // Chess board coordinates list: 8 by 8 coordiates (top left to bottom right) and x,y window coordinates of square

        // INTERNAL LOADING METHODS
        void loadBackground();                   
        void loadPiece(int x, int y, std::string filePath);
        void loadBoard(BoardState state);
        void loadSquare(int x, int y);
        void clearBoard();
        SDL_Rect returnSquare(int x, int y);
        
                
    public: 
        SDL_Board();                            // Constructor
        ~SDL_Board();                           // Destructor
        
        // PUBLIC RENDER METHODS
        void renderBackground();
        void renderNewBoard(BoardState state);
        void renderBoardUpdate(BoardState state);
        void renderOverlay(int x, int y);
    
        
        // STATIC METHODS
        static BoardPosition SDL_to_Coords(int SDL_x, int SDL_y);
};