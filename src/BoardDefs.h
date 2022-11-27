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

/***********************************************************
*       Class definitions
************************************************************/


struct Sightline
{
    Piece*  tarPiece;
    RELPOS  relPos;
    int     n_Blocking;

    Sightline(Piece* p, RELPOS rel, int N) :
                    tarPiece(p),
                    relPos(rel),
                    n_Blocking(N)
                    {};
};

typedef std::vector<Sightline> SightedVector;

struct Movements
{
    PositionVector validMoves;
    PositionVector validTakes;
    PositionVector invalidMoves;
    PositionVector invalidTakes;
    SightedVector  sightedVector;
};

struct MovementState
{
    Movements	state[8][8];
};

struct ChecksState
{
    PieceChecks     state[8][8];
};