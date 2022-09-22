/***********************************************************
 *                      COORDS.H
 * 
 *  Global coordinate class
 * 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "SDL_Board.h"

// EXTERNAL INCLUDES


struct BoardPosition
{
    int x;
    int y;
};

class Coords {
    protected:
        BoardPosition cur_pos;
        
    public:
        Coords(int x, int y);
        void UpdatePosition(int x, int y); 
        bool ValidPosition(BoardPosition pos); 
        BoardPosition Get_Position() {return cur_pos;};
        static void SDLCoordsToBoard(int* SDL_X, int* SDL_y, int* BRD_x, int* BRD_y);
};