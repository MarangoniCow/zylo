/***********************************************************
 *                      COORDS IMPLEMENTATION
  ***********************************************************/

// INTERNAL INCLUDES
#include "Coords.h"

Coords::Coords()
{
    pos.x = -1;
    pos.y = -1;
}

Coords::Coords(int x, int y)
{
    // Set x/y
    pos.x = x;
    pos.y = y; 
}

void Coords::updatePosition(int TAR_x, int TAR_y)
{
    pos.x = TAR_x;
    pos.y = TAR_y;
}

void Coords::updatePosition(BoardPosition newPos)
{
    pos.x = newPos.x;
    pos.y = newPos.y;
}



bool Coords::validUpdate(int i, int j)
{
    BoardPosition newPos = pos;
    newPos.x += i;
    newPos.y += j;
    return newPos.validPosition();
}

BoardPosition Coords::returnUpdate(int i, int j)
{   
    BoardPosition newPos;
    newPos.x = pos.x + i;
    newPos.y = pos.y + j;
    return newPos;
}




