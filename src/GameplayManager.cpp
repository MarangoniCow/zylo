/***********************************************************
 *              GAMEPLAYMANAGER IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "GameplayManager.h"

// EXTERNAL INCLUDES
#include <iostream>

// The heck is wrong with identations on this file and this file only? They seem to be half the size, it's extremely annoying!

BOARD_EVENT GameplayManager::processBoardClick(BoardPosition curPos, BoardPosition newPos)
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

