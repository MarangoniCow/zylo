/* SDL_EVENTMANAGER.CPP */

// INTERNAL DEPENDENCIES
#include "SDL_EventManager.h"

// EXTERNAL DEPENDENCIES
#include <SDL2/SDL.h>
#include <iostream>


void SDL_EventManager::RunGame() {

    // Generate board
    SDL_Board gameWindow;
    gameWindow.GenerateBoard();
    


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