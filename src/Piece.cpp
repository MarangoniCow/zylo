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
PIECE_COLOUR Piece::Get_Colour()
{
    return col;
}
std::string Piece::Get_FilePath()
{   
    std::string pieceColour;

    if(col)
       pieceColour = "black";
    else
        pieceColour = "white";

    return descriptor.filePath + pieceColour + ".bmp";
}



/***********************************************************
 *                   PIECE DESCRIPTIONS
 ***********************************************************/
PieceDescriptor Pawn::pawn_description      {"./res/pawn_"}; 
PieceDescriptor Rook::rook_description      {"./res/rook_"}; 
PieceDescriptor Knight::knight_description  {"./res/knight_"}; 
PieceDescriptor Bishop::bishop_description  {"./res/bishop_"}; 
PieceDescriptor Queen::queen_description    {"./res/queen_"}; 
PieceDescriptor King::king_description      {"./res/king_"}; 
