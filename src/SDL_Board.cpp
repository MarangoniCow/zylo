/***********************************************************
 *                 SDL_BOARD IMPLEMENTATION
 ***********************************************************/



// INTERNAL INCLUDES
#include "SDL_Board.h"
#include "SDL_Pieces.h"
#include "BoardPosition.h"
#include "Board.h"

// EXTERNAL INCLUDES
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <queue>

/****************************************************/
/*                  CONSTRUCTORS                    */
/****************************************************/
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

    // Set coordinates with white at the bottom
    setOrientation(BLACK);


    // Initialise renderer
    windowRenderer = nullptr;

     // Point render target to board window
    windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Error checking
    if(windowRenderer == NULL)
        std::cout << "Error creating render target: " << SDL_GetError() << std::endl;

    pieceGraphics.initialiseGraphicsObjs(windowRenderer);
    
    
}
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
    for(int y = 0; y < 8; y++) {
        for(int x = 0; x < 8; x++) {
           loadSquare(x, y);
        }
    }
}

void SDL_Board::loadPiece(int x, int y, PIECE_TYPE type, COLOUR col)
{
    // Load texture from graphics holder    
    SDL_Texture* pieceTexture = pieceGraphics.returnTexture(type, col);

    // Load the appropriate square and copy the piece texture to that target. Clean-up.
    SDL_Rect targetSquare = returnSDLSquare(x, y);
    SDL_RenderCopy(windowRenderer, pieceTexture, NULL, &targetSquare);
    
}

void SDL_Board::loadSquare(int x, int y)
{
    SDL_Rect targetSquare = returnSDLSquare(x, y);

    // Set the colour, opacity, and send to renderer.
    if((x + y)%2 == 1)
        SDL_SetRenderDrawColor(windowRenderer, SQUARE_WHITE_R, SQUARE_WHITE_G, SQUARE_WHITE_B, 255);
    else
        SDL_SetRenderDrawColor(windowRenderer, SQUARE_BLACK_R, SQUARE_BLACK_G, SQUARE_BLACK_B, 255);

    SDL_RenderFillRect(windowRenderer, &targetSquare);
}

void SDL_Board::loadState(BoardState state)
{
    clearBoard();
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            
            
            Piece* current = &state.current[i][j];
            if (current != NULL) {
                loadPiece(i, j, current->type(), current->colour());
            }
        }
    }         
}
 
void SDL_Board::loadOverlay(int x, int y, OVERLAY_COL col)
{
    // Set blend mode & load relevant square
    SDL_SetRenderDrawBlendMode(windowRenderer, SDL_BLENDMODE_BLEND);
    SDL_Rect targetSquare = returnSDLSquare(x, y);

    // Set draw colour and fill rectangle
    switch(col)
    {
        case OVERLAY_RED: {
            SDL_SetRenderDrawColor(windowRenderer, OVERLAY_RED_R, OVERLAY_RED_G, OVERLAY_RED_B, 80);
            break;
        }
        case OVERLAY_GREEN: {
            SDL_SetRenderDrawColor(windowRenderer, OVERLAY_GREEN_R, OVERLAY_GREEN_G, OVERLAY_GREEN_B, 80);
            break;
        }
        case OVERLAY_YELLOW: {
            SDL_SetRenderDrawColor(windowRenderer, OVERLAY_YELLOW_R, OVERLAY_YELLOW_G, OVERLAY_YELLOW_B, 50);
            break;
        }
        case OVERLAY_WHITE: {
            SDL_SetRenderDrawColor(windowRenderer, OVERLAY_WHITE_R, OVERLAY_WHITE_G, OVERLAY_WHITE_B, 80);
            break;
        }
    }    
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

SDL_Rect SDL_Board::returnSDLSquare(int x, int y)
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
    loadState(state);
    SDL_RenderPresent(windowRenderer);
    renderLast(state.lastMove());
}


void SDL_Board::renderOverlay(const PositionVector& movesVector, const PositionVector& takesVector, const PositionVector& invalidsVector)
{

    // Iterate through movement queue and load overlays
    for(auto targetSquare:movesVector)
        loadOverlay(targetSquare.x, targetSquare.y, OVERLAY_GREEN);
    for(auto targetSquare:takesVector)
        loadOverlay(targetSquare.x, targetSquare.y, OVERLAY_RED);
    for(auto targetSquare:invalidsVector)
        loadOverlay(targetSquare.x, targetSquare.y, OVERLAY_WHITE);
        
    SDL_RenderPresent(windowRenderer);
}

void SDL_Board::renderLast(Move lastMove)
{
    if(!lastMove.first.validPosition() || !lastMove.second.validPosition())
        return;

    // Previous move
    loadOverlay(lastMove.first.x, lastMove.first.y, OVERLAY_YELLOW);
    loadOverlay(lastMove.second.x, lastMove.second.y, OVERLAY_YELLOW);
    SDL_RenderPresent(windowRenderer);
}



/****************************************************/
/*      STATIC METHODS: Additional functionality    */
/****************************************************/

BoardPosition SDL_Board::SDL_to_Coords(int SDL_x, int SDL_y)
{
    BoardPosition pos;
    pos.x = (SDL_x - BOARD_X)/SQUARE_WIDTH;
    pos.y = 7 - (SDL_y - BOARD_Y)/SQUARE_HEIGHT;

    if(m_orientation == BLACK)
    {
        pos.x = 7 - pos.x;
        pos.y = 7 - pos.y;
    }
    return pos;
}

void SDL_Board::setOrientation(COLOUR col)
{

    // Set SDL coordinates: Define the top left hand corner of the board
    int board_bottom_coord_x = BOARD_X; 
    int board_bottom_coord_y = 7*SQUARE_HEIGHT + BOARD_Y;
    int x, y;

    // Save orientation
    m_orientation = col;
    
    // Loop over and finish
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(col == WHITE)
            {
                x = i;
                y = j;
            }
            else
            {
                x = 7 - i;
                y = 7 - j;
            }
            SDL_coordinates[x][y][0] = board_bottom_coord_x + i*SQUARE_WIDTH; 
            SDL_coordinates[x][y][1] = board_bottom_coord_y - j*SQUARE_HEIGHT; 
        }
    }
}



