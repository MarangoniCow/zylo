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

        // Movement and checks state
        MovementQueue movementState[8][8];
        PieceChecks checksState[8][8];

        // Common memeber variables regularly accessed
        ChecksQueue curChecks;
        ChecksQueue oppChecks;

        PIECE_COLOUR curCol;
        PIECE_COLOUR oppCol;

        Piece* curKing;
        Piece* oppKing;

        int curDir;

        
        

        // Private methods: Resets
        void resetFlags();
        void resetMoves();
        void setTurnDependencies();

        // Private methods: Special takes/moves (castling)
        void addSpecialTakes(Piece* piece, PositionQueue* validTakes);
        void addCastling(Piece* piece);
        void verifyMate();

    public:
        // CONSTRUCTORS
        BoardMoves();
        BoardMoves(BoardState* statePtr_);

        // INITIALISATION & ADMIN
        void changeState(BoardState* statePtr_) {statePtr = statePtr_;};

        // MAIN METHODS
        void processState();
        void generateMovementRange(Piece* piece);
        MovementQueue processMoveRange(Piece* piece, PositionQueue moveRange);
 
        // CHECK-RELATED FUNCTIONS
        PieceChecks pieceChecks(Piece* piece);
        PieceQueue  pieceCheckedBy(PIECE_ID ID, ChecksQueue checksQueue);
        PieceQueue  positionCheckedBy(BoardPosition pos, PIECE_COLOUR oppCol);

        // REFINEMENTS
        PositionQueue returnSafeMoves(Piece* piece);
        PositionQueue returnSafeTakes(Piece* piece);
        void restrictRevealedCheckMoves(Piece* piece);
        void restrictKingMoves(PIECE_COLOUR col);
        void restrictToBlockingMoves(Piece* pieceToProtect, Piece* pieceToMove, Piece* targetPiece,
                                         PositionQueue* validMoves, PositionQueue* validTakes);
        void removeInvalidCheckedMoves(PieceQueue kingCheckedBy);

        // RETURNS
        MovementQueue returnMovementQueue(Piece* piece);
        MovementQueue returnMovementQueue(BoardPosition pos);
        ChecksQueue   returnChecksQueue(PIECE_COLOUR col);
};