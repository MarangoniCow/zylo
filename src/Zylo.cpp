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

    // Fetch all possible moves
    MovementState moves = m_moves.getMovementState();
    PieceVector   curPieces = m_moves.getCurrentPieces();

  
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, curPieces.size() - 1); // define the range

    Movements random_queue;
    BoardPosition pos;

    bool foundMove = 0;

    while (!foundMove)
    {
        // Select a random piece
        Piece* random_piece = curPieces[distr(gen)];

        // Fetch it's position
        pos = random_piece->position();

        // Define it's queue
        random_queue = moves.state[pos.x][pos.y];

        if(!random_queue.validMoves.empty())
            foundMove = 1;
    }

    // Ta daaaaaaa
    move.first  = pos;
    move.second = random_queue.validMoves.front();
}

void Zylo::evaluateState(BoardState& state)
{
    m_moves.changeState(&state);
    m_moves.processState();
}