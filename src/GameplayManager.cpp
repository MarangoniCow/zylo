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
    // 0) Define locals
    bool no_previous_click              = curClick.validPosition() && !prevClick.validPosition();
    bool previous_click_right_colour    = m_board->getState().pieceExists(prevClick, m_board->getTurn());
    bool current_click_right_colour     = m_board->getState().pieceExists(curClick, m_board->getTurn());


    // 1) If there isn't a previous click reigsterered, we can only return an overlay of the current position
    
    if (no_previous_click) {
        if (current_click_right_colour)
            return OVERLAY;
        else
            return INVALID;
    }

    // 2) If the previous click exists and was the right colour, we can attempt to find a movement
    if (previous_click_right_colour)
    {

        // 3) Attempt to process an update with the requested board coordinates:
        //      - Returns 0 if the requested update is invalid
        //      - Returns 1 if the requested update is valid 

        if (!m_board->processUpdate(prevClick, curClick))
            return INVALID;
        else
        {
            // Just a quick output for the current board state value
            ZyloState zyloState(m_board->getState());
            std::cout << zyloState.fetchStateRating() << std::endl;
        }


        // 4) Check if we're at the turn head, if not, truncate future history and append anyway
        if (m_turnhead != m_history.returnTurn())
        {
            m_history.truncateHistory(m_turnhead);
        }
        m_history.appendHistory(m_board->getState());
        m_turnhead = m_history.returnTurn();
        
        // 5) Check gameplay flags for promotion or checkmate

        BOARD_FLAGS flags = m_board->getFlags();
        if (flags.kingCheckmate.first)
            return CHECKMATE;
        else if (flags.pawnPromotion.first)
            return PROMOTION;
        else
            return MOVE;
    }
    

    // Otherwise, return default
    return DEFAULT;


   
}

void GameplayManager::newGame()
{
    m_board->newGame();
    m_history.newGame(m_board->getState());
    m_turnhead = m_history.returnTurn();
}

Move GameplayManager::fetchZyloMove()
{
    BoardState current = m_board->getState();
    m_zylo.state(current);
    return m_zylo.fetchBestMove();
}

PLAYER_TYPE GameplayManager::currentPlayerType()
{
    if(m_player1.colour == m_board->getTurn())
        return m_player1.type;
    else
        return m_player2.type;
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