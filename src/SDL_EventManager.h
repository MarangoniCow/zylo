/***********************************************************
 *                   SDL_EVENTMANAGER.H
 * 
 * Handles all game events, clicks, keyboard strokes, etc etc
 ***********************************************************/


#pragma once

// INTERNAL DEPENDENCIES
#include "SDL_Board.h"
#include "BoardPosition.h"


class SDL_EventManager
{
    private:
        SDL_Event ev_cur;
        SDL_Event ev_prev;
        BoardPosition curBoardCoord;
        BoardPosition prevBoardCoord;

    public:
        void RunGame();
        void MouseToBoardCoords();

        /* PLACEHOLDER METHODS: Nothing to see here! */
        void MouseEvents();
        void KeyboardEvents();

};