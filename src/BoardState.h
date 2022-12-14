/***********************************************************
 *                      BOARDSTATE.h
 * 
 * Complete positional description of pieces on the board:
 *      - Handles all updates to positional changes
 *      - Interface with Pieces class for adding/removing/updaing pieces
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "Piece.h"
#include "BoardPosition.h"
#include "BoardDefs.h"



struct BoardState {

    protected:
        COLOUR    m_turn;
        Move      m_lastMove;

    public:
        Piece     current[8][8];

    public:
        BoardState()
        {
            m_turn = WHITE;
            for(int i = 0; i < 8; i++) {
                for(int j = 0; j < 8; j++) {
                    current[i][j] = Piece();
                };
            };
        };

        
        // Initialisation methods
        void initialiseBoard();
        void resetBoard     ();

        // Updating methods
        void promotePiece   (BoardPosition pos, PIECE_TYPE newType);

        // Maintainence methods
        void addPiece       (PIECE_TYPE type, COLOUR col, BoardPosition pos);
        void removePiece    (BoardPosition pos);
        void movePiece      (Piece &piece, const BoardPosition &newPos);
        void movePiece      (Move move);

        // Boolean checks
        bool pieceExists    (BoardPosition pos) const;
        bool pieceExists    (BoardPosition pos, COLOUR col) const;
        bool pieceExists    (BoardPosition pos, COLOUR col, PIECE_TYPE type) const;

        // Queries
        void                turn     (const COLOUR col)  { m_turn = col;}
        COLOUR              turn     () const            { return m_turn; }

        void                lastMove (const Move& move)  { m_lastMove = move; }
        Move                lastMove () const            { return m_lastMove; }


};

