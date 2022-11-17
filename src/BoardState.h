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

// EXTERNAL INCLUDES
#include <utility>

typedef std::queue<BoardPosition> PositionQueue;
typedef std::queue<Piece*> PieceQueue;


struct BoardState {

    public: 
        // Pieces, current and previous turn
        Piece           current[8][8];
        PIECE_COLOUR    turn;
        Move            lastMove;

    public:
        BoardState()
        {
            turn = WHITE;
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
        void addPiece       (PIECE_TYPE type, PIECE_COLOUR col, BoardPosition pos);
        void removePiece    (BoardPosition pos);
        void movePiece      (BoardPosition oldPos, BoardPosition newPos);

        // Boolean checks
        bool pieceExists    (BoardPosition pos) const;
        bool pieceExists    (BoardPosition pos, PIECE_COLOUR col) const;
        bool pieceExists    (BoardPosition pos, PIECE_COLOUR col, PIECE_TYPE type) const;

        // Piece queues
        void pieceQueue     (PositionQueue& queue) const;
        void pieceQueue     (PositionQueue& queue, PIECE_COLOUR col) const;
        void pieceQueue     (PositionQueue& queue, PIECE_COLOUR col, PIECE_TYPE type) const;

};

