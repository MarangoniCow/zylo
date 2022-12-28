/***********************************************************
 *              BOARDSTATE IMPLEMENTATION
 ***********************************************************/
// INTERNAL INCLUDES
#include "BoardState.h"
#include "Piece.h"

/***************** INITIALISATION METHODS *****************/
void BoardState::initialiseBoard()
{
    // Make sure board is clear
    resetBoard();

    // Add pawns
    for(int i = 0; i < 8; i++) {
        addPiece(PAWN, WHITE, BoardPosition(i, 1));
        addPiece(PAWN, BLACK, BoardPosition(i, 6));
    }

    // Add other pieces
    addPiece(ROOK, WHITE, BoardPosition(0, 0));
    addPiece(ROOK, WHITE, BoardPosition(7, 0));
    addPiece(ROOK, BLACK, BoardPosition(0, 7));
    addPiece(ROOK, BLACK, BoardPosition(7, 7));

    addPiece(KNIGHT, WHITE, BoardPosition(1, 0));
    addPiece(KNIGHT, WHITE, BoardPosition(6, 0));
    addPiece(KNIGHT, BLACK, BoardPosition(1, 7));
    addPiece(KNIGHT, BLACK, BoardPosition(6, 7));

    addPiece(BISHOP, WHITE, BoardPosition(2, 0));
    addPiece(BISHOP, WHITE, BoardPosition(5, 0));
    addPiece(BISHOP, BLACK, BoardPosition(2, 7));
    addPiece(BISHOP, BLACK, BoardPosition(5, 7));

    addPiece(QUEEN, WHITE, BoardPosition(3, 0));
    addPiece(QUEEN, BLACK, BoardPosition(3, 7));

    addPiece(KING, WHITE, BoardPosition(4, 0));
    addPiece(KING, BLACK, BoardPosition(4, 7));  
}

void BoardState::resetBoard()
{
    // Remove all pieces
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            removePiece(BoardPosition(i, j));
        }
    }

    // Reset turns and positions
    m_turn = WHITE;
    m_lastMove.first.resetPosition();
    m_lastMove.second.resetPosition();
}

void BoardState::promotePiece(BoardPosition pos, PIECE_TYPE newType)
{
    current[pos.x][pos.y].type(newType);
}

/******************** MAINTAINENCE METHODS *******************/
void BoardState::addPiece(PIECE_TYPE type, COLOUR col, BoardPosition pos)
{
    current[pos.x][pos.y] = Piece(type, col, pos.x, pos.y);
}

void BoardState::removePiece(BoardPosition pos)
{
    current[pos.x][pos.y].resetFlags();
}


void
BoardState::movePiece(Piece& piece, const BoardPosition& newPos)
{
	// Initialise
	BoardPosition	oldPos = piece.position();

	piece.moved(true);
	piece.position(newPos);

    current[newPos.x][newPos.y] = piece;
	removePiece(oldPos);
}

void
BoardState::movePiece(Move move)
{
	Piece piece = current[move.first.x][move.first.y];
    movePiece(piece, move.second);
}


/******************** BOOL METHODS *******************/
 bool BoardState::pieceExists (BoardPosition pos) const
 {
    return current[pos.x][pos.y].type() != NONE;
 }
 bool BoardState::pieceExists (BoardPosition pos, COLOUR col) const
 {
    return      ( current[pos.x][pos.y].type()  != NONE)
            &&  ( current[pos.x][pos.y].colour() == col);
 }
 bool BoardState::pieceExists (BoardPosition pos, COLOUR col, PIECE_TYPE type) const
 {
    return      ( current[pos.x][pos.y].type()  == type)
            &&  ( current[pos.x][pos.y].colour() == col);
 }

