// board.h
#pragma once

// External includes
#include <SDL2/SDL.h>
#include <vector>

// Internal includes
#include "SDL_Board.h"
#include "Piece.h"
#include "Coords.h"



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
        
        
};