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

			BoardPosition	pos = p->returnPosition();
			BoardPiece		&bp=m_board[pos.x][pos.y];

			bp.type(p->type());
			bp.moved(p->hasMoved());
			bp.color(p->color());
			}
		}
