// board.h
#pragma once

// External includes
#include <SDL2/SDL.h>
#include <vector>

// Internal includes
#include "SDL_Board.h"
#include "Piece.h"

struct State {
    Piece* boardState[8][8];
};

class Board {

    protected: 
        SDL_Board* gameBoard;
        State current_state;
        static State start_state;
    public:
        Board(SDL_Board* gameBoard_) : gameBoard(gameBoard_) {}; 
        void initialiseBoard();
        
        
};