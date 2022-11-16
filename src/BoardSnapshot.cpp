#include "BoardSnapshot.h"
#include "Piece.h"
#include <cstring>


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
			BoardPiece  &bp=m_board[p->x()][p->y()];

			bp.type(p->type());
			bp.moved(p->hasMoved());
			bp.color(p->colour());
			}

        m_lastMove = state.lastMove;
		}
