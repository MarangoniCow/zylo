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
std::vector<int> Piece::Piece_Vector;


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

void Piece::OutputPieceList()
{
    for(int i = 0; i < Piece_Vector.size(); i++)
        std::cout << Piece_Vector[i] << std::endl;
}

void Piece::updatePosition(BoardPosition newPos)
{
    // Check flag
    if(!flag_move)
        flag_move = 1;
    
    pos.updatePosition(newPos);
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

/***********************************************************
 *                      MOVE RANGES
 ***********************************************************/
std::queue<BoardPosition> Pawn::moveRange()
{
    std::queue<BoardPosition> moveQueue;

    // WHITE = 0, BLACK = 1
    if(!col) {
        if(pos.validUpdate(0, 1)) moveQueue.push(pos.returnUpdate(0, 1));
        if(!flag_move && pos.validUpdate(0, 2)) moveQueue.push(pos.returnUpdate(0, 2));
    }
    else {
        if(pos.validUpdate(0, -1)) moveQueue.push(pos.returnUpdate(0, -1));
        if(!flag_move && pos.validUpdate(0, -2)) moveQueue.push(pos.returnUpdate(0, -2));
    }  
    
    return moveQueue;
    
}

std::queue<BoardPosition> Rook::moveRange()
{
    std::queue<BoardPosition> moveQueue;

    // Moving right
    int i = 1;
    while(pos.validUpdate(i, 0)) {
        moveQueue.push(pos.returnUpdate(i, 0));
        i++;
   }

    // Moving left
    i = -1;
    while(pos.validUpdate(i, 0)) {
        moveQueue.push(pos.returnUpdate(i, 0));
        i--;
    }

    // Moving forward
    int j = 1;
    while(pos.validUpdate(0, j)) {
        moveQueue.push(pos.returnUpdate(0, j));
        j++;
    }

    // Moving backward
    j = -1;
    while(pos.validUpdate(0, j)) {
        moveQueue.push(pos.returnUpdate(0, j));
        j--;
    }



    return moveQueue;
    
}
std::queue<BoardPosition> Knight::moveRange()
{
    std::queue<BoardPosition> moveQueue;

    if(pos.validUpdate(1, 2)) moveQueue.push(pos.returnUpdate(1, 2));
    if(pos.validUpdate(2, 1)) moveQueue.push(pos.returnUpdate(2, 1));

    if(pos.validUpdate(-1, 2)) moveQueue.push(pos.returnUpdate(-1, 2));
    if(pos.validUpdate(-2, 1)) moveQueue.push(pos.returnUpdate(-2, 1));

    if(pos.validUpdate(1, -2)) moveQueue.push(pos.returnUpdate(1, -2));
    if(pos.validUpdate(2, -1)) moveQueue.push(pos.returnUpdate(2, -1));

    if(pos.validUpdate(-1, -2)) moveQueue.push(pos.returnUpdate(-1, -2));
    if(pos.validUpdate(-2, -1)) moveQueue.push(pos.returnUpdate(-2, -1));

    return moveQueue;
    
}
std::queue<BoardPosition> Bishop::moveRange()
{
    std::queue<BoardPosition> moveQueue;

    // Moving diagonal right
    int i = 1;
    int j = 1;
    while(pos.validUpdate(i, j)) {
        moveQueue.push(pos.returnUpdate(i, j));
        i++;
        j++;
    }

    // Moving diagonal left
    i = -1;
    j = 1;
    while(pos.validUpdate(i, j)) {
        moveQueue.push(pos.returnUpdate(i, j));
        i--;
        j++;
    }

    // Moving diagonal down-right
    i = 1;
    j = -1;
    while(pos.validUpdate(i, j)) {
        moveQueue.push(pos.returnUpdate(i, j));
        i++;
        j--;
    }

    // Moving diagonal down-left
    i = -1;
    j = -1;
    while(pos.validUpdate(i, j)) {
        moveQueue.push(pos.returnUpdate(i, j));
        i--;
        j--;
    }

    return moveQueue;
    
}
std::queue<BoardPosition> Queen::moveRange()
{
    std::queue<BoardPosition> moveQueue;

    // Moving right
    int i = 1;
    while(pos.validUpdate(i, 0)) {
        moveQueue.push(pos.returnUpdate(i, 0));
        i++;
    }

    // Moving left
    i = -1;
    while(pos.validUpdate(i, 0)) {
        moveQueue.push(pos.returnUpdate(i, 0));
        i--;
    }

    // Moving forward
    int j = 1;
    while(pos.validUpdate(0, j)) {
        moveQueue.push(pos.returnUpdate(0, j));
        j++;
    }

    // Moving backward
    j = -1;
    while(pos.validUpdate(0, j)) {
        moveQueue.push(pos.returnUpdate(0, j));
        j--;
    }

    // Moving diagonal right
    i = 1;
    j = 1;
    while(pos.validUpdate(i, j)) {
        moveQueue.push(pos.returnUpdate(i, j));
        i++;
        j++;
    }

    // Moving diagonal left
    i = -1;
    j = 1;
    while(pos.validUpdate(i, j)) {
        moveQueue.push(pos.returnUpdate(i, j));
        i--;
        j++;
    }

    // Moving diagonal down-right
    i = 1;
    j = -1;
    while(pos.validUpdate(i, j)) {
        moveQueue.push(pos.returnUpdate(i, j));
        i++;
        j--;
    }

    // Moving diagonal down-left
    i = -1;
    j = -1;
    while(pos.validUpdate(i, j)) {
        moveQueue.push(pos.returnUpdate(i, j));
        i--;
        j--;
    }

    return moveQueue;
    
}
std::queue<BoardPosition> King::moveRange()
{
    std::queue<BoardPosition> moveQueue;

    if(pos.validUpdate(1, 0))   moveQueue.push(pos.returnUpdate(1, 0));
    if(pos.validUpdate(1, 1))   moveQueue.push(pos.returnUpdate(1, 1));
    if(pos.validUpdate(0, 1))   moveQueue.push(pos.returnUpdate(0, 1));
    if(pos.validUpdate(-1, 0))  moveQueue.push(pos.returnUpdate(-1, 0));
    if(pos.validUpdate(-1, -1)) moveQueue.push(pos.returnUpdate(-1, -1));
    if(pos.validUpdate(0, -1))  moveQueue.push(pos.returnUpdate(0, -1));
    if(pos.validUpdate(1, -1))  moveQueue.push(pos.returnUpdate(1, -1));
    if(pos.validUpdate(-1, 1))  moveQueue.push(pos.returnUpdate(-1, 1));
    BoardPosition test;
    moveQueue.push(test);

    return moveQueue;
    
}