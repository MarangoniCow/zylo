/***********************************************************
 *                   ZYLO IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "Zylo.h"
#include "ZyloPiece.h"
#include "ZyloBoard.h"
#include "Board.h"
#include "BoardDefs.h"

// EXTERNAL INCLUDES
#include <cmath>



Move Zylo::fetchBestMove()
{
    Move move;
    generateRandomMove(move);
    return move;
}

void Zylo::generateRandomMove(Move& move)
{


}

void Zylo::evaluateState(const BoardState& state)
{

}