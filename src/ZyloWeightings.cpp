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

CP ZyloWeightings::PositionWeight(Piece piece)
{
    BoardPosition pos(piece.position());
    if(piece.colour() == WHITE)
        return PositionalWeightings::pawn[pos.x][pos.y];
    else
        return PositionalWeightings::pawn[7 - pos.x][7 - pos.y];
}
