// board.h
#pragma once

// External includes
#include <SDL2/SDL.h>
#include <vector>

// Internal includes
#include "SDL_Board.h"
#include "Piece.h"



class Board {

    protected: 
        SDL_Board* gameBoard;
        Piece* boardState[8][8];
        
    public:
        Board(SDL_Board* gameBoard_) : gameBoard(gameBoard_) {}; 
        void initialiseBoard();
        void renderCurrentState(); 
        
        
};