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
            MaskX           = 0xE0,
            MaskY           = 0X700,
			ShiftColour		= 3,
            ShiftX          = 5,
            ShiftY          = 8,
            FlagMoved		= 0x10,
			};

public:
		BoardPiece();
        BoardPiece(PIECE_TYPE type_, COLOUR col_, int x_, int y_)
                                {
                                type(type_);
                                color(col_);
                                x(x_);
                                y(y_);
                                }
        
		
		void	        type(PIECE_TYPE v)		{ m_flags = (m_flags & ~MaskType) | (v & MaskType); }
		PIECE_TYPE		type() const	        { return (PIECE_TYPE)(m_flags & MaskType); }

		void	        color(COLOUR v)	{ m_flags = (m_flags & ~MaskColor) | ((v << ShiftColour) & MaskColor); }
		COLOUR    color() const	        { return (COLOUR)((m_flags & MaskColor) >> ShiftColour); }

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

        void	x(int v)	{ m_flags = (m_flags & ~MaskX) | ((v << ShiftX) & MaskX); }
		int     x() const	{ return (int)((m_flags & MaskX) >> ShiftX); }

        void	y(int v)	{ m_flags = (m_flags & ~MaskY) | ((v << ShiftY) & MaskY); }
		int     y() const	{ return (int)((m_flags & MaskY) >> ShiftY); }

protected:
		uint16_t	m_flags;
		};
