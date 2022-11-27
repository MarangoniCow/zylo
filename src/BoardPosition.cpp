/***********************************************************
 *             BOARDPOSITION IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "BoardPosition.h"

// EXTERNAL INCLUDES
#include <queue>
#include <utility>
#include <cmath>




void BoardPosition::resetPosition() {
        x = -1;
        y = -1;
}

bool BoardPosition::validPosition() const
{
    if(x < 8 && x >= 0 && y < 8 && y >= 0)
        return 1;
    else
        return 0;
}
bool BoardPosition::validUpdate(int i, int j) const
{
    if(x + i < 8 && x + i >= 0 && y + j < 8 && y + j >= 0)
        return 1;
    else
        return 0;
}
bool BoardPosition::validUpdate(BoardPosition newPos) const
{
    int i = newPos.x;
    int j = newPos.y;
    if(x + i < 8 && x + i >= 0 && y + j < 8 && y + j >= 0)
        return 1;
    else
        return 0;
}
void BoardPosition::position(BoardPosition newPos)
{
    x = newPos.x;
    y = newPos.y;
}
void BoardPosition::position(int i, int j)
{
    x = i;
    y = j;
}
BoardPosition BoardPosition::returnUpdate(int i, int j) const
{
    BoardPosition newPos(x + i, y + j);
    return newPos;
}
bool BoardPosition::operator == (const BoardPosition& rhs)
{   
    if (x == rhs.x && y == rhs.y)
        return 1;
    else
        return 0;
}
bool BoardPosition::operator != (const BoardPosition& rhs)
{   
if (x == rhs.x && y == rhs.y)
    return 0;
else
    return 1;
}
RELPOS BoardPosition::returnRelPos(BoardPosition curPos, BoardPosition tarPos)
{
    int xa = curPos.x - tarPos.x;
    int ya = curPos.y - tarPos.y;
    
    if(curPos.y == tarPos.y && tarPos.x < curPos.x)
        return LEFT;
    else if(curPos.y == tarPos.y && tarPos.x > curPos.x)
        return RIGHT;
    else if(curPos.y > tarPos.y && tarPos.x == curPos.x)
        return DOWN;
    else if(curPos.y < tarPos.y && tarPos.x == curPos.x)
        return UP;
    else if(curPos.y > tarPos.y && tarPos.x < curPos.x && xa == ya)
        return LEFTDOWN;
    else if(curPos.y < tarPos.y && tarPos.x < curPos.x && xa == -ya)
        return LEFTUP;
    else if(curPos.y > tarPos.y && tarPos.x > curPos.x && xa == -ya)
        return RIGHTDOWN;
    else if(curPos.y < tarPos.y && tarPos.x > curPos.x && xa == ya)
        return RIGHTUP;
    else if (curPos.y == tarPos.y && tarPos.x == curPos.x)
        return SAME;
    else
        return OUTOFLINE;
};
BoardPosition BoardPosition::returnIncrement(RELPOS relpos) const
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
std::queue<BoardPosition> BoardPosition::returnPositionQueue()
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

std::vector<BoardPosition> BoardPosition::returnRelativePositions(BoardPosition curPos, BoardPosition tarPos)
{
    std::vector<BoardPosition> returnVector;
    RELPOS relpos = returnRelPos(curPos, tarPos);
    if(relpos != SAME && relpos != OUTOFLINE)
    {
        BoardPosition temp = curPos;
        while(temp.validPosition())
        {
            returnVector.push_back(temp);
            temp = temp.returnIncrement(relpos);
        }
    }
    return returnVector;
}