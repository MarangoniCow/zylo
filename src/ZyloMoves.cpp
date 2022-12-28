/***********************************************************
 *                   ZYLOMOVES IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "ZyloMoves.h"
#include "ZyloDefs.h"

// EXTERNAL INCLUDES


void ZyloMoves::processState()
{
    m_moveClass.changeState(&m_state);
    m_moveClass.processState();
    generateMoves();
}

void ZyloMoves::generateMoves()
{
    // Reset necessary variables
    m_moves.clear();

    // Temporary scope: Will probably move to its own place as methodology evolves
    {
        // Fetch current pieces
        const PieceVector& curPieces = m_moveClass.getCurrentPieces();

        // Fetch current move state
        const MovementState moveState = m_moveClass.getMovementState();

        // Iterate through all the pieces, adding their possible moves and takes to the move list
        for(auto piece:curPieces)
        {

            BoardPosition pos(piece->position());
            const Movements& moves = moveState.state[pos.x][pos.y];

            auto createMovesFromVector = [&](const PositionVector& posVec)
            {
                for(auto newPos:posVec)
                    m_moves.push_back(Move(pos, newPos));   
            };

            createMovesFromVector(moves.validMoves);
            createMovesFromVector(moves.validTakes);
        }
    }
}


