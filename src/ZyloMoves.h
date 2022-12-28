/***********************************************************
 *                      ZYLOMOVES
 * 
 * Analyses possible moves for a given state
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "Board.h"
#include "ZyloMoves.h"
#include "ZyloDefs.h"

// EXTERNAL INCLUDES
#include <vector>


class ZyloMoves
{
    protected:
        BoardState m_state;
        BoardMoves  m_moveClass;
        MoveVector  m_moves;

        

    protected:
        
        void generateMoves();

    public:
        ZyloMoves() {};
        ZyloMoves(BoardState state) : m_state(state) {};

        const MoveVector& fetchMoves() const    { return m_moves; }

        void state(BoardState state)            { m_state = state;}
        void processState();
        
};