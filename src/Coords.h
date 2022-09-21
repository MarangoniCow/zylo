/***********************************************************
 *                      COORDS.H
 * 
 *  Global coordinate class
 * 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES

// EXTERNAL INCLUDES

class Coords {
    protected:
        static const int Coords_Arr[8][8]; 

    public:
        static int CoordsToChess(int x, int y);
        static int CoordsToSDL(int x, int y); 
        
}