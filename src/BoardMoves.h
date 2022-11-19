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
#include "BoardDefs.h"

// EXTERNAL INCLUDES
#include <vector>





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
        ChecksVector curChecks;
        ChecksVector oppChecks;

        COLOUR curCol;
        COLOUR oppCol;

        Piece* curKing;
        Piece* oppKing;

        Piece* whiteKing;
        Piece* blackKing;

        PieceVector whitePieces;
        PieceVector blackPieces;
        
        PieceVector curPieces;
        PieceVector oppPieces;

        int curForward;

        
        

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
        void changeState(BoardState* statePtr_);

        // MAIN METHODS
        void processState();
        void generateMovementRange(Piece* piece);
        void processMoveRange(Piece* piece, PositionQueue moveRange, MovementQueue& moveQueue);
 
        // CHECK-RELATED FUNCTIONS
        PieceChecks     pieceChecks(Piece* piece);
        PieceVector     pieceCheckedBy(Piece* piece, const ChecksVector& oppChecks);
        PieceVector     positionCheckedBy(BoardPosition pos, COLOUR oppCol);

        // REFINEMENTS
        PositionQueue   returnSafeMoves(Piece* piece);
        PositionQueue   returnSafeTakes(Piece* piece);
        void            restrictRevealedCheckMoves(Piece* piece);
        void            restrictKingMoves(COLOUR col);
        void            restrictToBlockingMoves(Piece* pieceToProtect, Piece* pieceToMove, Piece* targetPiece,
                                         PositionQueue* validMoves, PositionQueue* validTakes);
        void            removeInvalidCheckedMoves(PieceVector kingCheckedBy);

        // RETURNS
        MovementQueue returnMovementQueue(Piece* piece);
        MovementQueue returnMovementQueue(BoardPosition pos);
        ChecksVector   returnChecksVector(const PieceVector& pieceVector);

    protected:
        bool    validPiece(Piece* piece)       { return !(piece == NULL || piece->type() == NONE); }
        Piece*  pieceByPosition(BoardPosition pos);
};