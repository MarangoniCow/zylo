/***********************************************************
 *                      BOARDSTATE.h
 * 
 * Complete description of all possible moves on the board
 *  - 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "Piece.h"
#include "BoardPosition.h"
#include "BoardState.h"

// EXTERNAL INCLUDES
#include <utility>

typedef std::pair<bool, PIECE_ID> PLAY_FLAG;
typedef std::pair<PIECE_ID, PieceQueue> PieceChecks;
typedef std::queue<PieceChecks> ChecksQueue;

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




class BoardMoves {

    protected:

        // Member variables
        BoardState* statePtr;
        PLAY_FLAG kingCheck;
        PLAY_FLAG kingCheckmate;
        MovementQueue movementState[8][8];
        PieceChecks checksState[8][8];
        ChecksQueue opponentChecks;
        ChecksQueue friendlyChecks;

        // Private methods: Resets
        void resetFlags();
        void resetMoves();
        // Private methods: Special moves/takes
        void addSpecialTakes(Piece* piece, PositionQueue* validTakes);
        void addSpecialMoves(Piece* piece);


    public:
        // CONSTRUCTORS
        BoardMoves();
        BoardMoves(BoardState* statePtr_);

        // INITIALISES/MAIN METHODS
        void changeState(BoardState* statePtr_) {
            statePtr = statePtr_;
        }
        void processState();
        void generateMovementRange(Piece* piece);
        MovementQueue processMoveRange(Piece* piece, PositionQueue moveRange);
        void refineMovementRange(PIECE_COLOUR col);





        // Check related functions
        // KING-RELATED FUNCTIONS
        bool isChecked(PIECE_ID ID, ChecksQueue checksQueue);
        bool isCheckmated(PIECE_ID ID);
        bool canCastle(PIECE_ID ID, RELPOS relpos);
        Piece* fetchKing(PIECE_COLOUR col);

        // CHECK-RELATED FUNCTIONS
        PieceChecks pieceChecks(Piece* piece);
        PieceChecks pieceChecks(Piece* piece, ChecksQueue* checksQueue);
        ChecksQueue generateChecksList(PIECE_COLOUR col);
        
        
        // Returns
        MovementQueue returnMovementQueue(BoardPosition pos);
        ChecksQueue returnChecksQueue(BoardPosition pos);      
};