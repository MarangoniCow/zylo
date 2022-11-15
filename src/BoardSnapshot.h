#pragma once

#include "BoardDefs.h"
#include "BoardState.h"

typedef uint8_t	byte_t;

#define	MASK_TYPE	0x07
#define FLAGS_COLOR	0x08
#define FLAGS_MOVED	0x10

class BoardPiece
		{
public:
		BoardPiece();
		
		void	type(int v)		{ m_flags = (m_flags & ~MASK_TYPE) | (v & MASK_TYPE); }
		int		type() const	{ return (int)(m_flags & MASK_TYPE); }

		bool	moved() const	{ return ((m_flags & FLAGS_MOVED) != 0); }
		void	moved(bool v)	{
								if (v)
									{
									m_flags |= FLAGS_MOVED;
									}
								else
									{
									m_flags &= ~FLAGS_MOVED;
									}
								}

		int		color() const	{ return (((m_flags & FLAGS_COLOR) != 0)?WHITE:BLACK); }

protected:
		byte_t	m_flags;
		};


class BoardSnapshot
		{
public:
		BoardSnapshot();

		void		grabBoardState(const BoardState &board);

protected:
		byte_t		m_board[8][8];
		Move		m_lastMove;
		};

