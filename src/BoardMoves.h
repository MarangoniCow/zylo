/***********************************************************
 *                      BOARDMOVES_H
 * 
 * Complete description of all possible moves on the board.
 * Important private members:
 *  - movements.state: accessed like BoardState, an [8][8] array of moves
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

class BoardMoves {

    protected:

        // Member variables
        BoardState* statePtr;

        PLAY_FLAG kingCheck;
        PLAY_FLAG kingCheckmate;

        // Movement and checks state
		MovementState   m_movements;
        ChecksState     m_checks;

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

        // Private methods: Generate movement ranges and checks state
        void            generateMovementRange       ();
        void            generateChecksState         ();

        // Private methods: Special takes/moves (castling)
        void            addPawnMoves                (Piece* piece, PositionVector& validTake);
        void            addCastling                 (Piece* piece);
        void            verifyMate                  ();

    public:
        // CONSTRUCTORS
        BoardMoves();
        BoardMoves(BoardState* statePtr_);

        // INITIALISATION & ADMIN
        void            changeState(BoardState* statePtr_);

        // MAIN METHODS
        void            processState                ();
        void            processMoveRange            (Piece* piece, PositionQueue moveRange, Movements& moveQueue);
 
        // CHECK-RELATED FUNCTIONS
        void            pieceCheckedBy              (Piece* piece, const ChecksVector& oppChecks, PieceVector& checkedByVector); 
        PieceVector     positionCheckedBy           (BoardPosition pos, COLOUR oppCol);

        // REFINEMENTS
        PositionVector  safeMoves                   (Piece* piece);
        PositionVector  safeTakes                   (Piece* piece);
        void            removeRevealedCheckMoves    (Piece* piece);
        void            restrictKingMoves           (COLOUR col);
        void            restrictInvalidCheckedMoves (const PieceVector& kingCheckedBy);
        void            restrictToBlockingMoves     (Piece* pieceToProtect, Piece* pieceToMove, Piece* targetPiece,
                                                     PositionVector& validMoves, PositionVector& validTakes);

        // GETTERS
		const PieceVector&      getCurrentPieces () const   { return curPieces; }
        const PieceVector&      getOpposingPieces() const   { return curPieces; }

        const ChecksState&      getChecksState   () const   { return m_checks; }
        const MovementState&    getMovementState () const   { return m_movements; }

        const Movements&        getMoves            (const BoardPosition pos) const { return m_movements.state[pos.x][pos.y]; }
		

    protected:
        bool            validPiece                  (Piece* piece)                        { return !(piece == NULL || piece->type() == NONE);}
        bool            validPiece                  (Piece* piece, PIECE_TYPE type)       { return  (piece != NULL && piece->type() == type);}
        Piece*          pieceByPosition             (BoardPosition pos);
};