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
#include <vector>

// Initialise static variables
int Piece::Piece_Count = 0;
std::vector<int> Piece::ID_list;



int Piece::Get_Count()
{
    return Piece_Count;
}
int Piece::Get_ID()
{
    return Piece_ID;
}
std::string Piece::Get_Colour()
{
    return descriptor.pieceColour;
}
std::string Piece::Get_FilePath()
{
    return descriptor.pieceFilePath;
}
