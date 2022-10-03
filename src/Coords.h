/***********************************************************
 *                      COORDS.H
 * 
 * Global coordinate class, contains a BoardPosition struct
 * for global use, as well as a class with methods.
 * 
 * Default constructor for BoardPosition sets (x, y) as (-1, -1)
 * as an 'invalid' position. 
 * 
 * Board coordinates are defined using a zero-index notation
 * with the bottom-left coordinate defined as 0,0 and the top-right
 * coordinate defined as 7-7, where white is always defined to be
 * along the bottom. 
 ***********************************************************/

#pragma once

struct BoardPosition
{
    int x;
    int y;

    BoardPosition() : x(-1), y(-1) {};
    void ResetPosition() {
        x = -1;
        y = -1;
    }
    bool validPosition()
    {
    if(x < 8 && x >= 0 && y < 8 && y >= 0)
        return 1;
    else
        return 0;
    }
    
    bool operator == (const BoardPosition& rhs)
    {   
    if (x == rhs.x && y == rhs.y)
        return 1;
    else
        return 0;
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
        bool validUpdate(int i, int j);
        BoardPosition returnUpdate(int i, int j);
};