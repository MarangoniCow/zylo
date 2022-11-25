/***********************************************************
 *                    GAMEPLAYMANAGER
 * 
 *  Struct to contain information about a player:
 *      - Type (Network, local or zylo)
 *      - Assigned colour (White/black)
 *      
 * 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "BoardDefs.h"


enum PLAYER_TYPE
{
    LOCAL,
    ZYLO
};


class Player
{
    public:
        PLAYER_TYPE     type;
        COLOUR          colour;

    public:
        Player(PLAYER_TYPE type_, COLOUR colour_) :
                type(type_),
                colour(colour_)
                {};
};
