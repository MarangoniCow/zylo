/***********************************************************
 *                      ZYLODEFS
 * 
 * Collection of definitions for use throughout Zylo
 * 
 ***********************************************************/
#pragma once


// EXTERNAL INCLUDES
#include <vector>
#include "BoardDefs.h"


// CP: Centipawn (1/100th of a pawn value)
typedef double CP;

typedef std::vector<Move> MoveVector;

enum PHASE
{
    PHASE_NONE = 0,
    PHASE_OPEN,
    PHASE_MID,
    PHASE_END,
    PHASE_MAX
};

