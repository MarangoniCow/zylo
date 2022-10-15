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

// EXTERNAL INCLUDES
#include <SDL2/SDL.h>

typedef std::queue<BoardPosition> PositionQueue;
typedef std::queue<PIECE_ID> IDQueue;

struct BoardState {
    Piece* piecesCurr[8][8];
    Piece* piecesPrev[8][8];

    BoardState() {
    for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                piecesCurr[i][j] = NULL;
                piecesPrev[i][j] = NULL; 
            };
        };
    };
};



struct MovementQueue {
    PositionQueue validMoves;
    PositionQueue validTakes;
    PositionQueue invalidMoves;
    PositionQueue invalidTakes;

    MovementQueue() {};
    MovementQueue(PositionQueue validMoves_, PositionQueue validTakes_, 
                    PositionQueue invalidMoves_, PositionQueue invalidTakes_)
        :   validMoves(validMoves_), validTakes(validTakes_),
            invalidMoves(invalidMoves_), invalidTakes(invalidTakes_) {};
};

class Board {

    protected:
        // MEMBER VARIABLES
        BoardState state;

        // PRIVATE HELPER FUNCTIONS FOR MOVEMENT
        void movePiece(Piece* curPiece, BoardPosition newPos);
        void takePiece(Piece* curPiece, BoardPosition newPos);
        void removePiece(Piece* pieceToDelete);

    public:
        // CONSTRUCTORS
        Board() {};
        ~Board() {};
        void initialiseBoard();
        
        // STATE-RELATED FUNCTIONS
        void addPieceToState(Piece* newPiece);
        void processClick(BoardPosition oldPos, BoardPosition newPos);
        
        // MOVEMENT-RELATED FUNCTIONS
        MovementQueue generateMovementRange(BoardPosition oldPos);
        MovementQueue processMoveRange(PositionQueue moveRange, BoardPosition curPiecePos);
        
        // CHECK-RELATED FUNCTIONS
        IDQueue pieceChecks(PIECE_ID ID);
        IDQueue generateCheckedList(PIECE_COLOUR col);
        bool isChecked(PIECE_ID ID);
        bool canCastle(PIECE_ID ID, RELPOS relpos);

        // RETURNS
        BoardState returnState() {return state;};       
        
        
};