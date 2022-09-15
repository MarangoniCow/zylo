/***********************************************************
 *                      PIECETYPES.H
 * 
 *  Declarations of specific types
 * 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES  
#include "Piece.h"

// EXTERNAL INCLUDES
#include <string> 




class Pawn : public Piece
{
    private:
        static PieceDescriptor pawn_description; 
    public:
        Pawn() : Piece(pawn_description) {};
};

