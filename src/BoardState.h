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

// EXTERNAL INCLUDES
#include <utility>

typedef std::queue<BoardPosition> PositionQueue;
typedef std::queue<Piece*> PieceQueue;


struct BoardState {
    // Pieces, current and previous turn
    Piece* current[8][8];
    Piece* previous[8][8];
    PIECE_COLOUR currentTurn;

    BoardState()
    {
        currentTurn = WHITE;
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                current[i][j] = NULL;
                previous[i][j] = NULL; 
            };
        };
    };
    
    // Initialisation methods
    void initialiseBoard();

    // Updating methods
    void promotePiece(PIECE_ID ID, PIECE_TYPE newType);
    void updatePrevious() {memcpy(previous, current, sizeof(current));};

    // Maintainence methods
    void clearBoard();
    void addPiece(Piece* newPiece);
    void removePiece(Piece* pieceToDelete);
    void removePiece(PIECE_ID pieceToDelete);

    // Bool checks
    bool pieceExists(Piece* piece) const;
    bool pieceExists(Piece* piece, PIECE_COLOUR col) const;
    bool pieceExists(Piece* piece, PIECE_COLOUR col, PIECE_TYPE type) const;
    bool pieceExists(BoardPosition pos) const;
    bool pieceExists(BoardPosition pos, PIECE_COLOUR col) const;
    bool pieceExists(BoardPosition pos, PIECE_COLOUR col, PIECE_TYPE type) const;
    
    // Returns
    Piece *		returnPiece(BoardPosition pos) const;
    PieceQueue	returnPieceQueue() const;
    PieceQueue	returnPieceQueue(PIECE_COLOUR col) const;
    PieceQueue	returnPieceQueue(PIECE_COLOUR col, PIECE_TYPE type) const;

	void		getPieceQueue(PieceQueue &pieceQueue) const;
   
};

