/***********************************************************
 *              GAMEPLAYMANAGER IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "GameplayManager.h"

// EXTERNAL INCLUDES
#include <iostream>


BOARD_EVENT
GameplayManager::processBoardClick(const BoardPosition &prevClick, const BoardPosition &curClick)
{
    // If there isn't a previous coordinate registered, return an overlay of the current position
    if (curClick.validPosition() && !prevClick.validPosition())
    {
        if(m_board->getState().pieceExists(curClick, m_board->getTurn()))
            return OVERLAY;
        else
            return INVALID;
    }
    // Else, check that the previous click was of the right colour
    else if(m_board->getState().pieceExists(prevClick, m_board->getTurn()))
    {
        
        // processUpdate returns 0 if the requested update is invalid
        if(!m_board->processUpdate(prevClick, curClick))
            return INVALID;
        else
        // procesUpdate returns 1 if the requested update is valid. 
        {
            if(m_turnhead != m_history.returnTurn())
            {
                m_history.truncateHistory(m_turnhead);
            }
            m_history.appendHistory(m_board->getState());
            m_turnhead = m_history.returnTurn();
        }
        /******************** GAME HISTORY ******************/

        // Check flags
        BOARD_FLAGS flags = m_board->getFlags();
        if(flags.pawnPromotion.first == 1)
            return PROMOTION;
        else if (flags.kingCheckmate.first == 1)
            return CHECKMATE;
        else
            return MOVE;
    }
    
    return DEFAULT;
}

void GameplayManager::newGame()
{
    m_board->newGame();
    m_history.newGame(m_board->getState());
    m_turnhead = m_history.returnTurn();
}



void GameplayManager::traverseHistory(DIRECTION direction)
{
    // Update turn head
    if(direction == FORWARD) {
        if(m_turnhead >= m_history.returnTurn())
            return;
        else
            m_turnhead++;
    }
    else {
        if(m_turnhead <= 1)
            return;
        else
            m_turnhead--;
    }

    // Set m_board state to turn head from history
    StatePtr newStatePtr = m_history.returnState(m_turnhead);
    m_board->newState(*newStatePtr);
}