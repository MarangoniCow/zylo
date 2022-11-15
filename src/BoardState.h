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
    // Pieces, current and previous turn
    Piece*          current[8][8];
    PIECE_COLOUR    currentTurn;
    Move            m_lastMove;

    BoardState()
    {
        currentTurn = WHITE;
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                current[i][j] = NULL;
            };
        };
    };
    
    // Initialisation methods
    void initialiseBoard();

    // Updating methods
    void promotePiece(PIECE_ID ID, PIECE_TYPE newType);

    // Maintainence methods
    void resetBoard();
    void addPiece(Piece* newPiece);
    void addPiece(PIECE_TYPE type, PIECE_COLOUR col, int x, int y);
    void removePiece(Piece* pieceToDelete);
    void removePiece(PIECE_ID pieceToDelete);

    // Bool checks
    bool pieceExists(Piece* piece) const;
    bool pieceExists(Piece* piece, PIECE_COLOUR col) const;
    bool pieceExists(Piece* piece, PIECE_COLOUR col, PIECE_TYPE type) const;
    bool pieceExists(BoardPosition pos) const;
    bool pieceExists(BoardPosition pos, PIECE_COLOUR col) const;
    bool pieceExists(BoardPosition pos, PIECE_COLOUR col, PIECE_TYPE type) const;
    
    // Last move
    Move lastMove()                 { return m_lastMove;}
    void lastMove(Move lastMove)    { m_lastMove = lastMove;}


    // Returns
    Piece*      returnIDPtr(PIECE_ID ID);
    Piece *		returnPiece(BoardPosition pos) const;
    PieceQueue	returnPieceQueue() const;
    PieceQueue	returnPieceQueue(PIECE_COLOUR col) const;
    PieceQueue	returnPieceQueue(PIECE_COLOUR col, PIECE_TYPE type) const;

	void		getPieceQueue(PieceQueue &pieceQueue) const;
   
};

