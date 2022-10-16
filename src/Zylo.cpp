/*                       ZYLO                           
 A small chess program developed on C++ using SDL2.
 */


// EXTERNAL DEPENDENCIES                                
#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>


// INTERNAL DEPENDENCIES
#include "SDL_EventManager.h"
#include "SDL_Board.h"
#include "Board.h"



// TEMP
#include "Piece.h"

int main () {

    std::cout << "Welcome to Zylo!" << std::endl;

    // Initalise board and board logic
    Board board;
    board.initialiseBoard();

    // Initialise a game window and render the current state
    SDL_Board gameWindow;
    gameWindow.renderBoard(board.returnState());

    // Gamne boolean
    
    

    SDL_EventManager zylo(&gameWindow, &board);
    zylo.RunGame();
    



    return 0;
}