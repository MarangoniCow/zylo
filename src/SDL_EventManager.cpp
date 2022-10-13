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
   
    SDL_Board gameWindow;
    gameWindow.renderBoard(board.returnState());
    
    

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
                    gameWindow.renderBoard(board.returnState());

                    click_location_curr.ResetPosition();
                    click_location_prev.ResetPosition();
                }
                else
                {
                    board.generateMovementRange(click_location_curr);

                    std::queue<BoardPosition> takeTemp = board.returnTakeMoves();
                    std::queue<BoardPosition> validTemp = board.returnValidMoves();
                    std::queue<BoardPosition> invalidTemp = board.returnInvalidMoves();
                    gameWindow.renderOverlay(validTemp, takeTemp, invalidTemp);

                    std::cout << "TAKE MOVES \n" << std::endl;
                    while(!takeTemp.empty())
                    {
                    std::cout << takeTemp.front().x << ", " << takeTemp.front().y << std::endl;
                    takeTemp.pop();
                    }
                    std::cout << "VALID MOVES \n" << std::endl;
                    while(!validTemp.empty())
                    {
                    std::cout << validTemp.front().x << ", " << validTemp.front().y << std::endl;
                    validTemp.pop();
                    }
                    std::cout << "INVALID MOVES \n" << std::endl;
                    while(!invalidTemp.empty())
                    {
                    std::cout << invalidTemp.front().x << ", " << invalidTemp.front().y << std::endl;
                    invalidTemp.pop();
                    }

                    std::cout << "\n " << std::endl;


                    
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
