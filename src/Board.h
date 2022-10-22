/***********************************************************
 *                      BOARD.h
 * 
 *  Handles all board logic:
 *      - Keeping track of board state
 *      - Moving pieces in board state
 *      - Telling SDL_Board to update screen #### POSSIBLY NOT THE BEST PLACE FOR THIS
 * 
 * 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "Piece.h"
#include "BoardPosition.h"
#include "BoardState.h"
#include "BoardMoves.h"

// EXTERNAL INCLUDES
#include <utility>



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
        BOARD_FLAGS boardFlags;

        // PRIVATE HELPER FUNCTIONS FOR MOVEMENT
        void preMoveTasks();
        void postMoveTasks();
        void movePiece(Piece* curPiece, BoardPosition newPos);
        void takePiece(Piece* curPiece, BoardPosition newPos);
    
    public:
        // CONSTRUCTORS
        Board() {newGame();};
        Board(BoardState state_) : state(state_) {};
        ~Board() {state.clearBoard();};
        void newGame();    

        // STATE-RELATED FUNCTIONS
        void processPromotion(PIECE_TYPE newType);
        bool processUpdate(BoardPosition oldPos, BoardPosition newPos);
        
        // RETURNS
        BoardState returnState() {return state;};   
        MovementQueue returnMovementQueue(BoardPosition pos);
        BOARD_FLAGS returnBoardFlags() {return boardFlags;};
        PIECE_COLOUR returnTurn() {return state.currentTurn;};
        // PositionQueue returnPositionQueue(BoardPosition pos);
        
};