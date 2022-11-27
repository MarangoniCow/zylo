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
typedef std::vector<BoardPosition> PositionVector;
typedef std::pair<bool, BoardPosition> PLAY_FLAG;

/***********************************************************
*       Piece types
************************************************************/

typedef std::vector<Piece*> PieceVector;



typedef std::pair<Piece*, PieceVector> PieceChecks;
typedef std::vector<PieceChecks> ChecksVector;
typedef std::vector<std::pair<Piece*, RELPOS>> SightedVector;

/***********************************************************
*       Class definitions
************************************************************/

struct Movements
{
    PositionVector validMoves;
    PositionVector validTakes;
    PositionVector invalidMoves;
    PositionVector invalidTakes;
    SightedVector sightedQueue;
};

struct MovementState
{
    Movements	state[8][8];
};

struct ChecksState
{
    PieceChecks     state[8][8];
};