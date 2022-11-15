#pragma once

#include "BoardDefs.h"
#include "BoardState.h"
#include "BoardPiece.h"


class BoardSnapshot
		{
public:
public:
		BoardSnapshot();

		void		grabBoardState(const BoardState &board);

protected:
		BoardPiece	m_board[8][8];
		Move		m_lastMove;
		};

