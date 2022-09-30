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
    board.renderCurrentState();

    SDL_Delay(1000);
    gameWindow.ClearBoard();
    SDL_Delay(1000);
    board.renderCurrentState();

    // Run game    
    bool isRunning = true;
    


    while(isRunning)
    {
        /* INITIALISE NECESSARY RUNNING VARIABLES */
        

        // Poll event checks for event and processes events each frame
        while(SDL_PollEvent(&ev_cur) != 0)
        {
            if (ev_cur.type == SDL_QUIT)
                isRunning = false;
            else if(ev_cur.type == SDL_MOUSEBUTTONDOWN) {

                // Save current board coordinates
                MouseToBoardCoords();

                // Check for previous board coordinates
                if (click_location_prev.x != -1 && click_location_prev.y != -1) {
                    board.movePiece(click_location_prev, click_location_curr);
                    click_location_prev.ResetPosition();
                }                
            }
        }
        
    }
    
    

    SDL_Quit(); 
};

void SDL_EventManager::MouseToBoardCoords()
{

        // Make sure we keep a track of old click location
    if(click_location_prev.x == -1 || click_location_prev.y == -1)
        click_location_prev = click_location_curr;
    
    // Fetch screen coordinates and translate into board coordinates
    int SDL_x, SDL_y;
    SDL_GetMouseState(&SDL_x, &SDL_y);
    click_location_curr = SDL_Board::SDL_to_Coords(SDL_x, SDL_y);
    std::cout << std::endl;


}
