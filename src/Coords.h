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


class Coords {
    protected:
        int x = -1;
        int y = -1;
        
    public:
        Coords();
        static void SDLCoordsToBoard(int* SDL_X, int* SDL_y, int* BRD_x, int* BRD_y);
};