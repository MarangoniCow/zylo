/***********************************************************
 *              SDL_EVENTMANAGER IMPLEMENTATION
 ***********************************************************/

// INTERNAL DEPENDENCIES
#include "SDL_EventManager.h"
#include "SDL_Board.h"
#include "Coords.h"
#include "Board.h"
#include "Piece.h"


// EXTERNAL DEPENDENCIES
#include <SDL2/SDL.h>
#include <iostream>
#include <queue>


void SDL_EventManager::RunGame() {

    
    
    

    // Initialise board logic class
    Board board;
    board.initialiseBoard();

    // Initialise board class
    SDL_Board gameWindow;
    gameWindow.renderNewBoard(board.returnState());
    gameWindow.renderOverlay(1, 1);
    

    // Game bool
    bool isRunning = true;
    
    // Run the game!
    while(isRunning)
    {
        
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
                    board.movePiece(click_location_prev, click_location_curr);
                    gameWindow.renderNewBoard(board.returnState());

                    click_location_curr.ResetPosition();
                    click_location_prev.ResetPosition();
                }
            }
        }
        
    }
    
    

    SDL_Quit(); 
};

void SDL_EventManager::MouseToBoardCoords()
{
    BoardPosition tempPos;  
    // Fetch screen coordinates and translate into board coordinates
    int SDL_x, SDL_y;
    SDL_GetMouseState(&SDL_x, &SDL_y);
    tempPos = SDL_Board::SDL_to_Coords(SDL_x, SDL_y);

    // If a valid location...
    if(tempPos.validPosition())     
    {
        
        // Check for a current click, if no previous click, save current click as prev
        if(click_location_curr.validPosition())
        {
            
            click_location_prev = click_location_curr;
        }
        click_location_curr = tempPos;
    }

    // Can have an else here for any clicks outside of the board coordinates
    
};
