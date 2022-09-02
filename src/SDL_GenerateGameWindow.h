/* SDL_GENERATEWINDOW.H
/ Generates the game window, deals with all erorr checking etc., etc
*/

#pragma once

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
// Standard window sizes
enum WINDOW_SIZES {WINDOW_HEIGHT = 640, WINDOW_WIDTH = 640};

class SDL_GenerateGameWindow
{
    protected:
        SDL_Window* window;
        SDL_Surface* windowSurface;
        
    public: 
        SDL_GenerateGameWindow();                               // Constructor
        ~SDL_GenerateGameWindow() {                             // Destructor
            SDL_DestroyWindow(window);
        }
        SDL_Window* Get_Window() {return window;};              // Return window
        SDL_Surface* Get_Surface() {return SDL_GetWindowSurface(window);};     // Return window surface
        SDL_Surface* OptimizedSurface(std::string filePath);    // Convert a bitmap to the same surface as the desired window. 
};