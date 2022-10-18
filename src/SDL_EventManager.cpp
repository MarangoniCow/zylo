/***********************************************************
 *              SDL_EVENTMANAGER IMPLEMENTATION
 ***********************************************************/

// INTERNAL DEPENDENCIES
#include "SDL_EventManager.h"
#include "SDL_Board.h"
#include "BoardPosition.h"
#include "Board.h"
#include "Piece.h"


// EXTERNAL DEPENDENCIES
#include <SDL2/SDL.h>
#include <iostream>
#include <queue>


void SDL_EventManager::RunGame() {

    /* CURRENT STATE REPORT 
    * Long term plan: Move most of the initialisation of the board logic, etc etc to zylo.cpp
    * and re-arrange the EventManager to do that: manage the events.
    * For the moment, everything is just initialised here to get things running.
    */
   PIECE_COLOUR currentTurn = WHITE;
       
    // Game boolean
    isRunning = true;
    
    // Run the game!
    while(isRunning)
    {
        // Poll event checks for event and processes events each frame
        while(SDL_PollEvent(&ev_cur) != 0)
        {
            if (ev_cur.type == SDL_QUIT)
            {
                isRunning = false;
                continue;
            }
            else if(checkGameplayFlags())
            {
                gameWindow->renderBoard(board->returnState());
                continue;
            }

            if(ev_cur.type == SDL_MOUSEBUTTONDOWN) {
                MouseEvents();          
            }
            else if (ev_cur.type == SDL_KEYDOWN) {
                KeyboardEvents();
            }
        }
    }
    SDL_Quit(); 
};



/****************************************************/
/*                  MOUSE EVENTS                    */
/****************************************************/


void SDL_EventManager::MouseEvents()
{
    CLICK_TYPE click = ProcessClick();

    if (click == BOARD)
    {
        MovementEvents();
    }
    else
    {
        // Nothing else yet
    }      
}

void SDL_EventManager::MovementEvents()
{

    // Check for previous board coordinates
    if(prevPos.validPosition()) {
        
        // Process a click and update the board
        board->processUpdate(prevPos, curPos);
        gameWindow->renderBoard(board->returnState());
        
        // Reset the click location
        curPos.ResetPosition();
        prevPos.ResetPosition();
    }
    else
    {
        // Generate the mvoement range, which is stored in board, and pass those movement queues to render overlay
        MovementQueue moveQueue = board->generateMovementRange(curPos);
        gameWindow->renderOverlay(moveQueue.validMoves, moveQueue.validTakes, moveQueue.invalidMoves);
    }    
}

CLICK_TYPE SDL_EventManager::ProcessClick()
{
    BoardPosition clickPos;  
    // Fetch screen coordinates and translate into board coordinates
    int SDL_x, SDL_y;
    SDL_GetMouseState(&SDL_x, &SDL_y);
    clickPos = SDL_Board::SDL_to_Coords(SDL_x, SDL_y);

    // If a valid board location...
    if(clickPos.validPosition())     
    {
        // Check for a current click, if no previous click, save current click as prev
        if(curPos.validPosition())
        {
            
            prevPos = curPos;
        }
        curPos = clickPos;
        return BOARD;
    }
    else
    {
        return OTHER;
    }    
};



/****************************************************/
/*                 KEYBOARD EVENTS                  */
/****************************************************/

void SDL_EventManager::KeyboardEvents()
{
    switch(ev_cur.key.keysym.sym)
    {
        case SDLK_r:
        {
            board->clearBoard();
            board->initialiseBoard();
            gameWindow->renderBoard(board->returnState());
            break;
        }
        default:
        {break;};
    }
}

/****************************************************/
/*                 SPECIAL EVENTS                   */
/****************************************************/

bool SDL_EventManager::checkGameplayFlags()
{
    BoardState state = board->returnState();

    // Check for promotion
    if(state.boardFlags.pawnPromotion.first)
    {
        requestPiecePromotion(state.boardFlags);
        board->resetFlags();
        return 1;
    }
    return 0;
}

void SDL_EventManager::requestPiecePromotion(BOARD_FLAGS boardFlags)
{
    // Fetch necessary piece ID
    PIECE_ID ID = boardFlags.pawnPromotion.second;
    Piece* currentPiece = Piece::returnIDPtr(ID);
    BoardPosition pos = currentPiece->returnPosition();
    PIECE_COLOUR col = currentPiece->returnColour();
    
    // Remove the piece
    board->removePiece(ID);
    bool found = 0;
    
    // Start an event loop to fetch the new item.
    while(!found)
    {
        while(SDL_PollEvent(&ev_cur) != 0)
        {
            if (ev_cur.type == SDL_QUIT)
                isRunning = false;
            else if (ev_cur.type == SDL_KEYDOWN)
            {
                switch(ev_cur.key.keysym.sym)
                {
                    case SDLK_q:
                    {
                        board->addPieceToState(new Queen(col, pos.x, pos.y, ID));
                        found = 1;
                        break;
                    }
                    case SDLK_k:
                    {
                        board->addPieceToState(new Knight(col, pos.x, pos.y, ID));
                        found = 1;
                        break;
                    }
                    case SDLK_r:
                    {
                        board->addPieceToState(new Rook(col, pos.x, pos.y, ID));
                        found = 1;
                        break;
                    }
                    case SDLK_b:
                    {
                        board->addPieceToState(new Bishop(col, pos.x, pos.y, ID));
                        found = 1;
                        break;
                    }
                    default:
                    {break;};
                }
            }
        }
    }
}


