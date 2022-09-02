#include "Board.h"
#include <iostream>
#include <SDL2/SDL.h>
#include "SDL_GenerateGameWindow.h"

SDL_Texture* LoadTexture(std::string filePath, SDL_Renderer* renderTarget)
{
    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());

    if(surface == NULL)
        std::cout << "Error, could not load .bmp file" << std::endl;
    else
    {
        texture = SDL_CreateTextureFromSurface(renderTarget, surface);

        if(texture == NULL)
            std::cout << "Error creating texture: " << SDL_GetError() << std::endl;
    }

    return texture;
}



void Board::method()
{
       
    // Generate window using wrapper
    SDL_GenerateGameWindow gameWindow;

    // Fetch pointers to window and window surface
    SDL_Window* boardWindow = gameWindow.Get_Window();
    SDL_Renderer* renderTarget = nullptr;
    SDL_Texture* currentTexture = nullptr;

    renderTarget = SDL_CreateRenderer(boardWindow, -1, SDL_RENDERER_ACCELERATED);

    if(renderTarget == NULL)
        std::cout << "Error creating render target: " << SDL_GetError() << std::endl;
    


    SDL_Texture* white_pawn = nullptr;
    SDL_Texture* white_rook = nullptr;
    SDL_Texture* white_queen = nullptr;

    white_pawn = LoadTexture("./res/white_pawn.bmp", renderTarget);
    white_rook = LoadTexture("./res/white_rook.bmp", renderTarget);
    white_queen = LoadTexture("./res/white_queen.bmp", renderTarget);

    

    
    
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
                        currentTexture = white_pawn;
                        break;
                    case SDLK_2:
                        currentTexture = white_rook;
                        break;
                    case SDLK_3:
                        currentTexture = white_queen;
                        break;
                }
            }
            else if (ev.type == SDL_MOUSEMOTION)
            {
                if(ev.button.x < 200)
                    currentTexture = white_pawn;
                else if(ev.button.x < 400)
                    currentTexture = white_rook;
                else
                    currentTexture = white_queen;                               
            }

            
            
            SDL_RenderClear(renderTarget);
            SDL_RenderCopy(renderTarget, currentTexture, NULL, NULL);
            SDL_RenderPresent(renderTarget); 
            
        }
    }


    SDL_DestroyTexture(white_pawn);
    SDL_DestroyTexture(white_rook);
    SDL_DestroyTexture(white_queen); 

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