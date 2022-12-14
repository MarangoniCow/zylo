/***********************************************************
 *                      PIECE.H
 * 
 * Class to represent Chess pieces. Methodology utilises bit
 * shifting to encapsulate:
 *  - type (can be none)
 *  - movement (true or false)
 *  - colour    (white/black)
 *  - board position (x/y)
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "BoardPosition.h"

// EXTERNAL INCLUDES
#include <string>
#include <vector>
#include <queue>

// TYPEDEFS
typedef uint16_t PIECE_ID;
typedef std::queue<BoardPosition> PositionQueue;

enum COLOUR {
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
        Piece(PIECE_TYPE type_, COLOUR col_, int x_, int y_)
			{
                type(type_);
                colour(col_);
                x(x_);
                y(y_);
            };
        ~Piece() {}
		

        // Flag-related methods
		void	        type    (PIECE_TYPE type);
		PIECE_TYPE		type    () const;

		void	        colour  (COLOUR col);
		COLOUR          colour  () const;

        void	        moved   (bool b);
		bool	        moved   () const;

        void            ID      (PIECE_ID ID);
        PIECE_ID        ID      () const;

        int             x() const	{ return (int)((m_flags & MaskX) >> ShiftX); }
        int             y() const	{ return (int)((m_flags & MaskY) >> ShiftY); }

        void            position(BoardPosition pos);
        BoardPosition   position() const;
  
        void            moveRange (PositionQueue& moveQueue);
        void            resetFlags() { m_flags = 0; }

    protected:
        void	        x(int v)	{ m_flags = (m_flags & ~MaskX) | ((v << ShiftX) & MaskX); }
        void	        y(int v)	{ m_flags = (m_flags & ~MaskY) | ((v << ShiftY) & MaskY); }
		

        void   pawnRange  (PositionQueue& moveQueue);
        void   rookRange  (PositionQueue& moveQueue);
        void   knightRange(PositionQueue& moveQueue);
        void   bishopRange(PositionQueue& moveQueue);
        void   queenRange (PositionQueue& moveQueue);
        void   kingRange  (PositionQueue& moveQueue);

        
};