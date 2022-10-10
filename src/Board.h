/***********************************************************
 *                      BOARD.h
 * 
 *  Handles all board logic:
 *      - Keeping track of board state
 *      - Moving pieces in board state
 *      - Telling SDL_Board to update screen #### POSSIBLY NOT THE BEST PLACE FOR THIS
 * 
 * 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "Piece.h"
#include "Coords.h"

// EXTERNAL INCLUDES
#include <SDL2/SDL.h>


struct BoardState {
    Piece* piecesCurr[8][8];
    Piece* piecesPrev[8][8];

    BoardState() {
    for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                piecesCurr[i][j] = NULL;
                piecesPrev[i][j] = NULL; 
            };
        };
    };
};

class Board {

    protected: 
        BoardState state;
        
    public:
        Board() {};
        void initialiseBoard();
        void movePiece(BoardPosition oldPos, BoardPosition newPos);
        void addPieceToState(Piece* newPiece);
        bool validMove(BoardPosition oldPos, BoardPosition newPos);
        std::queue<BoardPosition> generateValidMoves(BoardPosition oldPos);
        BoardState returnState() {return state;};
        
        
};