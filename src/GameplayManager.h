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
#include "Zylo.h"



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
    protected:
        PLAY_FLAG*      pawnPromotion;
        Board*          m_board;

        COLOUR          m_turn; 
        TURN            m_turnhead;
        GameplayHistory m_history;

        Zylo*           m_zylo;

    public:
        // CONSTRUCTORS 
        GameplayManager(Board* board) : m_board(board) {};

        // Gameplay events
        BOARD_EVENT         processBoardClick       (const BoardPosition &curPos, const BoardPosition &newPos);
        void                newGame                 ();

        void                board                   (Board* board)  { m_board = board; };
        Board*              board                   ()              { return m_board;} ;
        
        TURN                turnNumber              () {return m_history.returnTurn();};
        

        // History events
        void                traverseHistory         (DIRECTION direction);
        void                newHistoryBranch        (TURN n);
        StatePtr            fetchHistory            (TURN n);
};
