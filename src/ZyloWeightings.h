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
        static CP PositionWeight(Piece piece);
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
    // First row of values corresponds to state[0][0-7]
    // Second row of values corresponds to state[1][0-7]
    // Etc, etc
    const double defaults[8][8] =           {   {0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0},
                                            };
    
    const double pawn[8][8]       =         {   {0, 0, 0.1,  0.2,  0,   0, 0, 0},
                                                {0, 0, 0.25, 0,    0,   0, 0, 0},
                                                {0, 0, 0.1,  0.15, 0,   0, 0, 0},
                                                {0, 0, 0.15, 0.25, 0.2, 0, 0, 0},
                                                {0, 0, 0.15, 0.25, 0.2, 0, 0, 0},
                                                {0, 0, 0.1,  0.15, 0,   0, 0, 0},
                                                {0, 0, 0.25, 0,    0,   0, 0, 0},
                                                {0, 0, 0.1,  0.2,  0,   0, 0, 0}
                                            };
};