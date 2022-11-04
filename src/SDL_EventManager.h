/***********************************************************
 *                   SDL_EVENTMANAGER.H
 * 
 * Handles all screen related events, but makes no decisions.
 ***********************************************************/


#pragma once

// INTERNAL DEPENDENCIES
#include "SDL_Board.h"
#include "BoardPosition.h"
#include "GameplayManager.h"

enum CLICK_TYPE {
    BOARD, OTHER
};

class SDL_EventManager
{
    private:
        bool isRunning;

        SDL_Board* gameWindow;
        GameplayManager* manager;
        Board* board;

        SDL_Event ev_cur;
        SDL_Event ev_prev;

        BoardPosition curPos;
        BoardPosition prevPos;

        

       
        

    public:
        SDL_EventManager(SDL_Board* gameWindow_, GameplayManager* manager_) : gameWindow(gameWindow_), manager(manager_) {
            board = manager->gameBoard();
        };
        void RunGame();
        
        // MOUSE EVENTS
        void MouseEvents();
        void BoardEvents();
        CLICK_TYPE processWindowClick();
        
        // KEYBOARD EVENTS
        void KeyboardEvents();

        // GAMEPLAY EVENTS
        bool checkGameplayFlags();
        void requestPiecePromotion();
        

};