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
#include "Player.h"
#include "Board.h"
#include "GameplayHistory.h"
#include "Zylo.h"

// EXTERNAL INCLUDES
#include <stdexcept>
 

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
    PLAY_FLAG* pawnPromotion;
    Board* m_board;

    COLOUR          m_turn;
    TURN            m_turnhead;
    GameplayHistory m_history;

    Zylo m_zylo;

    Player          m_player1;
    Player          m_player2;

public:
    // CONSTRUCTORS 
    GameplayManager(Board* board, const Player& player1, const Player& player2) :
                    m_board(board)
                    {
                        if(player1.colour == player2.colour)
                            throw std::invalid_argument("Gameplay manager: players must be different colours");
                        m_player1 = player1;
                        m_player2 = player2;

                        if(player1.type == ZYLO || player2.type == ZYLO)
                            m_zylo = Zylo();

                    }
    
    // Gameplay events 
    BOARD_EVENT         processBoardClick(const BoardPosition& curPos, const BoardPosition& newPos);
    Move                fetchZyloMove();
    void                newGame();

    void                board(Board* board) { m_board = board; };
    Board* board() { return m_board; };

    TURN                turnNumber() { return m_history.returnTurn(); };

    void                playerOne(Player player)    { m_player1 = player; };
    Player              playerOne() const           { return m_player1; }

    void                playerTwo(Player player)    { m_player2 = player; };
    Player              playerTwo() const           { return m_player2; }

    PLAYER_TYPE         currentPlayerType();
        

    // History events
    void                traverseHistory         (DIRECTION direction);
    void                newHistoryBranch        (TURN n);
    StatePtr            fetchHistory            (TURN n);
};
