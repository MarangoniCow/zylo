/***********************************************************
 *                      BOARDSTATE.h
 * 
 * A complete description of a current board state, including:
 *  - Current piece positions
 *  - Prerevious piece positions
 *  - List of all valid moves for current pieces
 *  - List of all checks against the current colour
 *  - Flags indicating promotions, checks & checkmate
 *  - The current turn
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "Piece.h"
#include "BoardPosition.h"

// EXTERNAL INCLUDES
#include <utility>


typedef std::queue<BoardPosition> PositionQueue;
typedef std::queue<PIECE_ID> IDQueue;
typedef std::pair<PIECE_ID, IDQueue> PieceChecks;
typedef std::queue<PieceChecks> ChecksQueue;
typedef std::pair<bool, PIECE_ID> PLAY_FLAG;

struct BOARD_FLAGS
{
    PLAY_FLAG kingCheck;
    PLAY_FLAG kingCheckmate;
    PLAY_FLAG pawnPromotion;

    BOARD_FLAGS()
    {
        RESET_FLAGS();
    }
    void RESET_FLAGS()
    {
        kingCheck.first = 0;
        kingCheck.second = UINT16_MAX;
        kingCheckmate.first = 0;
        kingCheckmate.second = UINT16_MAX;
        pawnPromotion.first = 0;
        pawnPromotion.second = UINT16_MAX;
    }
};

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


struct BoardState {
    Piece* piecesCurr[8][8];
    Piece* piecesPrev[8][8];
    MovementQueue* piecePositionQueue[8][8];
    ChecksQueue currentChecks;
    BOARD_FLAGS boardFlags;
    PIECE_COLOUR currentTurn;

    BoardState()
    {
        currentTurn = WHITE;
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                piecesCurr[i][j] = NULL;
                piecesPrev[i][j] = NULL; 
            };
        };
    };
    
    // Initialisation methods
    void initialiseBoard();

    // Maintainence methods
    void clearBoard();
    void clearChecksQueue();
    
    // Updating methods
    void updatePiece(PIECE_ID ID, PIECE_TYPE newType);
    void addPiece(Piece* newPiece);
    void removePiece(Piece* pieceToDelete);
    void removePiece(PIECE_ID pieceToDelete);
    
};

