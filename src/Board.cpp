#include "Board.h"
#include <iostream>
#include <SDL2/SDL.h>
#include "SDL_GenerateGameWindow.h"



void Board::method() {
       
    // Generate window using wrapper
    SDL_GenerateGameWindow generateWindow;

    // Fetch pointers to window and window surface
    SDL_Window* boardWindow = generateWindow.Get_Window();
    SDL_Surface* boardSurface = generateWindow.Get_Surface();
    
    // Load a test image
     loadImage(boardSurface);

    SDL_UpdateWindowSurface(boardWindow); // Have to update every time we make a change         
    
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
        SDL_UpdateWindowSurface(boardWindow);

    }



    SDL_Quit();


};
void Board::loadImage(SDL_Surface *targetSurface)
{
    // Pointer to image surface 
    SDL_Surface *imageSurface = nullptr;
    
    // Load a bitmap image
    imageSurface = SDL_LoadBMP("./res/water.bmp");

    // Check for correct load
    if(imageSurface == NULL)
        std::cout << "Image loading error: " << SDL_GetError() << std::endl;
    else
    {
        // Unsure what the NULLs do at the moment
        SDL_BlitSurface(imageSurface, NULL, targetSurface, NULL);       
    }
}