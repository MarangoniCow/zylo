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

    // Piece::OutputPieceList();
    
    // Run game    
    bool isRunning = true;
    SDL_Event ev;

    // Initialise screen coordinates for later use
    int SDL_x, SDL_y;
    int BRD_x, BRD_y;
    BoardPosition curPos;
    BoardPosition oldPos;

    oldPos.x = -1;
    oldPos.y = -1;
    
    
    //SDL_Delay(1000);
    //board.movePiece(0, 1, 0, 2);


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
                SDL_Board::SDL_to_Coords(&SDL_x, &SDL_y, &BRD_x, &BRD_y);
                
                curPos.x = BRD_x;
                curPos.y = BRD_y;
                
                
                
                
                if(oldPos.x == -1 || oldPos.y == -1) {
                    oldPos = curPos; 
                }
                else {
                    board.movePiece(oldPos, curPos);
                    oldPos.x = -1;
                    oldPos.y = -1; 
                }
                

                
            }
        }
        
    }
    
    

    SDL_Quit(); 
};

void SDL_EventManager::MouseToBoardCoords(int* x, int* y)
{

   std::cout << "x: " << *x << " y: " << *y << std::endl;


}