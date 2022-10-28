/***********************************************************
 *                      BOARDPOSITION.H
 * 
 * Global coordinate class, contains a BoardPosition struct
 * for global use.
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

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <queue>
#include <utility>
#include <cmath>


enum RELPOS
{
    OUTOFLINE, SAME, LEFT, LEFTUP, UP, RIGHTUP, RIGHT, RIGHTDOWN, DOWN, LEFTDOWN
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
    static RELPOS returnRelPos(BoardPosition curPos, BoardPosition tarPos)
    {
        int xa = curPos.x - tarPos.x;
        int ya = curPos.y - tarPos.y;
        int diff = abs(xa - ya);

        if(curPos.y == tarPos.y && tarPos.x < curPos.x)
            return LEFT;
        else if(curPos.y == tarPos.y && tarPos.x > curPos.x)
            return RIGHT;
        else if(curPos.y > tarPos.y && tarPos.x == curPos.x)
            return DOWN;
        else if(curPos.y < tarPos.y && tarPos.x == curPos.x)
            return UP;
        else if(curPos.y > tarPos.y && tarPos.x < curPos.x && diff == 0)
            return LEFTDOWN;
        else if(curPos.y < tarPos.y && tarPos.x < curPos.x && diff == 0)
            return LEFTUP;
        else if(curPos.y > tarPos.y && tarPos.x > curPos.x && diff == 0)
            return RIGHTDOWN;
        else if(curPos.y < tarPos.y && tarPos.x > curPos.x && diff == 0)
            return RIGHTUP;
        else if (curPos.y == tarPos.y && tarPos.x == curPos.x)
            return SAME;
        else
            return OUTOFLINE;
    };
    BoardPosition increment(RELPOS relpos)
    {
        if(relpos == LEFT && validUpdate(-1, 0))
            return returnUpdate(-1, 0);
        else if(relpos == RIGHT && validUpdate(1, 0))
            return returnUpdate(1, 0);
        else if(relpos == DOWN && validUpdate(0, -1))
            return returnUpdate(0, -1);
        else if(relpos == UP && validUpdate(0, 1))
            return returnUpdate(0, 1);
        else if(relpos == LEFTDOWN && validUpdate(-1, -1))
            return returnUpdate(-1, -1);
        else if(relpos == LEFTUP && validUpdate(-1, 1))
            return returnUpdate(-1, 1);
        else if(relpos == RIGHTDOWN && validUpdate(1, -1))
            return returnUpdate(1, -1);
        else if(relpos == RIGHTUP && validUpdate(1, 1))
            return returnUpdate(1, 1);
        else
        {
            BoardPosition pos;
            return pos;
        }
    }
    static std::queue<BoardPosition> returnPositionQueue()
    {
        std::queue<BoardPosition> allPositions;
        BoardPosition temp;
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                temp.x = i; temp.y = j;
                allPositions.push(temp);
            }
        }
        return allPositions;
    }

    static std::queue<BoardPosition> returnRelativePositions(BoardPosition curPos, BoardPosition tarPos)
    {
        std::queue<BoardPosition> returnQueue;
        RELPOS relpos = returnRelPos(curPos, tarPos);
        if(relpos != SAME || relpos != OUTOFLINE)
        {
            BoardPosition temp = curPos;
            while(temp.validPosition())
            {
                returnQueue.push(temp);
                temp.increment(relpos);
            }
        }
        return returnQueue;
    }
};