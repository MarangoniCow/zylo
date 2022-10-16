/***********************************************************
 *                    GAMEPLAYMANAGER
 * 
 *  Class to handle turns, keep track of taken pieces,
 *  turn history, winning conditions, etc., etc.
 * 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "Piece.h"

// EXTERNAL INCLUDES


class GameplayManager
{
    private:
        PIECE_COLOUR currentTurn;

    public:
        // CONSTRUCTORS 
        GameplayManager();
        ~GameplayManager();



};
