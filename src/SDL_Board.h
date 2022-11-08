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
#include "BoardPosition.h"
#include "Board.h"
#include "SDL_Pieces.h"

// EXTERNAL INCLUDES
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <queue>


// ENUMERATIONS
enum WINDOW_SIZES
{
    WINDOW_HEIGHT = 640,
    WINDOW_WIDTH = 640
};

enum BOARD_VALUES
{   
    SQUARE_WIDTH = 75,
    SQUARE_HEIGHT = 75,
    BOARD_X = 20,
    BOARD_Y = 20,
    BOARD_BORDER_R = 28,
    BOARD_BORDER_G = 27,
    BOARD_BORDER_B = 34
};

enum BOARD_BACKGROUND
{
    SQUARE_WHITE_R = 252,
    SQUARE_WHITE_G = 245,
    SQUARE_WHITE_B = 232,
    SQUARE_BLACK_R = 117,
    SQUARE_BLACK_G = 79,
    SQUARE_BLACK_B = 18
};

enum BOARD_OVERLAY
{
    OVERLAY_GREEN_R = 130,
    OVERLAY_GREEN_G = 180,  
    OVERLAY_GREEN_B = 126,
    OVERLAY_RED_R = 234,
    OVERLAY_RED_G = 10,
    OVERLAY_RED_B = 43,
    OVERLAY_WHITE_R = 40,
    OVERLAY_WHITE_G = 40,
    OVERLAY_WHITE_B = 40,
    OVERLAY_YELLOW_R = 246,
    OVERLAY_YELLOW_G = 218,
    OVERLAY_YELLOW_B = 60
};

enum OVERLAY_COL
{
    OVERLAY_GREEN, OVERLAY_RED, OVERLAY_WHITE, OVERLAY_YELLOW
};


class SDL_Board
{
    protected:
        // MEMBER VARIABLES
        SDL_Pieces pieceGraphics;
        SDL_Window* window;                     // Window handle
        SDL_Renderer* windowRenderer;           // Window renderer
        int SDL_coordinates[8][8][2];           // Chess board coordinates list: 8 by 8 coordiates (top left to bottom right) and x,y window coordinates of square


        // INTERNAL LOADING METHODS
        void loadBackground();                   
        void loadPiece(int x, int y, PIECE_TYPE type, PIECE_COLOUR col);
        void loadState(BoardState state);
        void loadSquare(int x, int y);
        void loadOverlay(int x, int y, OVERLAY_COL col);
        void clearBoard();
        SDL_Rect returnSDLSquare(int x, int y);
        
                
    public:
        // CONSTRUCTORS
        SDL_Board();                            
        ~SDL_Board();                           
        
        // PUBLIC RENDER METHODS
        void renderBackground();
        void renderBoard(BoardState state);
        void renderBoardUpdate(BoardState state);
        void renderOverlay(PositionQueue validQueue, PositionQueue takeQueue, PositionQueue invalidQueue);
        void renderLast(Move lastMove);
        void renderPrevious(BoardPosition prevPos, BoardPosition newPos);

        // STATIC METHODS
        static BoardPosition SDL_to_Coords(int SDL_x, int SDL_y);
};