/* SDL_EVENTMANAGER.CPP */

// INTERNAL DEPENDENCIES
#include "SDL_EventManager.h"
#include "Board.h"

// EXTERNAL DEPENDENCIES
#include <SDL2/SDL.h>
#include <iostream>


void SDL_EventManager::RunGame() {

    // Generate board
    SDL_Board gameWindow;
    gameWindow.GenerateBoard();

    for(int i = 0; i < 8; i++)
    {
        gameWindow.LoadTexture(i, 0);
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

