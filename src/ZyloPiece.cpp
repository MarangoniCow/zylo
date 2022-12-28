/***********************************************************
 *                 ZYLOPIECE IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "ZyloWeightings.h"
#include "ZyloPiece.h"

// EXTERNAL INCLUDES

CP ZyloPiece::getPieceRating(Piece piece) 
{
    return additiveMethod(piece);
}

CP ZyloPiece::additiveMethod(Piece piece)
{
    return ZyloWeightings::PieceWeight(piece.type()) + ZyloWeightings::PositionWeight(piece);
}