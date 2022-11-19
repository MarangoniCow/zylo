/***********************************************************
 *              SDL_EVENTMANAGER IMPLEMENTATION
 ***********************************************************/

// INTERNAL DEPENDENCIES
#include "SDL_EventManager.h"
#include "SDL_Board.h"
#include "BoardPosition.h"
#include "Board.h"
#include "Piece.h"
#include "GameplayManager.h"


// EXTERNAL DEPENDENCIES
#include <SDL2/SDL.h>
#include <iostream>
#include <queue>


void SDL_EventManager::RunGame() {
        
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
    CLICK_TYPE click = processWindowClick();

    if (click == BOARD)
    {
        BoardEvents();
    }
    else
    {
        // Nothing else yet, space here for any other functions
    }      
}

void SDL_EventManager::BoardEvents()
{

    BOARD_EVENT ev_board = manager->processBoardClick(prevPos, curPos);
    switch(ev_board)
    {
        case OVERLAY:
        {
            MovementQueue moveQueue = board->returnMovementQueue(curPos);
            gameWindow->renderOverlay(moveQueue.validMoves, moveQueue.validTakes, moveQueue.invalidMoves);
            break;
        }
        case PROMOTION:
        {
            requestPiecePromotion();
            gameWindow->renderBoard(board->getState());
            gameWindow->renderLast(board->getLastMove());
            break;
        }
        case INVALID:
        {
            curPos.resetPosition();
            prevPos.resetPosition();
            gameWindow->renderBoard(board->getState());
            gameWindow->renderLast(board->getLastMove());
            break;
        }
        case MOVE:
        {
            gameWindow->renderBoard(board->getState());
            gameWindow->renderLast(board->getLastMove());
            
            // Reset the click location
            curPos.resetPosition();
            prevPos.resetPosition();

            break;
        }
        default: {}
    }   
}

CLICK_TYPE SDL_EventManager::processWindowClick()
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
            manager->newGame();
            gameWindow->renderBoard(manager->gameBoard()->getState());
            break;
        }
        case SDLK_LEFT:
        {
            manager->traverseHistory(BACKWARD);
            gameWindow->renderBoard(manager->gameBoard()->getState());
            break;
        }
        case SDLK_RIGHT:
        {
            manager->traverseHistory(FORWARD);
            gameWindow->renderBoard(manager->gameBoard()->getState());
            break;
        }
        default:
        {};
    }
}

/****************************************************/
/*                 SPECIAL EVENTS                   */
/****************************************************/

bool SDL_EventManager::checkGameplayFlags()
{
    BOARD_FLAGS boardFlags = board->getFlags();

    // Check for promotion
    if(boardFlags.pawnPromotion.first)
    {
        requestPiecePromotion();
        return 1;
    }
    return 0;
}

void SDL_EventManager::requestPiecePromotion()
{  
    // Boolean for found
    bool found = 0;
    PIECE_TYPE promotionType;
    
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
                        promotionType = QUEEN;
                        found = 1;
                        break;
                    }
                    case SDLK_k:
                    {
                        promotionType = KNIGHT;
                        found = 1;
                        break;
                    }
                    case SDLK_r:
                    {
                        promotionType = ROOK;
                        found = 1;
                        break;
                    }
                    case SDLK_b:
                    {
                        promotionType = BISHOP;
                        found = 1;
                        break;
                    }
                    default:
                    {break;};
                }
            }
        }
    }
    board->processPromotion(promotionType);
}
