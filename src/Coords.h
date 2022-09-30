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


struct BoardPosition
{
    int x;
    int y;

    BoardPosition() : x(-1), y(-1) {};
    void ResetPosition() {
        x = -1;
        y = -1;
    }
};

class Coords {
    protected:
        BoardPosition pos;
        
    public:
        Coords(); 
        Coords(int x, int y);
  
        
        BoardPosition Get_Position() {return pos;};
        void updatePosition(int TAR_x, int TAR_y);
        void updatePosition(BoardPosition newPos);
        static bool ValidPosition(BoardPosition pos); 
};