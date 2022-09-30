


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





