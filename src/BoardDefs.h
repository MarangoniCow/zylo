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

typedef std::pair<BoardPosition, BoardPosition> Move;

typedef std::vector<Piece*> PieceVector;
typedef std::pair<bool, Piece*> PLAY_FLAG;


typedef std::pair<Piece*, PieceVector> PieceChecks;
typedef std::vector<PieceChecks> ChecksVector;
typedef std::queue<std::pair<Piece*, RELPOS>> SightedQueue;



// typedef std::queue<PieceChecks> ChecksQueue;
// typedef std::pair<bool, PIECE_ID> PLAY_FLAG;
// typedef std::pair<PIECE_ID, PieceQueue> PieceChecks;