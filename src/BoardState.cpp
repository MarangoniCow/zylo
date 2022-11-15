/***********************************************************
 *              BOARDSTATE IMPLEMENTATION
 ***********************************************************/
// INTERNAL INCLUDES
#include "BoardState.h"
#include "Piece.h"

// EXTERNAL INCLUDES
#include <iostream>
#include <queue>

/***************** INITIALISATION METHODS *****************/
void BoardState::initialiseBoard()
{
    // Make sure board is clear
    resetBoard();
    

    
    

    // Initialise all 32 pieces with correct positions
    
    addPiece(PAWN, WHITE, 0, 1);
    addPiece(PAWN, WHITE, 1, 1);
    addPiece(PAWN, WHITE, 2, 1);
    addPiece(PAWN, WHITE, 3, 1);
    addPiece(PAWN, WHITE, 4, 1);
    addPiece(PAWN, WHITE, 5, 1);
    addPiece(PAWN, WHITE, 6, 1);
    addPiece(PAWN, WHITE, 7, 1);

    addPiece(PAWN, BLACK, 0, 6);
    addPiece(PAWN, BLACK, 1, 6);
    addPiece(PAWN, BLACK, 2, 6);
    addPiece(PAWN, BLACK, 3, 6);
    addPiece(PAWN, BLACK, 4, 6);
    addPiece(PAWN, BLACK, 5, 6);
    addPiece(PAWN, BLACK, 6, 6);
    addPiece(PAWN, BLACK, 7, 6);

    addPiece(ROOK, WHITE, 0, 0);
    addPiece(ROOK, WHITE, 7, 0);
    addPiece(ROOK, BLACK, 0, 7);
    addPiece(ROOK, BLACK, 7, 7);

    addPiece(KNIGHT, WHITE, 1, 0);
    addPiece(KNIGHT, WHITE, 6, 0);
    addPiece(KNIGHT, BLACK, 1, 7);
    addPiece(KNIGHT, BLACK, 6, 7);

    addPiece(BISHOP, WHITE, 2, 0);
    addPiece(BISHOP, WHITE, 5, 0);
    addPiece(BISHOP, BLACK, 2, 7);
    addPiece(BISHOP, BLACK, 5, 7);

    addPiece(QUEEN, WHITE, 3, 0);
    addPiece(QUEEN, BLACK, 3, 7);

    addPiece(KING, WHITE, 4, 0);
    addPiece(KING, BLACK, 4, 7);  
}

/******************** UPDATE METHODS **********************/
Piece* BoardState::returnIDPtr(PIECE_ID ID)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Piece* curPiece = current[i][j];
            if(curPiece != NULL && curPiece->ID() == ID)
                return curPiece;
        }
    }
    return NULL;
}

void BoardState::promotePiece(PIECE_ID ID, PIECE_TYPE newType)
{
    Piece* currentPiece = returnIDPtr(ID);
    PIECE_COLOUR col = currentPiece->colour();
    BoardPosition pos(currentPiece->x(), currentPiece->y());
    removePiece(currentPiece);


    switch(newType)
    {
        case PAWN:
        {
            addPiece(PAWN, col, pos.x, pos.y);
            break;
        }
        case ROOK:
        {
            addPiece(ROOK, col, pos.x, pos.y);
            break;
        }
        case KNIGHT:
        {
            addPiece(KNIGHT, col, pos.x, pos.y);
            break;

        }
        case BISHOP:
        {
            addPiece(BISHOP, col, pos.x, pos.y);
            break;
        }
        case QUEEN:
        {
            addPiece(QUEEN, col, pos.x, pos.y);
            break;
        }
        case KING:
        {
            addPiece(KING, col, pos.x, pos.y);
            break;
        }
        default:
        {break;};
    }  
}

