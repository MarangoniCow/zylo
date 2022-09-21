/* SDL_EVENTMANAGER.H
/ Manages all game events
/
/ 
/                               */


#pragma once

// INTERNAL DEPENDENCIES
#include "SDL_Board.h"



class SDL_EventManager
{
    public:
        void RunGame();
        void MouseToBoardCoords(int* x, int* y);
};