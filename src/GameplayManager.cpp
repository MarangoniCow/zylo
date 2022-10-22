/***********************************************************
 *              GAMEPLAYMANAGER IMPLEMENTATION
  ***********************************************************/

// INTERNAL INCLUDES
#include "GameplayManager.h"

// EXTERNAL INCLUDES
#include <iostream>

// GameplayManager::GameplayManager()
// {
//     // Do stuff
// }

// GameplayManager::~GameplayManager()
// {
//     // Do other stuff
// }

bool GameplayManager::processClick(BoardPosition curPos, BoardPosition newPos)
{
    // 1) Check if the move is valid
    // bool Board::validMovement(move)

    // 2) If it's a valid move, send the current state to history
    // addToHistory(board.returnState());

    // 3) Ask board to update the move internally
    // board.processUpdate(curPos, newPos);

    // 4) Check for flags (could be passed straight on)

    // 5) Allow Zylo to start processing moves (ideally, multithreaded so that movements can be displayed)
    // Zylo::generateSuggestedMove(board.returnState())



    return 0;
}