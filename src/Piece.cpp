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
std::vector<int> Piece::Inst_list;


int Piece::Get_Count()
{
    return Piece_Count;
}
int Piece::Get_ID()
{
    return Inst_ID;
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

void Piece::OutputPieceList()
{
    for(int i = 0; i < Inst_list.size(); i++)
        std::cout << Inst_list[i] << std::endl;
}


/***********************************************************
 *                   PIECE DESCRIPTIONS
 ***********************************************************/
PieceDescriptor Pawn::pawn_description      {"./res/pawn_", "p"}; 
PieceDescriptor Rook::rook_description      {"./res/rook_", "r"}; 
PieceDescriptor Knight::knight_description  {"./res/knight_", "k"}; 
PieceDescriptor Bishop::bishop_description  {"./res/bishop_", "b"}; 
PieceDescriptor Queen::queen_description    {"./res/queen_", "q"}; 
PieceDescriptor King::king_description      {"./res/king_", "ki"}; 
