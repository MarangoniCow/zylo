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

// EXTERNAL INCLUDES
#include <utility>


class Board {

    protected:
        // MEMBER VARIABLES
        BoardState state;

        // PRIVATE HELPER FUNCTIONS FOR MOVEMENT
        void preMoveTasks();
        void postMoveTasks();
        void movePiece(Piece* curPiece, BoardPosition newPos);
        void takePiece(Piece* curPiece, BoardPosition newPos);
    
    public:
        // CONSTRUCTORS
        Board() {};
        Board(BoardState state_) : state(state_) {};
        ~Board() {};
        void newGame() {state.initialiseBoard();};
        
        
        // STATE-RELATED FUNCTIONS
        void processPromotion(PIECE_TYPE newType);
        bool processUpdate(BoardPosition oldPos, BoardPosition newPos);
        
        MovementQueue retrievePositionQueue(int i, int j);
        
        
        
        // MOVEMENT-RELATED FUNCTIONS
        MovementQueue generateMovementRange(BoardPosition oldPos);
        MovementQueue processMoveRange(PositionQueue moveRange, BoardPosition curPiecePos);
        void addSpecialMoves(Piece* currentPiece, PositionQueue* validMoves);
        void addSpecialTakes(Piece* currentPiece, PositionQueue* validTakes);
        
        // CHECK-RELATED FUNCTIONS
        PieceChecks pieceChecks(PIECE_ID ID);
        ChecksQueue generateChecksList(PIECE_COLOUR col);

        // KING-RELATED FUNCTIONS
        bool isChecked(PIECE_ID ID);
        bool isCheckmated(PIECE_ID ID);
        bool canCastle(PIECE_ID ID, RELPOS relpos);
        PIECE_ID fetchKingID(PIECE_COLOUR col);

        // FLAG RELATED FUNCTIONS
        void resetFlags() {state.boardFlags.RESET_FLAGS();};

        // RETURNS
        BoardState returnState() {return state;};   
        
        
};