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
        Piece(PIECE_TYPE type_, PIECE_COLOUR col_, int x_, int y_)
			{
                type(type_);
                colour(col_);
                x(x_);
                y(y_);
            };
        Piece(PIECE_TYPE type_, PIECE_COLOUR col_, int x_, int y_, PIECE_ID ID_) 
			{
                ID(ID_);
                type(type_);
                colour(col_);
                x(x_);
                y(y_);
                moved(true);
            };
        virtual ~Piece() {}
		
		void	        type(PIECE_TYPE type);
		PIECE_TYPE		type() const;

		void	        colour(PIECE_COLOUR col);
		PIECE_COLOUR    colour() const;

        void	        moved(bool v);
		bool	        moved() const;

        void            ID(PIECE_ID ID);
        PIECE_ID        ID();

        void            position(BoardPosition pos);
        BoardPosition   position();

        virtual PositionQueue moveRange() = 0;      
};



/***********************************************************
 *                   PIECE SUB-CLASSES
 ***********************************************************/

class Pawn : public Piece
{
    public:
        Pawn(PIECE_COLOUR col_, int x, int y) : Piece(PAWN, col_, x, y) {};
        Pawn(PIECE_COLOUR col_, int x, int y, PIECE_ID ID) : Piece(PAWN, col_, x, y, ID) {};

        ~Pawn() {};
        PositionQueue moveRange();
};

class Rook : public Piece
{
    public:
        Rook(PIECE_COLOUR col_, int x, int y) : Piece(ROOK, col_, x, y){};
        Rook(PIECE_COLOUR col_, int x, int y, PIECE_ID ID) : Piece(ROOK, col_, x, y, ID){};
        ~Rook() {};
        PositionQueue moveRange();
};

class Knight : public Piece
{
    public:
        Knight(PIECE_COLOUR col_, int x, int y) : Piece(KNIGHT, col_, x, y) {};
        Knight(PIECE_COLOUR col_, int x, int y, PIECE_ID ID) : Piece(KNIGHT, col_, x, y, ID) {};
        ~Knight() {};
        PositionQueue moveRange();
};

class Bishop : public Piece
{
    public:
        Bishop(PIECE_COLOUR col_, int x, int y) : Piece(BISHOP, col_, x, y) {};
        Bishop(PIECE_COLOUR col_, int x, int y, PIECE_ID ID) : Piece(BISHOP, col_, x, y, ID) {};
        ~Bishop() {};
        PositionQueue moveRange();
};

class Queen : public Piece
{
    public:
        Queen(PIECE_COLOUR col_, int x, int y) : Piece(QUEEN, col_, x, y) {};
        Queen(PIECE_COLOUR col_, int x, int y, PIECE_ID ID) : Piece(QUEEN, col_, x, y, ID) {};
        ~Queen() {};
        PositionQueue moveRange();
};

class King : public Piece
{
    public:
        King(PIECE_COLOUR col_, int x, int y) : Piece(KING, col_, x, y) {};
        King(PIECE_COLOUR col_, int x, int y, PIECE_ID ID) : Piece(KING, col_, x, y, ID) {};
        ~King() {};
        PositionQueue moveRange();
};
