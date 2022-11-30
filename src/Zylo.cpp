/***********************************************************
 *                   ZYLO IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "Zylo.h"
#include "ZyloPiece.h"
#include "ZyloState.h"
#include "Board.h"
#include "BoardDefs.h"

// EXTERNAL INCLUDES
#include <cmath>
#include <random>



Move Zylo::fetchBestMove()
{
    Move move;
    generateRandomMove(move);
    return move;
}

void Zylo::generateRandomMove(Move& move)
{
    // Evaluate the current state
    evaluateState(m_state);
}

void Zylo::evaluateState(BoardState& state)
{
    m_moves.changeState(&state);
    m_moves.processState();
}