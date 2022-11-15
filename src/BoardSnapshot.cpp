#include "BoardSnapshot.h"
#include "Piece.h"


BoardSnapshot::BoardSnapshot()
		{
		memset(m_board, 0, 64);
		}


void
BoardSnapshot::grabBoardState(const BoardState &state)
		{
		PieceQueue pieceQueue;
		
		state.getPieceQueue(pieceQueue);

		while (!pieceQueue.empty())
			{
			Piece	*p = pieceQueue.front();

			pieceQueue.pop();

			BoardPosition	pos = p->returnPosition();
			}
		}
