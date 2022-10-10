/***********************************************************
 *                      BOARD IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "Board.h"
#include "Piece.h"

// EXTERNAL INCLUDES
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <queue>


void Board::initialiseBoard()
{
  
    static Pawn pw1(WHITE, 0, 1), pw2(WHITE, 1, 1), pw3(WHITE, 2, 1), pw4(WHITE, 3, 1), pw5(WHITE, 4, 1), pw6(WHITE, 5, 1), pw7(WHITE, 6, 1), pw8(WHITE, 7, 1);
    static Pawn pb1(BLACK, 0, 6), pb2(BLACK, 1, 6), pb3(BLACK, 2, 6), pb4(BLACK, 3, 6), pb5(BLACK, 4, 6), pb6(BLACK, 5, 6), pb7(BLACK, 6, 6), pb8(BLACK, 7, 6);

    static Rook rw1(WHITE, 0, 0), rw2(WHITE, 7, 0), rb1(BLACK, 0, 7), rb2(BLACK, 7, 7);
    static Knight kw1(WHITE, 1, 0), kw2(WHITE, 6, 0), kb1(BLACK, 1, 7), kb2(BLACK, 6, 7);
    static Bishop bw1(WHITE, 2, 0), bw2(WHITE, 5, 0), bb1(BLACK, 2, 7), bb2(BLACK, 5, 7);
    static Queen qw1(WHITE, 3, 0), qb1(BLACK, 3, 7);
    static King kiw1(WHITE, 4, 0), kib1(BLACK, 4, 7);
   
    std::vector<Piece*> pieceList = Piece::returnInstanceList();
    for(auto it = pieceList.begin(); it != pieceList.end(); it++)
    {
        Piece* temp = *it;

        if(temp != NULL)
            state.piecesCurr[temp->returnPosition().x][temp->returnPosition().y] = temp;
    }
    
    
    
}

void Board::addPieceToState(Piece* piece)
{

    if(piece == NULL) {
        std::cout << "Cannot add piece: null ptr" << std::endl;
        return;
    }
        
    BoardPosition pos = piece->returnPosition();
    if(pos.validPosition())
        state.piecesCurr[pos.x][pos.y] = piece;
    else
        std::cout << "Cannot add piece: invalid position" << std::endl;
}

void Board::movePiece(BoardPosition currPos, BoardPosition tarPos)
{
    // Determine the target piece
    Piece* currentPiece = state.piecesCurr[currPos.x][currPos.y];
    if (currentPiece == NULL)
        return;
    
    // Check for valid move
    if (!validMove(currPos, tarPos))
        return;

    // Update old state
    memcpy(state.piecesPrev, state.piecesCurr, sizeof(state.piecesCurr));
  
    // Update current position in state as null
    state.piecesCurr[currPos.x][currPos.y] = NULL;
    // Update the target position in state with the current piece
    state.piecesCurr[tarPos.x][tarPos.y] = currentPiece; 

    // Update the piece position
    currentPiece->updatePosition(tarPos); 
}

bool Board::validMove(BoardPosition currPos, BoardPosition tarPos)
{
    
    std::queue<BoardPosition> moveQueue = generateValidMoves(currPos);
    
    // Determine the piece in the old position
    Piece* currentPiece = state.piecesCurr[currPos.x][currPos.y];

    // Check for empty queue
    if(moveQueue.empty())
        return 0;

    
    bool isValid = 0;
    
    // Iterate through queue
    while(!moveQueue.empty())
    {
        BoardPosition temp;
        temp = moveQueue.front();
        moveQueue.pop();

        // Check if requested move is in move list
        if (temp == tarPos) {
            isValid = 1;
            break;
        }
    }

    if(!isValid) return 0;

    
    
    // Check for a piece in the new position
    Piece* targetPiece = state.piecesCurr[tarPos.x][tarPos.y];

    if(targetPiece != NULL && (targetPiece->returnColour() == currentPiece->returnColour())) 
        return 0;
    else
        return 1;
    
}

std::queue<BoardPosition> Board::generateValidMoves(BoardPosition currPos)
{
    std::queue<BoardPosition> moveQueue;

    // Determine the piece in the old position
    Piece* currentPiece = state.piecesCurr[currPos.x][currPos.y];

    if (currentPiece == NULL)
        return moveQueue;

    // Generate move range
    moveQueue = currentPiece->moveRange();

    // Special moves list
    switch(currentPiece->returnDescriptor().type)
    {
        case PAWN:
        {
            // Initialise
            PIECE_COLOUR oppositeCol;
            int enpassantRow;
            int forward;

            if(currentPiece->returnColour() == WHITE)
            {
                oppositeCol = BLACK;
                enpassantRow = 4;
                forward = 1;               
            }
            else
            {
                oppositeCol = WHITE;
                enpassantRow = 3;
                forward = -1;                
            }

            // GENERATE TAKE & EN-PASSANT: For loop repeats move queue for left/right instructions
            for(int i = -1; i <= 1; i += 2)
            {
                // CHECK FOR TAKE: Check if we're at the edge or not
                if(currPos.validUpdate(i, forward))
                {
                    BoardPosition temp = currPos.returnUpdate(i, forward);
                    Piece* targetPiece = state.piecesCurr[temp.x][temp.y];
                    
                    if(targetPiece != NULL && targetPiece->returnColour() == oppositeCol)
                        moveQueue.push(temp);
                }

                    // CHECK FOR EN-PASSANT: Must be on the row adjacent to backpawn line
                if(currPos.y == enpassantRow && currPos.validUpdate(i, 0))
                {
                    Piece* targetPiece;
                    targetPiece = state.piecesCurr[currPos.x + i][enpassantRow];
                    
                    // Check for an existence piece, and that it's a pawn, and that it's the opposite colour (black)
                    bool pawnAdjacent = (targetPiece != NULL && targetPiece->returnDescriptor().type == PAWN && targetPiece->returnColour() == oppositeCol) ? 1 : 0;
                    // Check that if it was there last move (needs to be an updated move)
                    bool adjacentLastMove = (targetPiece == state.piecesPrev[currPos.x + i][enpassantRow]) ? 1 : 0;
                    
                    if(pawnAdjacent && !adjacentLastMove)
                        moveQueue.push(currPos.returnUpdate(i, forward));
                }
            }     
        }
        case KING:
        {
            if(!currentPiece->hasMoved())
            {
                Piece* targetRookRight = state.piecesCurr[7][currPos.y];
                Piece* targetRookLeft  = state.piecesCurr[0][currPos.y];

                if(targetRookRight->returnDescriptor().type == ROOK && !targetRookRight->hasMoved() && targetRookRight->returnColour() == currentPiece->returnColour())
                    moveQueue.push(currPos.returnUpdate(2, 0));
                if(targetRookLeft->returnDescriptor().type == ROOK && !targetRookLeft->hasMoved() && targetRookLeft->returnColour() == currentPiece->returnColour())
                    moveQueue.push(currPos.returnUpdate(-2, 0));
            }
        }
        default:
        {};
    }

    return moveQueue;
}