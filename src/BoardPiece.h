#pragma once

#include "BoardDefs.h"
#include "BoardState.h"

class BoardPiece
		{
public:
		enum Flags
			{
			MaskType		= 0x07,
			MaskColor		= 0x08,
			ShiftColor		= 3,
			FlagMoved		= 0x10,
			};

public:
		BoardPiece();
		
		void	type(int v)		{ m_flags = (m_flags & ~MaskType) | (v & MaskType); }
		int		type() const	{ return (int)(m_flags & MaskType); }

		void	color(int v)	{ m_flags = (m_flags & ~MaskColor) | ((v << ShiftColor) & MaskColor); }
		int		color() const	{ return (int)((m_flags & MaskColor) >> ShiftColor); }

		bool	moved() const	{ return ((m_flags & FlagMoved) != 0); }
		void	moved(bool v)	{
								if (v)
									{
									m_flags |= FlagMoved;
									}
								else
									{
									m_flags &= ~FlagMoved;
									}
								}

protected:
		uint8_t	m_flags;
		};
