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
    m_historyVec.push_back(std::make_shared<BoardState>(state));
    // Update current turn
    m_turnCount = (int)size(m_historyVec);
}

StatePtr GameplayHistory::returnState(TURN n) const
{
    if(n > 0)
        return m_historyVec[n - 1];
    else
        return nullptr;
}

void GameplayHistory::truncateHistory(unsigned int n)
{
    if(n >= m_turnCount)
        return;
    else
    {
        m_historyVec.erase(m_historyVec.begin() + n, m_historyVec.end());
    }
}

void GameplayHistory::newGame(const BoardState& newState)
{
    m_historyVec.clear();
    appendHistory(newState);
}