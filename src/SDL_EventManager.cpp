/* SDL_EVENTMANAGER.CPP */

// INTERNAL DEPENDENCIES
#include "SDL_EventManager.h"
#include "Coords.h"
#include "Board.h"
#include "Piece.h"


// EXTERNAL DEPENDENCIES
#include <SDL2/SDL.h>
#include <iostream>
#include <queue>


void SDL_EventManager::RunGame() {

    // Generate board
    SDL_Board gameWindow;
    gameWindow.GenerateBoard();

    Board board(&gameWindow);
    board.initialiseBoard();
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

                // Fetch current board coordinates
                MouseToBoardCoords();

                // Check for previous board coordinates
                if(click_location_prev.validPosition()) {
                    std::cout << "MOVE PIECE \n" << std::endl; 
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
    std::cout << "MOUSE CLICK" << std::endl;
    std::cout << "Prev: " << click_location_prev.x << ", " << click_location_prev.y << std::endl;
    std::cout << "Curr: " << click_location_curr.x << ", " << click_location_curr.y << std::endl;
    
    // Check for previous click, if no previous click, save current click as prev
    if(!click_location_prev.validPosition())
        click_location_prev = click_location_curr;
    
    
    // Fetch screen coordinates and translate into board coordinates
    int SDL_x, SDL_y;
    SDL_GetMouseState(&SDL_x, &SDL_y);
    click_location_curr = SDL_Board::SDL_to_Coords(SDL_x, SDL_y);


    std::cout << "UPDATE" << std::endl;
    std::cout << "Prev: " << click_location_prev.x << ", " << click_location_prev.y << std::endl;
    std::cout << "Curr: " << click_location_curr.x << ", " << click_location_curr.y << std::endl;
    std::cout << "\n";
}
