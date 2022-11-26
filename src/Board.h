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
        kingCheck.first = false;
        kingCheck.second.resetPosition();
        kingCheckmate.first = false;
        kingCheckmate.second.resetPosition();
        pawnPromotion.first = false;
        pawnPromotion.second.resetPosition();
    }
};


class Board {

    protected:
        // MEMBER VARIABLES
        BoardState state;
        BoardMoves boardMoves;
        BOARD_FLAGS boardFlags;

        // PRIVATE HELPER FUNCTIONS FOR MOVEMENT
        void            postMoveTasks       (const BoardPosition& curPos, const BoardPosition& tarPos);
        void            movePiece           (Piece &curPiece, const BoardPosition& newPos);
        void            takePiece           (Piece &curPiece, const BoardPosition& newPos);
    
    public:
        // CONSTRUCTORS & ADMIN
        Board       () {};
        Board       (BoardState state_);
        ~Board      () {state.resetBoard();};


        void            newGame             ();
        void            newState            (BoardState state_);

        // STATE-RELATED FUNCTIONS
        bool            processUpdate       (BoardPosition oldPos, BoardPosition newPos);
        void            processPromotion    (PIECE_TYPE newType);

        MovementQueue   movementQueue       (BoardPosition pos);
        
        // RETURNS
        BoardState      getState    () const    { return state; };   
        BOARD_FLAGS     getFlags    () const    { return boardFlags; };
        COLOUR          getTurn     () const    { return state.turn(); };
        Move            getLastMove () const    { return state.lastMove(); }
        
};