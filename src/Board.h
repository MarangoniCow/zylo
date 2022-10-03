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
#include "SDL_Board.h"              // POSSIBLY NOT BEST PLACE FOR BOARD
#include "Piece.h"
#include "Coords.h"

// EXTERNAL INCLUDES
#include <SDL2/SDL.h>

/*      COMMENTS AND TO DO      
*   Not sure if Board really needs the game board. Would it not be better to output the boardstate?
*   Why should Board have to deal with renderState?
*/



class Board {

    protected: 
        SDL_Board* gameBoard;
        Piece* boardState[8][8];
        
    public:
        Board(SDL_Board* gameBoard_) : gameBoard(gameBoard_) {
            for(int i = 0; i < 8; i++) {
                for(int j = 0; j < 8; j++) {
                    boardState[i][j] = NULL; 
                };
            }
        }; 
        void initialiseBoard();
        void renderCurrentState();
        void movePiece(BoardPosition oldPos, BoardPosition newPos);
        bool validMove(BoardPosition oldPos, BoardPosition newPos);
        Piece* returnState(int i, int j);
        
        
};