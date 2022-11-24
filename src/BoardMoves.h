/***********************************************************
 *                      BOARDMOVES_H
 * 
 * Complete description of all possible moves on the board.
 * Important private members:
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
        MovementQueue   movementState[8][8];
        PieceChecks     checksState[8][8];

        // Common memeber variables regularly accessed
        ChecksVector curChecks;
        ChecksVector oppChecks;

        COLOUR curTurn;
        COLOUR oppTurn;

        Piece* curKing;
        Piece* oppKing;

        PieceVector curPieces;
        PieceVector oppPieces;

        int curForward;


        // Private methods: Resets
        void            resetFlags                  ();
        void            resetMoves                  ();
        void            setTurnDependencies         ();

        // Private methods: Special takes/moves (castling)
        void            addPawnMoves                (Piece* piece, PositionQueue& validTake);
        void            addCastling                 (Piece* piece);
        void            verifyMate                  ();

        // Private methods: Generate movement ranges and checks state
        void            generateMovementRange       ();
        void            generateChecksState         ();

    public:
        // CONSTRUCTORS
        BoardMoves();
        BoardMoves(BoardState* statePtr_);

        // INITIALISATION & ADMIN
        void            changeState(BoardState* statePtr_);

        // MAIN METHODS
        void            processState                ();
        void            processMoveRange            (Piece* piece, PositionQueue moveRange, MovementQueue& moveQueue);
        
 
        // CHECK-RELATED FUNCTIONS
        void            pieceCheckedBy              (Piece* piece, const ChecksVector& oppChecks, PieceVector& checkedByVector); 
        PieceVector     positionCheckedBy           (BoardPosition pos, COLOUR oppCol);

        // REFINEMENTS
        PositionQueue   safeMoves                   (Piece* piece);
        PositionQueue   safeTakes                   (Piece* piece);
        void            removeRevealedCheckMoves  (Piece* piece);
        void            restrictKingMoves           (COLOUR col);
        void            restrictInvalidCheckedMoves (const PieceVector& kingCheckedBy);
        void            restrictToBlockingMoves     (Piece* pieceToProtect, Piece* pieceToMove, Piece* targetPiece,
                                                     PositionQueue* validMoves, PositionQueue* validTakes);

        // RETURNS
        MovementQueue   movementQueue               (Piece* piece);
        MovementQueue   movementQueue               (BoardPosition pos);

    protected:
        bool            validPiece(Piece* piece)                        { return !(piece == NULL || piece->type() == NONE);}
        bool            validPiece(Piece* piece, PIECE_TYPE type)       { return  (piece != NULL && piece->type() == type);}
        Piece*          pieceByPosition(BoardPosition pos);
};