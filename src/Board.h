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
        kingCheck.second = nullptr;
        kingCheckmate.first = 0;
        kingCheckmate.second = nullptr;
        pawnPromotion.first = 0;
        pawnPromotion.second = nullptr;
    }
};


class Board {

    protected:
        // MEMBER VARIABLES
        BoardState state;
        BoardMoves boardMoves;
        BOARD_FLAGS boardFlags;

        // PRIVATE HELPER FUNCTIONS FOR MOVEMENT
        void postMoveTasks(const BoardPosition& curPos, const BoardPosition& tarPos);
        void movePiece(Piece* curPiece, const BoardPosition& newPos);
        void takePiece(Piece* curPiece, const BoardPosition& newPos);
    
    public:
        // CONSTRUCTORS & ADMIN
        Board() {};
        Board(BoardState state_);
        ~Board() {state.resetBoard();};
        void newGame();
        void newState(BoardState state_);

        // STATE-RELATED FUNCTIONS
        bool processUpdate(BoardPosition oldPos, BoardPosition newPos);
        void processPromotion(PIECE_TYPE newType);

        MovementQueue   returnMovementQueue (BoardPosition pos);
        
        // RETURNS
        BoardState      getState    () { return state; };   
        BOARD_FLAGS     getFlags    () { return boardFlags; };
        COLOUR    getTurn     () { return state.turn(); };
        Move            getLastMove () { return state.lastMove(); }
        
};