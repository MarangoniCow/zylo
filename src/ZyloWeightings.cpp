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
    if(piece.colour() == BLACK)
    {
        pos.x = 7 - pos.x;
        pos.y = 7 - pos.y;
    }

    switch(piece.type())
    {
        case PAWN:
            return PositionalWeightings::pawn[pos.x][pos.y]; 
        case ROOK:
            return PositionalWeightings::rook[pos.x][pos.y]; 
        case KNIGHT:
            return PositionalWeightings::knight[pos.x][pos.y]; 
        case BISHOP:
            return PositionalWeightings::bishop[pos.x][pos.y]; 
        case QUEEN:
            return PositionalWeightings::queen[pos.x][pos.y]; 
        case KING:
            return PositionalWeightings::king[pos.x][pos.y]; 
        default:
            return 0;
    }
}
