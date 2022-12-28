/***********************************************************
 *                      ZYLO
 * 
 * Homebrewed Chess engine, wrapper class for all Zylo
 * related functions. 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "ZyloMoves.h"
#include "ZyloPiece.h"
#include "ZyloState.h"
#include "ZyloWeightings.h"
#include "Board.h"

// EXTERNAL INCLUDES


class Zylo
{
    private:
        BoardState m_stateHead;
        ZyloMoves m_zyloMoves;
   
    public:
        Zylo    ()
        {
            m_stateHead = BoardState();
        }
        Zylo    (BoardState state)  :
                m_stateHead(state)
                {};

        Move                    fetchBestMove       ();

        void                    state(const BoardState& state_)     { m_stateHead = state_;}
        BoardState              state() const                       { return m_stateHead; }
};