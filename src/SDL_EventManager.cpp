/* SDL_EVENTMANAGER.CPP */

// INTERNAL DEPENDENCIES
#include "SDL_EventManager.h"
#include "Board.h"
#include "Piece.h"

// EXTERNAL DEPENDENCIES
#include <SDL2/SDL.h>
#include <iostream>


void SDL_EventManager::RunGame() {

    // Generate board
    SDL_Board gameWindow;
    gameWindow.GenerateBoard();

    
    Pawn pawn_one(WHITE);
    std::cout << pawn_one.Get_Colour() << std::endl;
    std::cout << pawn_one.Get_FilePath() << std::endl;

    
    for(int i = 0; i < 2; i++)
    {
        gameWindow.LoadTexture(i, 0, pawn_one.Get_FilePath());
        SDL_Delay(1000);
        gameWindow.ClearSquare(i, 0); 

    }
    
    
    
    // Run game    
    bool isRunning = true;
    SDL_Event ev;


    while(isRunning)
    {
        // Poll event checks for event
        // Processes events each frame
        while(SDL_PollEvent(&ev) != 0)
        {
            if (ev.type == SDL_QUIT)
                isRunning = false;            
        }
        
    }
    
    

    SDL_Quit(); 
};

