/***********************************************************
 *              GAMEPLAY HISTORY IMPLEMENTATION
 ***********************************************************/



// INTERNAL INCLUDES
#include "GameplayHistory.h"

// EXTERNAL INCLUDES
#include <vector>

void GameplayHistory::appendHistory(const BoardState &state)
{
    // Push to vector
    historyVec.push_back(std::make_shared<BoardState>(state));
    // Update current turn
    turnCount = (int)size(historyVec);
}

StatePtr GameplayHistory::returnState(TURN n) const
{
    if(n > 0)
        return historyVec[n - 1];
    else
        return nullptr;
}

void GameplayHistory::truncateHistory(unsigned int n)
{
    if(n >= turnCount)
        return;
    else
    {
        historyVec.erase(historyVec.begin() + n, historyVec.end());
    }
}

void GameplayHistory::newGame(const BoardState& newState)
{
    historyVec.clear();
    appendHistory(newState);
}