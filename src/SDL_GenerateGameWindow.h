/* SDL_GENERATEWINDOW.H
/ Generates the game window, deals with all erorr checking etc., etc
*/

#pragma once

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include<SDL2/SDL.h>
#include<iostream>

// Standard window sizes
enum WINDOW_SIZES {WINDOW_HEIGHT = 640, WINDOW_WIDTH = 480};

class SDL_GenerateGameWindow
{
    protected:
        SDL_Window* window;
        SDL_Surface* windowSurface;
        
    public: 
        SDL_GenerateGameWindow() {

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

            windowSurface = SDL_GetWindowSurface(window);
        }

        ~SDL_GenerateGameWindow() {
            SDL_DestroyWindow(window);
        }
        
        SDL_Window* Get_Window() {return window;};
        SDL_Surface* Get_Surface() {return windowSurface;};
};