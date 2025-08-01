/***********************************************************
 *                      BOARDPOSITION.H
 * 
 * Global coordinate class, contains a BoardPosition struct
 * for global use.
 * 
 * Default constructor for BoardPosition sets (x, y) as (-1, -1)
 * as an 'invalid' position. 
 * 
 * Board coordinates are defined using a zero-index notation
 * with the bottom-left coordinate defined as 0,0 and the top-right
 * coordinate defined as 7-7, where white is always defined to be
 * along the bottom. 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES

// EXTERNAL INCLUDES
#include <queue>
#include <utility>
#include <cmath>


enum RELPOS
{
    OUTOFLINE, SAME, LEFT, LEFTUP, UP, RIGHTUP, RIGHT, RIGHTDOWN, DOWN, LEFTDOWN
};


struct BoardPosition
{
    int x;
    int y;

    // Constructors
    BoardPosition() : x(-1), y(-1) {};
    BoardPosition(int i, int j) : x(i), y(j) {};

    // Admin
    void resetPosition();
    void position(BoardPosition newPos);
    void position(int i, int j);

    // Const methods
    bool validPosition() const; 
    bool validUpdate(int i, int j) const;
    bool validUpdate(BoardPosition newPos) const;
    
    BoardPosition returnIncrement(RELPOS relpos) const;
    BoardPosition returnUpdate(int i, int j) const;

    // Operator overloads
    bool operator == (const BoardPosition& rhs);
    bool operator != (const BoardPosition& rhs);
    
    // Static methods
    static RELPOS returnRelPos(BoardPosition curPos, BoardPosition tarPos);
    static std::queue<BoardPosition> returnPositionQueue();
    static std::vector<BoardPosition> returnRelativePositions(BoardPosition curPos, BoardPosition tarPos);
};