/****************** MAINTAINENCE METHODS ******************/
void BoardState::resetBoard()
{
    // Remove all pieces
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Piece* currentPiece = current[i][j];
            if(currentPiece != NULL)
                removePiece(currentPiece);
        }
    }

    // Reset turns and positions
    currentTurn = WHITE;
    BoardPosition pos;
    m_lastMove.first = pos; 
    m_lastMove.second = pos;
}

void BoardState::addPiece(Piece* piece)
{

    if(piece == NULL) {
        std::cout << "Cannot add piece: nullptr" << std::endl;
        return;
    }

    BoardPosition pos(piece->x(), piece->y());
    if(pos.validPosition())
        current[pos.x][pos.y] = piece;
    else
        std::cout << "Cannot add piece: invalid position" << std::endl;
}

void BoardState::addPiece(PIECE_TYPE type, PIECE_COLOUR col, int x, int y)
{
    Piece* p;
    // Check position here
    switch (type)
    {
        case PAWN: {
            p = new Pawn(col, x, y);
            break;
        }
        case ROOK: {
            p = new Rook(col, x, y);
            break;
        }
        case KNIGHT: {
            p = new Knight(col, x, y);
            break;
        }
        case BISHOP: {
            p =  new Bishop(col, x, y);
            break;
        }
        case QUEEN: {
            p = new Queen(col, x, y);
            break;
        }
        case KING: {
            p = new King(col, x, y);
            break;
        }
        default: {};   
    }
    current[x][y] = p;
}

void BoardState::removePiece(Piece* pieceToDelete)
{
    if(!pieceExists(pieceToDelete))
        return;
    // Get current position
    BoardPosition pos(pieceToDelete->x(), pieceToDelete->y());

    // Update board state to null
    current[pos.x][pos.y] = NULL;

    // Delete the piece, destructor will take care of the rest
    delete(pieceToDelete);
}
void BoardState::removePiece(PIECE_ID pieceToDelete)
{
    removePiece(returnIDPtr(pieceToDelete));
}

/******************** BOOL CHECKS**************************/
bool BoardState::pieceExists(Piece* piece) const {
    return (piece != NULL);
}
bool BoardState::pieceExists(Piece* piece, PIECE_COLOUR col) const {
    return (pieceExists(piece) && piece->colour() == col);
}
bool BoardState::pieceExists(Piece* piece, PIECE_COLOUR col, PIECE_TYPE type) const {
    return (pieceExists(piece, col) && piece->type() == type);
}

bool BoardState::pieceExists(BoardPosition pos) const {
    return (current[pos.x][pos.y] != NULL);
}
bool BoardState::pieceExists(BoardPosition pos, PIECE_COLOUR col) const {
    return pieceExists(pos) && current[pos.x][pos.y]->colour() == col;
}
bool BoardState::pieceExists(BoardPosition pos, PIECE_COLOUR col, PIECE_TYPE type) const {
    return (pieceExists(pos, col) && current[pos.x][pos.y]->type() == type);
}


/********************** RETURNS ***************************/
void
BoardState::getPieceQueue(PieceQueue &pieceQueue) const
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(current[i][j] != NULL)
                pieceQueue.push(current[i][j]);
        }
    }
}

PieceQueue BoardState::returnPieceQueue() const
{
    PieceQueue pieceQueue;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(current[i][j] != NULL)
                pieceQueue.push(current[i][j]);
        }
    }
    return pieceQueue;
}
PieceQueue BoardState::returnPieceQueue(PIECE_COLOUR col) const
{
    PieceQueue pieceQueue;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            BoardPosition pos(i, j);
            if(pieceExists(pos, col))
                pieceQueue.push(current[i][j]);
        }
    }
    return pieceQueue;
}
PieceQueue BoardState::returnPieceQueue(PIECE_COLOUR col, PIECE_TYPE type) const
{
    PieceQueue pieceQueue;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            BoardPosition pos(i, j);
            if(pieceExists(pos, col, type))
                pieceQueue.push(current[i][j]);
        }
    }

    return pieceQueue;
}


Piece* BoardState::returnPiece(BoardPosition pos) const
{
    return current[pos.x][pos.y];
}

