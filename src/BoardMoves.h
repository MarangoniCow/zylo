/***********************************************************
 *                      BOARDMOVES_H
 * 
 * Complete description of all possible moves on the board
 *  - movementState: accessed like BoardState, an [8][8] array of moves
 *   -checksState: accessed like BoardState, an [8][8] array of checks
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
typedef std::queue<std::pair<Piece*, RELPOS>> SightedQueue;



/**************** MOVEMENT QUEUE STRUCT *******************
 * Holds all valid and invalid, moves and takes.
 *********************************************************/




struct MovementQueue {


    PositionQueue validMoves;
    PositionQueue validTakes;
    PositionQueue invalidMoves;
    PositionQueue invalidTakes;
    SightedQueue sightedQueue;

    MovementQueue() {};
    MovementQueue(PositionQueue validMoves_, PositionQueue validTakes_, 
        PositionQueue invalidMoves_, PositionQueue invalidTakes_) :
        validMoves(validMoves_), validTakes(validTakes_),
        invalidMoves(invalidMoves_), invalidTakes(invalidTakes_) {};
    MovementQueue(PositionQueue validMoves_, PositionQueue validTakes_, PositionQueue invalidMoves_,
        PositionQueue invalidTakes_, SightedQueue sightedQueue_) :
        sightedQueue(sightedQueue_), validMoves(validMoves_), validTakes(validTakes_),
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

        ChecksQueue curChecks;
        ChecksQueue oppChecks;

        PIECE_COLOUR curCol;
        PIECE_COLOUR oppCol;
        

        // Private methods: Resets
        void resetFlags();
        void resetMoves();
        void fetchTurn();

        // Private methods: Special takes/moves (castling)
        void addSpecialTakes(Piece* piece, PositionQueue* validTakes);
        void addSpecialMoves(Piece* piece);
        bool canCastle(PIECE_ID ID, RELPOS relpos);

    public:
        // CONSTRUCTORS
        BoardMoves();
        BoardMoves(BoardState* statePtr_);

        // INITIALISATION & ADMIN
        void changeState(BoardState* statePtr_) {statePtr = statePtr_;};

        // MAIN METHODS
        void processState();
        void generateMovementRange(Piece* piece);
        void refineMovementRange(PIECE_COLOUR col);
        MovementQueue processMoveRange(Piece* piece, PositionQueue moveRange);

        // Check related functions
        bool    isChecked(PIECE_ID ID, ChecksQueue checksQueue);
        bool isCheckmated(PIECE_ID ID);

        // CHECK-RELATED FUNCTIONS
        PieceChecks pieceChecks(Piece* piece);
        PieceChecks pieceChecks(Piece* piece, ChecksQueue* checksQueue);
        ChecksQueue generateChecksList(PIECE_COLOUR col);
        
        // Returns
        MovementQueue returnMovementQueue(Piece* piece);
        MovementQueue returnMovementQueue(BoardPosition pos);
        ChecksQueue     returnChecksQueue(BoardPosition pos);

        // 'Refinement' methods
        PositionQueue returnNoncheckedMoves(Piece* piece);
        void restrictRevealedCheckMoves(Piece* piece);
};