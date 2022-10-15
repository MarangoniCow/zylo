/***********************************************************
 *              SDL_EVENTMANAGER IMPLEMENTATION
 ***********************************************************/

// INTERNAL DEPENDENCIES
#include "SDL_EventManager.h"
#include "SDL_Board.h"
#include "BoardPosition.h"
#include "Board.h"
#include "Piece.h"


// EXTERNAL DEPENDENCIES
#include <SDL2/SDL.h>
#include <iostream>
#include <queue>


void SDL_EventManager::RunGame() {

    /* CURRENT STATE REPORT 
    * Long term plan: Move most of the initialisation of the board logic, etc etc to zylo.cpp
    * and re-arrange the EventManager to do that: manage the events.
    * For the moment, everything is just initialised here to get things running.
    */


    // Initialise board logic class and initialise the board with a new state
    Board board;
    // Initialise the board with pieces
    board.initialiseBoard();
   
    // Initialise a game window and render the current state
    SDL_Board gameWindow;
    gameWindow.renderBoard(board.returnState());
    
    // Game boolean
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
                if(prevBoardCoord.validPosition()) {
                    
                    // Process a click and update the board
                    board.processClick(prevBoardCoord, curBoardCoord);
                    gameWindow.renderBoard(board.returnState());
                    
                    // Reset the click location
                    curBoardCoord.ResetPosition();
                    prevBoardCoord.ResetPosition();
                }
                else
                {
                    // Generate the mvoement range, which is stored in board, and pass those movement queues to render overlay
                    MovementQueue moveQueue = board.generateMovementRange(curBoardCoord);
                    gameWindow.renderOverlay(moveQueue.validMoves, moveQueue.validTakes, moveQueue.invalidMoves);
                }    
            }
        }
    }
    SDL_Quit(); 
};

void SDL_EventManager::MouseToBoardCoords()
{
    BoardPosition clickPos;  
    // Fetch screen coordinates and translate into board coordinates
    int SDL_x, SDL_y;
    SDL_GetMouseState(&SDL_x, &SDL_y);
    clickPos = SDL_Board::SDL_to_Coords(SDL_x, SDL_y);

    // If a valid board location...
    if(clickPos.validPosition())     
    {
        
        // Check for a current click, if no previous click, save current click as prev
        if(curBoardCoord.validPosition())
        {
            
            prevBoardCoord = curBoardCoord;
        }
        curBoardCoord = clickPos;
    }
    else
    {
        // Nothing yet! 
    }    
};
