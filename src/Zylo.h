/***********************************************************
 *                      ZYLO
 * 
 * Homebrewed Chess engine, wrapper class for all Zylo
 * related functions. 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "ZyloPiece.h"
#include "ZyloBoard.h"
#include "ZyloWeightings.h"
#include "Board.h"

// EXTERNAL INCLUDES


class Zylo
{
    private:
        BoardState m_state;
        BoardMoves m_moves;
    
    public:
        Zylo    (BoardState state) :
                m_state(state)
                {};

        Move                    fetchBestMove       ();
        void                    generateRandomMove  (Move& move);
        void                    evaluateState       (const BoardState& state);
};