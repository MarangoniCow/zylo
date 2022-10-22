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
enum BOARD_EVENT
{
    DEFAULT, MOVE, INVALID, OVERLAY, PROMOTION, CHECKMATE
};



class GameplayManager
{
    private:
        PLAY_FLAG* pawnPromotion;
        Board* board;
        PIECE_COLOUR currentTurn;

    public:
        // CONSTRUCTORS 
        GameplayManager(Board* board_) : board(board_) {
            board->newGame();
        };

        BOARD_EVENT processBoardClick(BoardPosition curPos, BoardPosition newPos);
        Board* gameBoard() {return board;};
        void newGame();
};
