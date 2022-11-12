/***********************************************************
 *              GAMEPLAYMANAGER IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "GameplayManager.h"

// EXTERNAL INCLUDES
#include <iostream>


BOARD_EVENT
GameplayManager::processBoardClick(const BoardPosition &curPos, const BoardPosition &newPos)
{
    

    // If nothing exists in the new position, return overlay of old position
    if (newPos.validPosition() && !curPos.validPosition())
    {
        if(board->returnState().pieceExists(newPos, board->returnTurn()))
        {
            BOARD_FLAGS boardFlags = board->returnBoardFlags();
            if(board->returnState().returnPiece(newPos)->returnID() == boardFlags.kingCheck.second)
                return INVALID;
            else
                return OVERLAY;
        }
        else
            return INVALID;
            
    }
    // Check for piece of the correct colour, can also throw in other checks here if necessary
    else if(board->returnState().pieceExists(curPos, board->returnTurn())) {
        
        // processUpdate returns 0 if no move was made.
        if(!board->processUpdate(curPos, newPos))
            return INVALID;
        else
        {
            if(turnHead != gameHistory.returnTurn())
            {
                gameHistory.truncateHistory(turnHead);
            }
            gameHistory.appendHistory(board->returnState());
            turnHead = gameHistory.returnTurn();
        }
        
            

        // Check flags
        BOARD_FLAGS flags = board->returnBoardFlags();
        if(flags.pawnPromotion.first == 1)
            return PROMOTION;
        else if (flags.kingCheckmate.first == 1)
            return CHECKMATE;
        else
            return MOVE;
    }
    else
        return DEFAULT;
}

void GameplayManager::newGame()
{
    board->newGame();
    gameHistory.newGame(board->returnState());
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