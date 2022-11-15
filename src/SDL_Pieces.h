/***********************************************************
 *                      SDL_PIECES.H
 * 
 * Class to encapsulate pieces as SDL textures for rendering
 * purposes. 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "Piece.h"

// EXTERNAL INCLUDES
#include <SDL2/SDL.h>
#include <string>
#include <vector>



class SDL_Pieces
{
    private:
        static SDL_Texture* pieceTextureArray[7][2];
        SDL_Renderer* renderer;

    public:
        // CONSTRUCTORS
        SDL_Pieces() {};
        ~SDL_Pieces();

        // PUBLIC FUNCTIONAL METHODS
        void initialiseGraphicsObjs(SDL_Renderer* renderer_);
        void loadTexture(PIECE_TYPE type, PIECE_COLOUR col);
        
        // RETURNS
        SDL_Texture* returnTexture(PIECE_TYPE type, PIECE_COLOUR col) {return pieceTextureArray[type][col];};
        static std::string returnBMPPath(PIECE_TYPE type_, PIECE_COLOUR col_);
};