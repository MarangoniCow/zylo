/***********************************************************
 *                    GAMEPLAYMANAGER
 * 
 *  Class to handle turns, keep track of taken pieces,
 *  turn history, winning conditions, etc., etc.
 * 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "Board.h"
// #include "Zylo.h"
// #include "BoardHistory.h"

// EXTERNAL INCLUDES


class GameplayManager
{
    private:
        PLAY_FLAG* pawnPromotion;
        Board* board;

    public:
        // CONSTRUCTORS 
        GameplayManager(Board* board_) : board(board_) {};
        ~GameplayManager();

        bool processClick(BoardPosition curPos, BoardPosition newPos);
        Board* gameBoard() {return board;};
};
