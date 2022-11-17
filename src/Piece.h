/***********************************************************
 *                      PIECE.H
 * 
 *  Class to handle all pieces and movement ranges
 *      - file path
 *      - colour
 *      - piece type
 *      - Global piece list & ID
 * 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "BoardPosition.h"

// EXTERNAL INCLUDES
#include <string>
#include <vector>
#include <queue>


typedef uint16_t PIECE_ID;
typedef std::queue<BoardPosition> PositionQueue;

enum PIECE_COLOUR {
    WHITE = 0,
    BLACK = 1
};

enum PIECE_TYPE {
	NONE = 0,
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,

	// Last one
	MAX_TYPE
};



class Piece 
{
    protected:
		uint16_t	m_flags;

    protected:
        void	x(int v)	{ m_flags = (m_flags & ~MaskX) | ((v << ShiftX) & MaskX); }
		int     x() const	{ return (int)((m_flags & MaskX) >> ShiftX); }

        void	y(int v)	{ m_flags = (m_flags & ~MaskY) | ((v << ShiftY) & MaskY); }
		int     y() const	{ return (int)((m_flags & MaskY) >> ShiftY); }

    public:
		enum Flags
			{
			MaskType		= 0x07,
			MaskColor		= 0x08,
            MaskX           = 0xE0,
            MaskY           = 0X700,
            MaskID          = 0x7FF,
			ShiftColour		= 3,
            ShiftX          = 5,
            ShiftY          = 8,
            FlagMoved		= 0x10,
			};
        
    public:
        Piece()
            {
                m_flags = 0;
            }
        Piece(PIECE_TYPE type_, PIECE_COLOUR col_, int x_, int y_)
			{
                type(type_);
                colour(col_);
                x(x_);
                y(y_);
            };
        ~Piece() {}
		
		void	        type(PIECE_TYPE type);
		PIECE_TYPE		type() const;

		void	        colour(PIECE_COLOUR col);
		PIECE_COLOUR    colour() const;

        void	        moved(bool v);
		bool	        moved() const;

        void            ID(PIECE_ID ID);
        PIECE_ID        ID() const;

        void            position(BoardPosition pos);
        BoardPosition   position() const;

        PositionQueue   moveRange();

    protected:
        PositionQueue pawnRange  ();
        PositionQueue rookRange  ();
        PositionQueue knightRange();
        PositionQueue bishopRange();
        PositionQueue queenRange ();
        PositionQueue kingRange  ();

        
};