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
    clearBoard();
    // Initialise all 32 pieces with correct positions
    Pawn* pw2 = new Pawn(WHITE, 1, 1);
    Pawn* pw1 = new Pawn(WHITE, 0, 1);
    Pawn* pw3 = new Pawn(WHITE, 2, 1);
    Pawn* pw4 = new Pawn(WHITE, 3, 1);
    Pawn* pw5 = new Pawn(WHITE, 4, 1);
    Pawn* pw6 = new Pawn(WHITE, 5, 1);
    Pawn* pw7 = new Pawn(WHITE, 6, 1);
    Pawn* pw8 = new Pawn(WHITE, 7, 1);

    Pawn* pb1 = new Pawn(BLACK, 0, 6);
    Pawn* pb2 = new Pawn(BLACK, 1, 6);
    Pawn* pb3 = new Pawn(BLACK, 2, 6);
    Pawn* pb4 = new Pawn(BLACK, 3, 6);
    Pawn* pb5 = new Pawn(BLACK, 4, 6);
    Pawn* pb6 = new Pawn(BLACK, 5, 6);
    Pawn* pb7 = new Pawn(BLACK, 6, 6);
    Pawn* pb8 = new Pawn(BLACK, 7, 6);

    Rook* rw1 = new Rook(WHITE, 0, 0);
    Rook* rw2 = new Rook(WHITE, 7, 0);
    Rook* rb1 = new Rook(BLACK, 0, 7);
    Rook* rb2 = new Rook(BLACK, 7, 7);

    Knight* kw1 = new Knight(WHITE, 1, 0);
    Knight* kw2 = new Knight(WHITE, 6, 0);
    Knight* kb1 = new Knight(BLACK, 1, 7);
    Knight* kb2 = new Knight(BLACK, 6, 7);

    Bishop* bw1 = new Bishop(WHITE, 2, 0);
    Bishop* bw2 = new Bishop(WHITE, 5, 0);
    Bishop* bb1 = new Bishop(BLACK, 2, 7);
    Bishop* bb2 = new Bishop(BLACK, 5, 7);

    Queen* qw1 = new Queen(WHITE, 3, 0);
    Queen* qb1 = new Queen(BLACK, 3, 7);

    King* kiw1 = new King(WHITE, 4, 0);
    King* kib1 = new King(BLACK, 4, 7);

  
   // Return the list of pieces from Piece, iterate over it and update BoardState accordingly.
    std::vector<Piece*> pieceList = Piece::returnInstanceList();
    for(auto it = pieceList.begin(); it != pieceList.end(); it++)
    {
        Piece* temp = *it;

        if(temp != NULL)
            current[temp->returnPosition().x][temp->returnPosition().y] = temp;
    }
}

/******************** UPDATE METHODS **********************/
void BoardState::promotePiece(PIECE_ID ID, PIECE_TYPE newType)
{
    Piece* currentPiece = Piece::returnIDPtr(ID);
    PIECE_COLOUR col = currentPiece->returnColour();
    BoardPosition pos = currentPiece->returnPosition();

    switch(newType)
    {
        case PAWN:
        {
            addPiece(new Pawn(col, pos.x, pos.y, ID));
            break;
        }
        case ROOK:
        {
            addPiece(new Rook(col, pos.x, pos.y, ID));
            break;
        }
        case KNIGHT:
        {
            addPiece(new Knight(col, pos.x, pos.y, ID));
            break;

        }
        case BISHOP:
        {
            addPiece(new Bishop(col, pos.x, pos.y, ID));
            break;
        }
        case QUEEN:
        {
            addPiece(new Queen(col, pos.x, pos.y, ID));
            break;
        }
        case KING:
        {
            addPiece(new King(col, pos.x, pos.y, ID));
            break;
        }
        default:
        {break;};
    }  
}

/****************** MAINTAINENCE METHODS ******************/
void BoardState::clearBoard()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Piece* currentPiece = current[i][j];
            if(currentPiece != NULL)
                removePiece(currentPiece);
        }
    }
}

void BoardState::addPiece(Piece* piece)
{

    if(piece == NULL) {
        std::cout << "Cannot add piece: nullptr" << std::endl;
        return;
    }

    BoardPosition pos = piece->returnPosition();
    if(pos.validPosition())
        current[pos.x][pos.y] = piece;
    else
        std::cout << "Cannot add piece: invalid position" << std::endl;
}

void BoardState::removePiece(Piece* pieceToDelete)
{
    if(!pieceExists(pieceToDelete))
        return;
    // Get current position
    BoardPosition pos = pieceToDelete->returnPosition();

    // Update board state to null
    current[pos.x][pos.y] = NULL;

    // Delete the piece, destructor will take care of the rest
    delete(pieceToDelete);
}
void BoardState::removePiece(PIECE_ID pieceToDelete)
{
    removePiece(Piece::returnIDPtr(pieceToDelete));
}

/******************** BOOL CHECKS**************************/
bool BoardState::pieceExists(Piece* piece) {
    return (piece != NULL);
}
bool BoardState::pieceExists(Piece* piece, PIECE_COLOUR col) {
    return (pieceExists(piece) && piece->returnColour() == col);
}
bool BoardState::pieceExists(Piece* piece, PIECE_COLOUR col, PIECE_TYPE type) {
    return (pieceExists(piece, col) && piece->returnDescriptor().type == type);
}

bool BoardState::pieceExists(BoardPosition pos) {
    return (current[pos.x][pos.y] != NULL);
}
bool BoardState::pieceExists(BoardPosition pos, PIECE_COLOUR col) {
    return pieceExists(pos) && current[pos.x][pos.y]->returnColour() == col;
}
bool BoardState::pieceExists(BoardPosition pos, PIECE_COLOUR col, PIECE_TYPE type) {
    return (pieceExists(pos, col) && current[pos.x][pos.y]->returnDescriptor().type == type);
}


/********************** RETURNS ***************************/
PieceQueue BoardState::returnPieceQueue()
{
    PieceQueue pieceQueue;
    std::vector<Piece*> pieceVector = Piece::returnInstanceList();

    for(auto it = pieceVector.begin(); it != pieceVector.end(); it++) {
        Piece* piece = *it;
        if(pieceExists(piece)) pieceQueue.push(piece);
    }
    return pieceQueue;
}
PieceQueue BoardState::returnPieceQueue(PIECE_COLOUR col)
{
    PieceQueue pieceQueue;
    std::vector<Piece*> pieceVector = Piece::returnInstanceList();

    for(auto it = pieceVector.begin(); it != pieceVector.end(); it++) {
        Piece* piece = *it;
        if(pieceExists(piece, col)) pieceQueue.push(piece);
    }
    return pieceQueue;
}
PieceQueue BoardState::returnPieceQueue(PIECE_COLOUR col, PIECE_TYPE type)
{
    PieceQueue pieceQueue;
    std::vector<Piece*> pieceVector = Piece::returnInstanceList();

    for(auto it = pieceVector.begin(); it != pieceVector.end(); it++) {
        Piece* piece = *it;
        if(pieceExists(piece, col, type)) pieceQueue.push(piece);
    }
    return pieceQueue;
}


Piece* BoardState::returnPiece(BoardPosition pos)
{
    return current[pos.x][pos.y];
}

