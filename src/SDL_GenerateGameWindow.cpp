/* SDL_GENERATEGAMEWINDOW.CPP  */


// External dependencies
#include <SDL2/SDL.h>
#include <iostream>
#include <string>

// Internal dependencies
#include "SDL_GenerateGameWindow.h"

// Constructor implementation
SDL_GenerateGameWindow::SDL_GenerateGameWindow()
{

            // Generate window
            if(SDL_Init(SDL_INIT_VIDEO) < 0)
                std::cout << "Video initialisation error: " << SDL_GetError() << std::endl;
            else
            {
                window = SDL_CreateWindow("SDL_Board_Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        WINDOW_HEIGHT, WINDOW_WIDTH, SDL_WINDOW_SHOWN);
            }
            
            // Error checking
            if(window == NULL)
                std::cout << "Window creation error: " << SDL_GetError() << std::endl;

            
}

SDL_Surface* SDL_GenerateGameWindow::OptimizedSurface(std::string filePath)
{
    // Initialise image surface to null ptr, load surface from bitmap.
    SDL_Surface* optimizedSurface = nullptr;
    SDL_Surface* imageSurface = SDL_LoadBMP(filePath.c_str());
    SDL_Surface* windowSurface = Get_Surface(); 

    // Standard error checking
    if(imageSurface == NULL)
        std::cout << "ERROR: Could not load image surface" << std::endl;
    else
    {
        // Surface conversion, unsure of 0 flag
        optimizedSurface = SDL_ConvertSurface(imageSurface, windowSurface->format, 0);

        // Error checking
        if(optimizedSurface == NULL)
            std::cout << "Error in optimising surface" << std::endl;
    }

    SDL_FreeSurface(imageSurface);

    return optimizedSurface;
}