/* SDL_BOARD.H
/ Generates the game window, deals with all erorr checking etc., etc
*/

#pragma once

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
// Standard window sizes
enum WINDOW_SIZES
{
    WINDOW_HEIGHT = 640,
    WINDOW_WIDTH = 640
};
enum BOARD_VALUES
{   
    SQUARE_WHITE_R = 252,
    SQUARE_WHITE_G = 245,
    SQUARE_WHITE_B = 232,
    SQUARE_BLACK_R = 117,
    SQUARE_BLACK_G = 79,
    SQUARE_BLACK_B = 18,
    SQUARE_WIDTH = 80,
    SQUARE_HEIGHT = 80
};

class SDL_Board
{
    protected:
        SDL_Window* window;
        SDL_Surface* windowSurface;
        
    public: 
        SDL_Board();                    // Constructor
        ~SDL_Board();                   // Destructor

        void GenerateBoard();
        SDL_Window* Get_Window() {return window;};              // Return window
        SDL_Surface* Get_Surface() {return SDL_GetWindowSurface(window);};     // Return window surface
        SDL_Surface* OptimizedSurface(std::string filePath);    // Convert a bitmap to the same surface as the desired window. 
        SDL_Texture* LoadTexture(std::string filePath, SDL_Renderer* renderTarget);
        void loadImage(SDL_Surface *targetSurface);
};