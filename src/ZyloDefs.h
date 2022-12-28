/***********************************************************
 *                      ZYLODEFS
 * 
 * Collection of definitions for use throughout Zylo
 * 
 ***********************************************************/

// EXTERNAL INCLUDES
#include <vector>
#include "BoardDefs.h"


// CP: Centipawn (1/100th of a pawn value)
typedef double CP;

typedef std::vector<Move> MoveVector;

enum PHASE
{
    PHASE_NONE,
    PHASE_OPEN,
    PHASE_MID,
    PHASE_END
};

