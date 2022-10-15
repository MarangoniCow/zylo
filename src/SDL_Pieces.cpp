/***********************************************************
 *              SDL_PIECES IMPLEMENETATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "SDL_Pieces.h"
#include "Piece.h"

// EXTERNAL INCLUDES
#include <SDL2/SDL.h>
#include <string>
#include <iostream>

SDL_Texture* SDL_Pieces::pieceTextureArray[6][2];

SDL_Pieces::~SDL_Pieces()
{
    // Loop over array and destroy all the textures.
   for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            SDL_DestroyTexture(pieceTextureArray[i][j]);
        }
    }    
};

void SDL_Pieces::initialiseGraphicsObjs(SDL_Renderer* renderer_)
{
    renderer = renderer_;
     // Load all pieces into memory
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            // Cast into the right type
            PIECE_TYPE type = static_cast<PIECE_TYPE>(i);
            PIECE_COLOUR col = static_cast<PIECE_COLOUR>(j);
            loadTexture(type, col);
        }
    }
}

void SDL_Pieces::loadTexture(PIECE_TYPE type, PIECE_COLOUR col)
{
    // Initialise texture and surface
    SDL_Texture* pieceTexture = nullptr;
    std::string bmpPath = returnBMPPath(type, col);
    SDL_Surface* surface = SDL_LoadBMP(bmpPath.c_str());

    // Error checking etc., etc
    if(surface == NULL)
        std::cout << "Error, could not load .bmp file" << std::endl;
    else
    {
        pieceTexture = SDL_CreateTextureFromSurface(renderer, surface);

        if(pieceTexture == NULL)
            std::cout << "Error creating texture: " << SDL_GetError() << std::endl;
    };

    // Place texture into memory
    pieceTextureArray[type][col] = pieceTexture;
}


std::string SDL_Pieces::returnBMPPath(PIECE_TYPE type, PIECE_COLOUR col)
{
    std::string colour = (col == WHITE) ? "white" : "black";
    std::string relPath;

    switch(type)
    {
        case PAWN: {
            relPath = "./res/pawn_";
            break;
        }
        case ROOK: {
            relPath = "./res/rook_";
            break;
        }
        case KNIGHT: {
            relPath = "./res/knight_";
            break;
        }
        case BISHOP: {
            relPath = "./res/bishop_";
            break;
        }
        case QUEEN: {
            relPath = "./res/queen_";
            break;
        }
        case KING: {
            relPath = "./res/king_";
            break;

        }
        default: {};
    }
    return relPath + colour + ".bmp";
}
