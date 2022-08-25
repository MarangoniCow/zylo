#include "board.h"
#include <iostream>
#include <SDL2/SDL.h>

void Board::method() {
    


    // The window we'll be pointing to.
    SDL_Window* window = nullptr;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        std::cout << "Video initialisation error: " << SDL_GetError() << std::endl;
    else
    {
        window = SDL_CreateWindow("SDL_Board_Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN); // A bunch of commands I should loo up later.
        if(window == NULL)
            std::cout << "Window creation error: " << SDL_GetError() << std::endl;
        else
        {
            SDL_UpdateWindowSurface(window); // Have to update every time
            SDL_Delay(2000); // Measured in ms
        }
    }

    SDL_DestroyWindow(window); // Created something dynamically (?) so should delete it
    SDL_Quit();


};