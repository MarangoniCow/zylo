#include "Board.h"
#include <iostream>
#include <SDL2/SDL.h>
#include "SDL_GenerateGameWindow.h"


void Board::method() {
       
    // Generate window using wrapper
    SDL_GenerateGameWindow gameWindow;

    // Fetch pointers to window and window surface
    SDL_Window* boardWindow = gameWindow.Get_Window();
    SDL_Surface* boardSurface = gameWindow.Get_Surface();

    SDL_Surface* white_pawn = nullptr;
    SDL_Surface* white_rook = nullptr;
    SDL_Surface* black_pawn = nullptr;

    SDL_Surface* curr_image = nullptr;

    white_pawn = gameWindow.OptimizedSurface("./res/white_pawn.bmp");
    white_rook = gameWindow.OptimizedSurface("./res/white_rook.bmp");
    black_pawn = gameWindow.OptimizedSurface("./res/black_pawn.bmp");

    curr_image = white_pawn; 
    
    // Load a test image
     //loadImage(boardSurface);

    
    
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
            else if (ev.type == SDL_KEYDOWN)
            {
                switch(ev.key.keysym.sym)
                {
                    case SDLK_1:
                        curr_image = white_pawn;
                        break;
                    case SDLK_2:
                        curr_image = white_rook;
                        break;
                    case SDLK_3:
                        curr_image = black_pawn;
                        break;
                }
            }
            else if (ev.type == SDL_MOUSEMOTION)
            {
                if(ev.button.x < 200)
                    curr_image = white_pawn;
                else if(ev.button.x < 400)
                    curr_image = white_rook;
                else
                    curr_image = black_pawn;                               
            }
            
            SDL_BlitSurface(curr_image, NULL, boardSurface, NULL);
            SDL_UpdateWindowSurface(boardWindow); // Have to update every time we make a change         
        }
    }


    SDL_FreeSurface(white_pawn);
    SDL_FreeSurface(white_rook);
    SDL_FreeSurface(black_pawn); 

    SDL_DestroyWindow(boardWindow);
    SDL_Quit();


};



void Board::loadImage(SDL_Surface *targetSurface)
{
    // Pointer to image surface 
    SDL_Surface *imageSurface = nullptr;
    
    // Load a bitmap image
    imageSurface = SDL_LoadBMP("./res/white_pawn.bmp");
    

    // Check for correct load
    if(imageSurface == NULL)
        std::cout << "Image loading error: " << SDL_GetError() << std::endl;
    else
    {
        // Unsure what the NULLs do at the moment
        SDL_BlitSurface(imageSurface, NULL, targetSurface, NULL);       
    }
}