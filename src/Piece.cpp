/***********************************************************
 *                      PIECE IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "Piece.h"

// EXTERNAL INCLUDES
#include <string>
#include <iostream>
#include <vector>


void Piece::type(PIECE_TYPE type)
{
    m_flags = (m_flags & ~MaskType) | (v & MaskType);
}
void Piece::type() const
{
    return (PIECE_TYPE)(m_flags & MaskType);   
}

void Piece::colour(PIECE_COLOUR col)
{
    m_flags = (m_flags & ~MaskColor) | ((col << ShiftColour) & MaskColor);
}
PIECE_COLOUR Piece::colour() const
{
    return (PIECE_COLOUR)((m_flags & MaskColor) >> ShiftColour);
}

void Piece::moved(bool b)
{  
    if (b)
        m_flags |= FlagMoved;
    else
        m_flags &= ~FlagMoved;
}
bool Piece::moved() const
{
    return ((m_flags & FlagMoved) != 0);
}

void Piece::ID(PIECE_ID ID)
{
    m_flags = ID;
}
PIECE_ID Piece::ID() const
{
    m_flags & MaskID;
}

void Piece::position(BoardPosition pos)
{
    x(pos.x);
    y(pos.y);
    moved(true);
}
BoardPosition Piece::position() const
{
    BoardPosition pos(x(), y());
    return pos;
}

/***********************************************************
 *                      MOVE RANGES
 * 
 * All movement ranges are defined such that all movements in
 * a particular axis are sequentially defined. This allows 
 * methods in-built to Board to check all the movements in one
 * direction sequentially, saving a lot of hassle!
 * 
 ***********************************************************/
PositionQueue Piece::moveRange()
{ 
    switch(type())
    {
        case PAWN:
        {
            return pawnRange();
        }
        case ROOK:
        {
            return rookRange();
        }
        case KNIGHT:
        {
            return knightRange();
        }
        case BISHOP:
        {
            return bishopRange();
        }
        case QUEEN:
        {
            return queenRange();
        }
        case KING:
        {
            return kingRange();
        }
        default
        {
            PositionQueue queue;
            return queue;
        }
    }
}

PositionQueue Piece::pawnRange()
{
    PositionQueue moveQueue;
    BoardPosition m_pos = position();

    // WHITE = 0, BLACK = 1
    if(!m_col) {
        if(m_pos.validUpdate(0, 1)) moveQueue.push(m_pos.returnUpdate(0, 1));
        if(!m_hasMoved && m_pos.validUpdate(0, 2)) moveQueue.push(m_pos.returnUpdate(0, 2));
    }
    else {
        if(m_pos.validUpdate(0, -1)) moveQueue.push(m_pos.returnUpdate(0, -1));
        if(!m_hasMoved && m_pos.validUpdate(0, -2)) moveQueue.push(m_pos.returnUpdate(0, -2));
    }  
    return moveQueue;
}

PositionQueue Piece::rookRange()
{
    PositionQueue moveQueue;
    BoardPosition m_pos = position();

    // Moving right
    int i = 1;
    while(m_pos.validUpdate(i, 0)) {
        moveQueue.push(m_pos.returnUpdate(i, 0));
        i++;
   }

    // Moving left
    i = -1;
    while(m_pos.validUpdate(i, 0)) {
        moveQueue.push(m_pos.returnUpdate(i, 0));
        i--;
    }

    // Moving forward
    int j = 1;
    while(m_pos.validUpdate(0, j)) {
        moveQueue.push(m_pos.returnUpdate(0, j));
        j++;
    }

    // Moving backward
    j = -1;
    while(m_pos.validUpdate(0, j)) {
        moveQueue.push(m_pos.returnUpdate(0, j));
        j--;
    }
    
    return moveQueue;
}

PositionQueue Piece::knighteRange()
{
    PositionQueue moveQueue;
    BoardPosition m_pos = position();
    
    // Binary permutations of {1,2,-1,-2} without repettition of 1s/2s.
    if(m_pos.validUpdate(1, 2)) moveQueue.push(m_pos.returnUpdate(1, 2));
    if(m_pos.validUpdate(2, 1)) moveQueue.push(m_pos.returnUpdate(2, 1));

    if(m_pos.validUpdate(-1, 2)) moveQueue.push(m_pos.returnUpdate(-1, 2));
    if(m_pos.validUpdate(-2, 1)) moveQueue.push(m_pos.returnUpdate(-2, 1));

    if(m_pos.validUpdate(1, -2)) moveQueue.push(m_pos.returnUpdate(1, -2));
    if(m_pos.validUpdate(2, -1)) moveQueue.push(m_pos.returnUpdate(2, -1));

    if(m_pos.validUpdate(-1, -2)) moveQueue.push(m_pos.returnUpdate(-1, -2));
    if(m_pos.validUpdate(-2, -1)) moveQueue.push(m_pos.returnUpdate(-2, -1));

    return moveQueue;
    
}

