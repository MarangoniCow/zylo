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


typedef unsigned int PIECE_ID;
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
        // STATIC MEMBERS FOR GLOBAL FUNCTIONALITY
        static std::vector<Piece*> Piece_instanceList;          // Global ID list
        static int Piece_count;                                 // Global piece count

        // INSTANCE SPECIFIC FUNCTIONALITY
        PIECE_ID		m_ID;                                           // Unique Piece ID
        PIECE_COLOUR	m_col;                                 // Enumerated colour
        PIECE_TYPE		m_type;
        BoardPosition	m_pos;                                     // Current position
        bool			m_hasMoved;                                         // Movement flag
        
    public:
        Piece(PIECE_TYPE type_, PIECE_COLOUR col_, int x_, int y_) :
			m_col(col_),
			m_pos(x_, y_),
			m_type(type_),
			m_hasMoved(false)
			{
            // Assert global methodology
            m_ID = (PIECE_ID)Piece_instanceList.size();
            Piece_instanceList.push_back(this); 
            m_hasMoved = 0;
            m_flags = 0;

            type(type_);
            colour(col_);
            x(x_);
            y(y_);

            };
        Piece(PIECE_TYPE type_, PIECE_COLOUR col_, int x_, int y_, PIECE_ID ID_) :
			m_ID(ID_),
            m_col(col_),
			m_pos(x_, y_),
			m_type(type_),
			m_hasMoved(true)
			{
            Piece_instanceList[ID_] = this;
            m_hasMoved = 1;
            type(type_);
            colour(col_);
            x(x_);
            y(y_);
            moved(true);
            };
        virtual ~Piece() {
            Piece_instanceList[m_ID] = nullptr;
        }

        // STATIC METHODS FOR GLOBAL FUNCTIONALITY
        static int returnTotalPieces() {return (int)size(Piece_instanceList);};      

        // FLAGS
        PIECE_ID ID()   {return m_flags & MaskID;};
        bool hasMoved() {return m_hasMoved;};

        // SETTER METHODS
        void updatePosition(BoardPosition pos);
        
        // FUNCTIONALITY METHODS
        virtual PositionQueue moveRange() = 0;






        /****************************************************
        *       BOARD PIECE INTEGRATION                     */

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
		
		void	        type(PIECE_TYPE v)		{ m_flags = (m_flags & ~MaskType) | (v & MaskType); }
		PIECE_TYPE		type() const	        { return (PIECE_TYPE)(m_flags & MaskType); }

		void	        colour(PIECE_COLOUR v)	{ m_flags = (m_flags & ~MaskColor) | ((v << ShiftColour) & MaskColor); }
		PIECE_COLOUR    colour() const	        { return (PIECE_COLOUR)((m_flags & MaskColor) >> ShiftColour); }

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

        void    position(BoardPosition pos)     {
                                                x(pos.x);
                                                y(pos.y);
                                                }
        BoardPosition position()                {
                                                BoardPosition pos(x(), y());
                                                return pos;
                                                }


protected:
		uint16_t	m_flags;
        
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
