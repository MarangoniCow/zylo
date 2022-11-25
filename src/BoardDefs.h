/***********************************************************
 *                      BOARDDEFS
 * 
 * Collection of typedefs used throughout Board logic
 * 
 ***********************************************************/


#pragma once


// INTERNAL INCLUDES
#include "Piece.h"
#include "BoardPosition.h"


// EXTERNAL INCLUDES
#include <vector>

/***********************************************************
*       Positional types
************************************************************/

typedef std::pair<BoardPosition, BoardPosition> Move;
typedef std::pair<bool, BoardPosition> PLAY_FLAG;

/***********************************************************
*       Piece types
************************************************************/

typedef std::vector<Piece*> PieceVector;



typedef std::pair<Piece*, PieceVector> PieceChecks;
typedef std::vector<PieceChecks> ChecksVector;
typedef std::queue<std::pair<Piece*, RELPOS>> SightedQueue;

/***********************************************************
*       Class definitions
************************************************************/

class MovementQueue
{
public:

    PositionQueue validMoves;
    PositionQueue validTakes;
    PositionQueue invalidMoves;
    PositionQueue invalidTakes;
    SightedQueue sightedQueue;

public:

    MovementQueue   () {};
    MovementQueue   (PositionQueue validMoves_,
                    PositionQueue validTakes_,
                    PositionQueue invalidMoves_,
                    PositionQueue invalidTakes_,
                    SightedQueue sightedQueue_) :
                    sightedQueue(sightedQueue_),
                    validMoves(validMoves_),
                    validTakes(validTakes_),
                    invalidMoves(invalidMoves_),
                    invalidTakes(invalidTakes_)
                    {};
};

struct MovementState
{
    MovementQueue	state[8][8];
};

struct ChecksState
{
    PieceChecks     state[8][8];
};