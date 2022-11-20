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
        if(board->getState().pieceExists(curClick, board->getTurn()))
            return OVERLAY;
        else
            return INVALID;
    }
    // Else, check that the previous click was of the right colour
    else if(board->getState().pieceExists(prevClick, board->getTurn()))
    {
        
        // processUpdate returns 0 if the requested update is invalid
        if(!board->processUpdate(prevClick, curClick))
            return INVALID;
        // else
        //     historyActions;

        /******************** GAME HISTORY ******************/
        // else
        // // procesUpdate returns 1 if the requested update is valid. 
        // {
        //     if(turnHead != gameHistory.returnTurn())
        //     {
        //         gameHistory.truncateHistory(turnHead);
        //     }
        //     gameHistory.appendHistory(board->getState());
        //     turnHead = gameHistory.returnTurn();
        // }
        /******************** GAME HISTORY ******************/

        // Check flags
        BOARD_FLAGS flags = board->getFlags();
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
    board->newGame();
    gameHistory.newGame(board->getState());
    turnHead = gameHistory.returnTurn();
}



void GameplayManager::traverseHistory(DIRECTION direction)
{
    // Update turn head
    if(direction == FORWARD) {
        if(turnHead >= gameHistory.returnTurn())
            return;
        else
            turnHead++;
    }
    else {
        if(turnHead <= 1)
            return;
        else
            turnHead--;
    }

    // Set board state to turn head from history
    StatePtr newStatePtr = gameHistory.returnState(turnHead);
    board->newState(*newStatePtr);
}