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
#include "BoardPosition.h"

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
        // MEMBER VARIABLES
        BoardState state;
        std::queue<BoardPosition> validMoves;
        std::queue<BoardPosition> takeMoves;
        std::queue<BoardPosition> invalidMoves;

        // PRIVATE HELPER FUNCTIONS FOR MOVEMENT
        void movePiece(Piece* curPiece, BoardPosition newPos);
        void takePiece(Piece* curPiece, BoardPosition newPos);
        void removePiece(Piece* pieceToDelete);

    public:
        // CONSTRUCTORS
        Board() {};
        void initialiseBoard();
        
        // STATE-RELATED FUNCTIONS
        void addPieceToState(Piece* newPiece);
        void processClick(BoardPosition oldPos, BoardPosition newPos);
        
        // MOVEMENT-RELATED FUNCTIONS
        void generateMovementRange(BoardPosition oldPos);
        void processMoveQueue(std::queue<BoardPosition> moveQueue, BoardPosition curPiecePos);
        

        // RETURNS
        BoardState returnState() {return state;};
        std::queue<BoardPosition> returnValidMoves()    {return validMoves;};
        std::queue<BoardPosition> returnTakeMoves()     {return takeMoves;};
        std::queue<BoardPosition> returnInvalidMoves()  {return invalidMoves;};


       
        
        
};