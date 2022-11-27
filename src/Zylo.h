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
#include "ZyloState.h"
#include "ZyloWeightings.h"
#include "Board.h"

// EXTERNAL INCLUDES


class Zylo
{
    private:
        BoardState m_state;
        BoardMoves m_moves;
    
    public:
        Zylo    ()
        {
            m_moves = BoardMoves();
            m_state = BoardState();
        }
        Zylo    (BoardState state)  :
                m_state(state)
                {};

        Move                    fetchBestMove       ();
        void                    generateRandomMove  (Move& move);
        void                    evaluateState       (BoardState& state);

        void                    state(const BoardState& state_)     { m_state = state_;}
        BoardState              state() const                       { return m_state; }
};