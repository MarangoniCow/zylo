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
        static CP PieceWeight(PIECE_TYPE type, PHASE phase);
        static CP PositionWeight(Piece piece, PHASE phase);
};

namespace PieceWeightings
{
    const CP Pawn   = 100;
    const CP Knight = 300;
    const CP Bishop = 300;
    const CP Rook   = 500;
    const CP Queen  = 1000;
};

