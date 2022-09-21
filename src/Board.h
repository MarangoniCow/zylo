// board.h
#pragma once

// External includes
#include <SDL2/SDL.h>

// Internal includes
#include "SDL_Board.h"

class Board {

    protected: 
        SDL_Board* gameBoard;
        char boardCoordiantes[8][8];
        //static char Board_coordinates[8][8];
        
        

    public:
        Board(SDL_Board* gameBoard_) : gameBoard(gameBoard_) {}; 
        void startGame();
        
};