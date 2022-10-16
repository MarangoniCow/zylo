/***********************************************************
 *                   SDL_EVENTMANAGER.H
 * 
 * Handles all screen related events, but makes no decisions.
 ***********************************************************/


#pragma once

// INTERNAL DEPENDENCIES
#include "SDL_Board.h"
#include "BoardPosition.h"

enum CLICK_TYPE {
    BOARD, OTHER
};

class SDL_EventManager
{
    private:
        bool isRunning;

        SDL_Event ev_cur;
        SDL_Event ev_prev;

        SDL_Board* gameWindow;
        Board* board;

        BoardPosition curPos;
        BoardPosition prevPos;
        

    public:
        SDL_EventManager(SDL_Board* gameWindow_, Board* board_) : gameWindow(gameWindow_), board(board_) {};
        void RunGame();
        
        // MOUSE EVENTS
        void MouseEvents();
        void MovementEvents();
        CLICK_TYPE ProcessClick();
        
        // KEYBOARD EVENTS
        void KeyboardEvents();

        // GAMEPLAY EVENTS
        bool checkGameplayFlags();
        void requestPiecePromotion(BOARD_FLAGS boardFlags);
        

};