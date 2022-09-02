#include "Board.h"
#include <iostream>
#include <SDL2/SDL.h>
#include "SDL_GenerateGameWindow.h"

enum
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

    // Create board
    

    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            SDL_Rect board_square;
            board_square.x = x*SQUARE_WIDTH;
            board_square.y = y*SQUARE_HEIGHT;
            board_square.w = SQUARE_WIDTH;
            board_square.h = SQUARE_HEIGHT;

            if((x + y)%2 == 0)
                SDL_SetRenderDrawColor(renderTarget, SQUARE_WHITE_R, SQUARE_WHITE_G, SQUARE_WHITE_B, 255);
            else
                SDL_SetRenderDrawColor(renderTarget, SQUARE_BLACK_R, SQUARE_BLACK_G, SQUARE_BLACK_B, 255);

            SDL_RenderFillRect(renderTarget, &board_square);
            SDL_RenderPresent(renderTarget);
        }
    }


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