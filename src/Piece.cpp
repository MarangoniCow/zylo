/***********************************************************
 *                      PIECE.CPP
 * 
 *  Code implementation
 * 
 * 
 ***********************************************************/

// INTERNAL INCLUDES
#include "Piece.h"

// EXTERNAL INCLUDES
#include <string>
#include <iostream>

// Initialise piece count
int Piece::Piece_Count = 0;



int Piece::Get_Count()
{
    return Piece_Count;
}
