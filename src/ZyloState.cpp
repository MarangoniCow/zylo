/***********************************************************
 *                  ZYLOBOARD IMLPEMENTATION
 **********************************************************/

// INTERNAL INCLUDES
#include "ZyloState.h"
#include "ZyloDefs.h"
#include "ZyloPiece.h"

// EXTERNAL INCLUDES


CP ZyloState::fetchStateRating()
{
    return sumPieceValues();
}

CP ZyloState::sumPieceValues()
{
    CP pieceSum = 0;

    // Run over the entire board and evaluate
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(m_state.current[i][j].type() != NONE)
                pieceSum += ZyloPiece::getPieceRating(m_state.current[i][j]);

    return pieceSum;
}

