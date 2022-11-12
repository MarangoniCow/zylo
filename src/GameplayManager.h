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
#include "GameplayHistory.h"
// #include "Zylo.h"


// EXTERNAL INCLUDES
enum BOARD_EVENT
{
    DEFAULT, MOVE, INVALID, OVERLAY, PROMOTION, CHECKMATE
};
enum DIRECTION
{
    FORWARD = 1, BACKWARD = -1
};



class GameplayManager
{
    private:
        PLAY_FLAG* pawnPromotion;
        Board* board;
        PIECE_COLOUR currentTurn;
        GameplayHistory gameHistory;
        TURN turnHead;

    public:
        // CONSTRUCTORS 
        GameplayManager(Board* board_) : board(board_) {};

        BOARD_EVENT processBoardClick(const BoardPosition &curPos, const BoardPosition &newPos);
        Board* gameBoard() {return board;};
        void newGame();
        TURN turnNumber() {return gameHistory.returnTurn();};
        StatePtr returnHistory(TURN n);
        void newHistoryBranch(TURN n);
        void traverseHistory(DIRECTION direction);
};
