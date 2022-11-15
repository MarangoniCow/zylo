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
        Piece(PIECE_TYPE type, PIECE_COLOUR col_, int x, int y) :
			m_col(col_),
			m_pos(x, y),
			m_type(type),
			m_hasMoved(false)
			{
            // Assert global methodology
            m_ID = (PIECE_ID)Piece_instanceList.size();
            Piece_instanceList.push_back(this); 
            m_hasMoved = 0;
        };
        Piece(PIECE_TYPE type, PIECE_COLOUR col_, int x, int y, PIECE_ID ID_) :
			m_col(col_),
			m_pos(x, y),
			m_ID(ID_),
			m_type(type),
			m_hasMoved(true)
			{
            Piece_instanceList[ID_] = this;
            m_hasMoved = 1; 
        };
        virtual ~Piece() {
            Piece_instanceList[m_ID] = nullptr;
        }

        // STATIC METHODS FOR GLOBAL FUNCTIONALITY
        static Piece* returnIDPtr(PIECE_ID ID_) {return Piece_instanceList[ID_];}; 
        static int returnTotalPieces() {return (int)size(Piece_instanceList);};

        // NON-STATIC RETURNS
		PIECE_TYPE      type() const        { return m_type;}	
		PIECE_ID        ID() const          { return m_ID;}
        PIECE_COLOUR    colour() const      { return m_col;}
        BoardPosition   position() const    { return m_pos;}
        

        // FLAGS
        bool hasMoved() {return m_hasMoved;};

        // SETTER METHODS
        void updatePosition(BoardPosition pos);
        
        // FUNCTIONALITY METHODS
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
