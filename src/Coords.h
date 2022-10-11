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

enum RELPOS
{
    SAME, LEFT, RIGHT, DOWN, UP, LEFTUP, LEFTDOWN, RIGHTUP, RIGHTDOWN
};

struct BoardPosition
{
    int x;
    int y;

    BoardPosition() : x(-1), y(-1) {};
    BoardPosition(int i, int j) : x(i), y(j) {};
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
    bool validUpdate(int i, int j)
    {
        if(x + i < 8 && x + i >= 0 && y + j < 8 && y + j >= 0)
            return 1;
        else
            return 0;
    }
    bool validUpdate(BoardPosition newPos)
    {
        int i = newPos.x;
        int j = newPos.y;
        if(x + i < 8 && x + i >= 0 && y + j < 8 && y + j >= 0)
            return 1;
        else
            return 0;
    }
    void updatePosition(BoardPosition newPos)
    {
        x = newPos.x;
        y = newPos.y;
    }
    void updatePosition(int i, int j)
    {
        x = i;
        y = j;
    }
    BoardPosition returnUpdate(int i, int j)
    {
        BoardPosition newPos(x + i, y + j);
        return newPos;
    }
    RELPOS returnRelPos(BoardPosition tarPos)
    {
        // LEFT
        if(y == tarPos.y && tarPos.x < x)
            return LEFT;
        else if(y == tarPos.y && tarPos.x > x)
            return RIGHT;
        else if(y > tarPos.y && tarPos.x == x)
            return DOWN;
        else if(y < tarPos.y && tarPos.x == x)
            return UP;
        else if(y > tarPos.y && tarPos.x < x)
            return LEFTDOWN;
        else if(y > tarPos.y && tarPos.x > x)
            return RIGHTDOWN;
        else if(y < tarPos.y && tarPos.x < x)
            return LEFTUP;
        else if(y < tarPos.y && tarPos.x > x)
            return RIGHTUP;
        else
            return SAME;   
    }
    bool operator == (const BoardPosition& rhs)
    {   
        if (x == rhs.x && y == rhs.y)
            return 1;
        else
            return 0;
    }
    bool operator != (const BoardPosition& rhs)
    {   
    if (x == rhs.x && y == rhs.y)
        return 0;
    else
        return 1;
    }
};