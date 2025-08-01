/***********************************************************
 *                      ZYLOPIECE
 * 
 * Class which deals with evaluating the value of each piece
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "Board.h"
#include "BoardMoves.h"
#include "Piece.h"
#include "ZyloDefs.h"

// EXTERNAL INCLUDES

class ZyloPiece
{
    // Static class
    private:
        ZyloPiece();

    protected:
        static CP multiplicativeMethod  (Piece piece);
        static CP additiveMethod        (Piece piece);
    public:
        static CP getPieceRating(Piece piece);
};