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
#include "GameplayManager.h"



// TEMP
#include "Piece.h"


typedef std::pair<bool, PIECE_ID> PLAY_FLAG;
typedef std::pair<PIECE_ID, PieceQueue> PieceChecks;
typedef std::queue<PieceChecks> ChecksQueue;
typedef std::pair<bool, PIECE_ID> PLAY_FLAG;
typedef std::pair<PIECE_ID, PieceQueue> PieceChecks;



int main () {

    std::cout << "Welcome to Zylo!" << std::endl;

    BoardPosition pos(6, 1);
    while(pos.validPosition())
        pos = pos.increment(RIGHT);

    // Initalise board and board logic
    Board board;
    board.newGame();

    // Initialise gameplay manager
    GameplayManager manager(&board);

    // Initialise a game window and render the current state
    SDL_Board gameWindow;
    gameWindow.renderBoard(board.returnState());

    // Gamne boolean
    
    

    SDL_EventManager zylo(&gameWindow, &manager);
    zylo.RunGame();
    



    return 0;
}