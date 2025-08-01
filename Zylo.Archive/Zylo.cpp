/***********************************************************
 *                   ZYLO IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "Zylo.h"
#include "ZyloPiece.h"
#include "ZyloState.h"
#include "Board.h"
#include "BoardDefs.h"
#include "ContainerFunctions.h"

// EXTERNAL INCLUDES
#include <cmath>
#include <random>



Move Zylo::fetchBestMove()
{
    Move move;
    m_zyloMoves.state(m_stateHead);
    m_zyloMoves.processState();
    const MoveVector& moveVector = m_zyloMoves.fetchMoves();

    std::vector<CP> moveScores;
    moveScores.resize(moveVector.size());

    ZyloState zyloState;

    for(int i = 0; i < moveVector.size(); i++)
    {
        BoardState newState = m_stateHead;
        newState.movePiece(moveVector[i]);
        zyloState.changeState(newState);
        moveScores[i] = zyloState.fetchStateRating();
    }

    int idx;
    if(m_stateHead.turn() == WHITE)
        idx = vectorMaxPosition(moveScores);
    else
        idx = vectorMinPosition(moveScores);
        


    return moveVector[idx];
}


