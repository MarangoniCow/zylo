/***********************************************************
 *                   SDL_EVENTMANAGER.H
 * 
 * Handles all game events, clicks, keyboard strokes, etc etc
 ***********************************************************/


#pragma once

// INTERNAL DEPENDENCIES
#include "SDL_Board.h"
#include "Coords.h"




class SDL_EventManager
{
    private:
        SDL_Event ev_cur;
        SDL_Event ev_prev;
        BoardPosition click_location_curr;
        BoardPosition click_location_prev;

    public:
        void RunGame();
        void MouseToBoardCoords();
        void MouseEvents();
        void KeyboardEvents();

};