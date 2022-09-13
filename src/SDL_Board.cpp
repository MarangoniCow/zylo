/* SDL_GENERATEGAMEWINDOW.CPP  */


// External dependencies
#include <SDL2/SDL.h>
#include <iostream>
#include <string>

// Internal dependencies
#include "SDL_Board.h"




// Constructor implementation
SDL_Board::SDL_Board()
{
    // Generate game window
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
// Destuctor implementation
SDL_Board::~SDL_Board()
{
    SDL_DestroyWindow(window); 
}



// Generate game board and render
void SDL_Board::GenerateBoard()
{
    // Initialise render target
    SDL_Renderer* renderTarget = nullptr;

    // Point to board window
    renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Error checking
    if(renderTarget == NULL)
        std::cout << "Error creating render target: " << SDL_GetError() << std::endl;

    // Generate board
    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            SDL_Rect board_square;
            board_square.x = BOARD_X + x*SQUARE_WIDTH;
            board_square.y = BOARD_Y + y*SQUARE_HEIGHT;
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

    SDL_DestroyRenderer(renderTarget);
}



// Method for returning an optimized surface (OUTDATED: USING TEXTURES; REMOVE IN FUTURE RELEASE)
SDL_Surface* SDL_Board::OptimizedSurface(std::string filePath)
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

// REDUNDANT CODE: TO BE REMOVE IN FUTURE RELEASE
SDL_Texture* SDL_Board::LoadTexture(std::string filePath, SDL_Renderer* renderTarget)
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

// REDUNDANT CODE: TO BE REMOVE IN FUTURE RELEASE
void SDL_Board::loadImage(SDL_Surface *targetSurface)
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