PositionQueue Piece::bishopRange()
{
    PositionQueue moveQueue;
    BoardPosition m_pos = position();

    // Moving diagonal right
    int i = 1;
    int j = 1;
    while(m_pos.validUpdate(i, j)) {
        moveQueue.push(m_pos.returnUpdate(i, j));
        i++;
        j++;
    }

    // Moving diagonal left
    i = -1;
    j = 1;
    while(m_pos.validUpdate(i, j)) {
        moveQueue.push(m_pos.returnUpdate(i, j));
        i--;
        j++;
    }

    // Moving diagonal down-right
    i = 1;
    j = -1;
    while(m_pos.validUpdate(i, j)) {
        moveQueue.push(m_pos.returnUpdate(i, j));
        i++;
        j--;
    }

    // Moving diagonal down-left
    i = -1;
    j = -1;
    while(m_pos.validUpdate(i, j)) {
        moveQueue.push(m_pos.returnUpdate(i, j));
        i--;
        j--;
    }

    return moveQueue;
}

PositionQueue Piece::queenRange()
{
    PositionQueue moveQueue;
    BoardPosition m_pos = position();

    // Moving right
    int i = 1;
    while(m_pos.validUpdate(i, 0)) {
        moveQueue.push(m_pos.returnUpdate(i, 0));
        i++;
    }

    // Moving left
    i = -1;
    while(m_pos.validUpdate(i, 0)) {
        moveQueue.push(m_pos.returnUpdate(i, 0));
        i--;
    }

    // Moving forward
    int j = 1;
    while(m_pos.validUpdate(0, j)) {
        moveQueue.push(m_pos.returnUpdate(0, j));
        j++;
    }

    // Moving backward
    j = -1;
    while(m_pos.validUpdate(0, j)) {
        moveQueue.push(m_pos.returnUpdate(0, j));
        j--;
    }

    // Moving diagonal right
    i = 1;
    j = 1;
    while(m_pos.validUpdate(i, j)) {
        moveQueue.push(m_pos.returnUpdate(i, j));
        i++;
        j++;
    }

    // Moving diagonal left
    i = -1;
    j = 1;
    while(m_pos.validUpdate(i, j)) {
        moveQueue.push(m_pos.returnUpdate(i, j));
        i--;
        j++;
    }

    // Moving diagonal down-right
    i = 1;
    j = -1;
    while(m_pos.validUpdate(i, j)) {
        moveQueue.push(m_pos.returnUpdate(i, j));
        i++;
        j--;
    }

    // Moving diagonal down-left
    i = -1;
    j = -1;
    while(m_pos.validUpdate(i, j)) {
        moveQueue.push(m_pos.returnUpdate(i, j));
        i--;
        j--;
    }

    return moveQueue;
    
}

PositionQueue Piece::kingRange()
{
    PositionQueue moveQueue;
    BoardPosition m_pos = position();

    // Permutations of {0, 1, -1}
    if(m_pos.validUpdate(1, 0))   moveQueue.push(m_pos.returnUpdate(1, 0));
    if(m_pos.validUpdate(1, 1))   moveQueue.push(m_pos.returnUpdate(1, 1));
    if(m_pos.validUpdate(0, 1))   moveQueue.push(m_pos.returnUpdate(0, 1));
    if(m_pos.validUpdate(-1, 0))  moveQueue.push(m_pos.returnUpdate(-1, 0));
    if(m_pos.validUpdate(-1, -1)) moveQueue.push(m_pos.returnUpdate(-1, -1));
    if(m_pos.validUpdate(0, -1))  moveQueue.push(m_pos.returnUpdate(0, -1));
    if(m_pos.validUpdate(1, -1))  moveQueue.push(m_pos.returnUpdate(1, -1));
    if(m_pos.validUpdate(-1, 1))  moveQueue.push(m_pos.returnUpdate(-1, 1));
    
    return moveQueue;
    
}