/***********************************************************
 *                      ZYLOBOARD
 * 
 * Class which deals with evaluating the value of a BoardState
 * 
 **********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "ZyloDefs.h"
#include "Board.h"
#include "BoardMoves.h"
#include "Piece.h"

// EXTERNAL INCLUDES

class ZyloState
{
    protected:
        BoardState m_state;

    protected:
        CP      sumPieceValues(COLOUR col);
    
    public:
        ZyloState() {};
        ZyloState(const BoardState& state) : m_state(state) {};

        void    changeState(const BoardState& state)       { m_state = state; }
        CP      fetchStateRating();        
};