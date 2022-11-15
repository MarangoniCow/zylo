/***********************************************************
 *                      BOARD.h
 * 
 *  Handles all board logic:
 *      - Link together BoardState and BoardMoves
 *      - Process updates to state if they're valid
 *      - Coallates board flags
 *      - Introduce management functions like newGame and processPromotion
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "BoardDefs.h"

#include "Piece.h"
#include "BoardPosition.h"
#include "BoardState.h"
#include "BoardMoves.h"


struct BOARD_FLAGS
{
    PLAY_FLAG kingCheck;
    PLAY_FLAG kingCheckmate;
    PLAY_FLAG pawnPromotion;

    BOARD_FLAGS()
    {
        RESET_FLAGS();
    }
    void RESET_FLAGS()
    {
        kingCheck.first = 0;
        kingCheck.second = UINT16_MAX;
        kingCheckmate.first = 0;
        kingCheckmate.second = UINT16_MAX;
        pawnPromotion.first = 0;
        pawnPromotion.second = UINT16_MAX;
    }
};


class Board {

    protected:
        // MEMBER VARIABLES
        BoardState state;
        BoardMoves boardMoves;
        Move lastMove;
        BOARD_FLAGS boardFlags;

        // PRIVATE HELPER FUNCTIONS FOR MOVEMENT
        void preMoveTasks(BoardPosition curPos, BoardPosition tarPos);
        void postMoveTasks();
        void movePiece(Piece* curPiece, BoardPosition newPos);
        void takePiece(Piece* curPiece, BoardPosition newPos);
    
    public:
        // CONSTRUCTORS & ADMIN
        Board() {};
        Board(BoardState state_);
        ~Board() {state.clearBoard();};
        void newGame();
        void newState(BoardState state_);

        // STATE-RELATED FUNCTIONS
        bool processUpdate(BoardPosition oldPos, BoardPosition newPos);
        void processPromotion(PIECE_TYPE newType);
        
        // RETURNS
        BoardState      returnState()   {return state;};   
        MovementQueue   returnMovementQueue (BoardPosition pos);
        BOARD_FLAGS     returnBoardFlags()  {return boardFlags;};
        PIECE_COLOUR    returnTurn()    {return state.currentTurn;};
        Move            returnLastMove()    {return lastMove;}
        
};