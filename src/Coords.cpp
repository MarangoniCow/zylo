


// INTERNAL INCLUDES
#include "Coords.h"
Coords::Coords()
{
    cur_pos.x = -1;
    cur_pos.y = -1;
}

Coords::Coords(int x, int y)
{
    // Set x/y
    cur_pos.x = x;
    cur_pos.y = y; 
}

void Coords::updatePosition(int TAR_x, int TAR_y)
{
    cur_pos.x = TAR_x;
    cur_pos.y = TAR_y;
}

void Coords::updatePosition(BoardPosition newPos)
{
    cur_pos.x = newPos.x;
    cur_pos.y = newPos.y;
}



