/***********************************************************
 *          ZYLOWEIGHTINGS IMPLEMENTATION
 ***********************************************************/


#include "ZyloWeightings.h"

CP ZyloWeightings::PieceWeight(PIECE_TYPE type)
{
    switch(type)
    {
        case PAWN:
            return PieceWeightings::Pawn; 
        case ROOK:
            return PieceWeightings::Rook;
        case KNIGHT:
            return PieceWeightings::Knight;
        case BISHOP:
            return PieceWeightings::Bishop;
        case QUEEN:
            return PieceWeightings::Queen;
        default:
            return 0;
    }
}

CP ZyloWeightings::PositionWeight(PIECE_TYPE type, BoardPosition pos)
{
    if(!pos.validPosition())
        return 0;
    else
        return PositionalWeightings::defaults[pos.x][pos.y];
}
