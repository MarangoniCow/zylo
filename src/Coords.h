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
        Coords(); 
        Coords(int x, int y);
  
        
        BoardPosition Get_Position() {return cur_pos;};
        void updatePosition(int TAR_x, int TAR_y);
        void updatePosition(BoardPosition newPos);
        static bool ValidPosition(BoardPosition pos); 
};