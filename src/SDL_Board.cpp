/***********************************************************
 *                 SDL_BOARD IMPLEMENTATION
 ***********************************************************/



// INTERNAL INCLUDES
#include "SDL_Board.h"
#include "Coords.h"
#include "Board.h"

// EXTERNAL INCLUDES
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <queue>

// Constructor implementation
SDL_Board::SDL_Board()
{
    // Generate game window
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        std::cout << "Video initialisation error: " << SDL_GetError() << std::endl;
    else
    {
                window = SDL_CreateWindow("Zylo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        WINDOW_HEIGHT, WINDOW_WIDTH, SDL_WINDOW_SHOWN);
    }
            
    // Error checking 
    if(window == NULL)
        std::cout << "Window creation error: " << SDL_GetError() << std::endl;

    // Set SDL coordinates: Define the top left hand corner of the board
    int board_bottom_coord_x = BOARD_X; 
    int board_bottom_coord_y = 7*SQUARE_HEIGHT + BOARD_Y;
    
    // Loop over and finish
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            SDL_coordinates[i][j][0] = board_bottom_coord_x + i*SQUARE_WIDTH; 
            SDL_coordinates[i][j][1] = board_bottom_coord_y - j*SQUARE_HEIGHT; 
        }
    }

    // Initialise renderer
    windowRenderer = nullptr;

     // Point render target to board window
    windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Error checking
    if(windowRenderer == NULL)
        std::cout << "Error creating render target: " << SDL_GetError() << std::endl;
    
}
// Destuctor implementation
SDL_Board::~SDL_Board()
{
    SDL_DestroyWindow(window); 
    SDL_DestroyRenderer(windowRenderer); 

}

/****************************************************/
/*   PRIVATE METHODS: LOAD TO RENDERER TO SCREEN    */
/****************************************************/

void SDL_Board::loadBackground()
{
   
   // Draw border as rectangle
    SDL_Rect board_border = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    
    // Define borrder colours, fill and present. 
    SDL_SetRenderDrawColor(windowRenderer, BOARD_BORDER_R, BOARD_BORDER_G, BOARD_BORDER_B, 255);
    SDL_RenderFillRect(windowRenderer, &board_border);
    
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
                SDL_SetRenderDrawColor(windowRenderer, SQUARE_WHITE_R, SQUARE_WHITE_G, SQUARE_WHITE_B, 255);
            else
                SDL_SetRenderDrawColor(windowRenderer, SQUARE_BLACK_R, SQUARE_BLACK_G, SQUARE_BLACK_B, 255);

            SDL_RenderFillRect(windowRenderer, &board_square);
        }
    }
}

void SDL_Board::loadPiece(int x, int y, std::string filePath)
{


    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());

    if(surface == NULL)
        std::cout << "Error, could not load .bmp file" << std::endl;
    else
    {
        texture = SDL_CreateTextureFromSurface(windowRenderer, surface);

        if(texture == NULL)
            std::cout << "Error creating texture: " << SDL_GetError() << std::endl;
    }
    SDL_Rect targetSquare = returnSquare(x, y);

    //SDL_RenderClear(renderTarget);
    SDL_RenderCopy(windowRenderer, texture, NULL, &targetSquare);
    SDL_DestroyTexture(texture); 
}

void SDL_Board::loadSquare(int x, int y)
{
    SDL_Rect targetSquare = returnSquare(x, y);

    if((x + y)%2 == 1)
        SDL_SetRenderDrawColor(windowRenderer, SQUARE_WHITE_R, SQUARE_WHITE_G, SQUARE_WHITE_B, 255);
    else
        SDL_SetRenderDrawColor(windowRenderer, SQUARE_BLACK_R, SQUARE_BLACK_G, SQUARE_BLACK_B, 255);

    SDL_RenderFillRect(windowRenderer, &targetSquare);
}

void SDL_Board::loadBoard(BoardState state)
{
    clearBoard();
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            
            
            Piece* current = state.piecesCurr[i][j];
            if (current != NULL) {
                loadPiece(i, j, current->returnPath());
            }
        }
    }         
}
 
void SDL_Board::loadOverlay(int x, int y)
{
    // Set blend mode & load relevant square
    SDL_SetRenderDrawBlendMode(windowRenderer, SDL_BLENDMODE_BLEND);
    SDL_Rect targetSquare = returnSquare(x, y);

    // Set draw colour and fill rectangle
    SDL_SetRenderDrawColor(windowRenderer, SQUARE_GREEN_R, SQUARE_GREEN_G, SQUARE_GREEN_B, 80);
    SDL_RenderFillRect(windowRenderer, &targetSquare);

    // Restore blend mode
    SDL_SetRenderDrawBlendMode(windowRenderer, SDL_BLENDMODE_NONE);
    

}   
void SDL_Board::clearBoard()
{
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            loadSquare(i, j);
}

SDL_Rect SDL_Board::returnSquare(int x, int y)
{
    SDL_Rect targetSquare;
    targetSquare.x = SDL_coordinates[x][y][0];
    targetSquare.y = SDL_coordinates[x][y][1];
    targetSquare.w = SQUARE_WIDTH;
    targetSquare.h = SQUARE_HEIGHT;

    return targetSquare; 
}



/****************************************************/
/*          PUBLIC METHODS: PRESENT RENDERER        */
/****************************************************/

void SDL_Board::renderBackground()
{
    loadBackground();
    SDL_RenderPresent(windowRenderer);
}

void SDL_Board::renderBoard(BoardState state)
{
    SDL_RenderClear(windowRenderer);
    loadBackground();
    loadBoard(state);
    SDL_RenderPresent(windowRenderer);
}

void SDL_Board::renderBoardUpdate(BoardState state)
{
    // Compare differences between new and old state
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++)
        {
            if(state.piecesCurr[i][j] != state.piecesPrev[i][j]) {

                // Clear the current square
                loadSquare(i, j);

                // Check for a new piece and render if necessary
                Piece* currentPiece = state.piecesCurr[i][j];
                if (currentPiece != NULL)
                    loadPiece(i, j, state.piecesCurr[i][j]->returnPath());
            }
        }
    }
}

void SDL_Board::renderOverlay(std::queue<BoardPosition> moveQueue)
{

    // Iterate through movement queue and load overlays
    while(!moveQueue.empty())
    {
        BoardPosition targetSquare = moveQueue.front();
        moveQueue.pop();
        loadOverlay(targetSquare.x, targetSquare.y);
    }
    SDL_RenderPresent(windowRenderer);
}
/****************************************************/
/*      STATIC METHODS: Additional functionality    */
/***************************************************/

BoardPosition SDL_Board::SDL_to_Coords(int SDL_x, int SDL_y)
{
    BoardPosition pos;
    pos.x = (SDL_x - BOARD_X)/SQUARE_WIDTH;
    pos.y = 7 - (SDL_y - BOARD_Y)/SQUARE_HEIGHT;
    return pos;
}




