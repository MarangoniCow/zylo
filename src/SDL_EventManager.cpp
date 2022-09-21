/* SDL_EVENTMANAGER.CPP */

// INTERNAL DEPENDENCIES
#include "SDL_EventManager.h"
#include "Coords.h"
#include "Board.h"
#include "Piece.h"

// EXTERNAL DEPENDENCIES
#include <SDL2/SDL.h>
#include <iostream>


void SDL_EventManager::RunGame() {

    // Generate board
    SDL_Board gameWindow;
    gameWindow.GenerateBoard();

    Board board(&gameWindow);
    board.initialiseBoard();

    Piece::OutputPieceList();



        
    
    
    // Run game    
    bool isRunning = true;
    SDL_Event ev;

    int SDL_x, SDL_y;
    int BRD_x, BRD_y;


    while(isRunning)
    {
        // Poll event checks for event
        // Processes events each frame
        while(SDL_PollEvent(&ev) != 0)
        {
            if (ev.type == SDL_QUIT)
                isRunning = false;
            else if(ev.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&SDL_x, &SDL_y);
                Coords::SDLCoordsToBoard(&SDL_x, &SDL_y, &BRD_x, &BRD_y);
                MouseToBoardCoords(&BRD_x, &BRD_y);
            }
        }
        
    }
    
    

    SDL_Quit(); 
};

void SDL_EventManager::MouseToBoardCoords(int* x, int* y)
{

   std::cout << "x: " << *x << " y: " << *y << std::endl;


}