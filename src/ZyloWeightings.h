/***********************************************************
 *                      ZYLOWEIGHTINGS
 * 
 * Handles piece weightings
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "Piece.h"
#include "BoardDefs.h"
#include "ZyloDefs.h"

// 'Static' class

class ZyloWeightings
{
    // Static class: No instance can be made.
    private:
        ZyloWeightings();

    public:
        static CP PieceWeight(PIECE_TYPE type);
        static CP PositionWeight(PIECE_TYPE type, BoardPosition pos);
};

namespace PieceWeightings
{
    const CP Pawn   = 1;
    const CP Knight = 3;
    const CP Bishop = 3;
    const CP Rook   = 5;
    const CP Queen  = 9;
};


namespace PositionalWeightings
{

    const double defaults[8][8] =           {   {1, 1, 1, 1, 1, 1, 1, 1},
                                                {1, 1, 1, 1, 1, 1, 1, 1},
                                                {1, 1, 1, 1, 1, 1, 1, 1},
                                                {1, 1, 1, 1, 1, 1, 1, 1},
                                                {1, 1, 1, 1, 1, 1, 1, 1},
                                                {1, 1, 1, 1, 1, 1, 1, 1},
                                                {1, 1, 1, 1, 1, 1, 1, 1},
                                                {1, 1, 1, 1, 1, 1, 1, 1}
                                            };
    
    const double biasMiddle[8][8]       =  {    {1, 1, 1, 1, 1, 1, 1, 1},
                                                {1, 1, 1, 1, 1, 1, 1, 1},
                                                {1, 1, 2, 2, 2, 2, 1, 1},
                                                {1, 1, 3, 3, 3, 3, 1, 1},
                                                {1, 1, 3, 3, 3, 3, 1, 1},
                                                {1, 1, 2, 2, 2, 2, 1, 1},
                                                {1, 1, 1, 1, 1, 1, 1, 1},
                                                {1, 1, 1, 1, 1, 1, 1, 1}
                                            };
};