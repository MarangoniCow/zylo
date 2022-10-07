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
   

    state.piecesCurr[0][0] = &rw1;
    state.piecesCurr[1][0] = &kw1;
    state.piecesCurr[2][0] = &bw1;
    state.piecesCurr[3][0] = &qw1;
    state.piecesCurr[4][0] = &kiw1;
    state.piecesCurr[5][0] = &bw2;
    state.piecesCurr[6][0] = &kw2;
    state.piecesCurr[7][0] = &rw2;   

    state.piecesCurr[0][1] = &pw1;
    state.piecesCurr[1][1] = &pw2;
    state.piecesCurr[2][1] = &pw3;
    state.piecesCurr[3][1] = &pw4;
    state.piecesCurr[4][1] = &pw5;
    state.piecesCurr[5][1] = &pw6;
    state.piecesCurr[6][1] = &pw7;
    state.piecesCurr[7][1] = &pw8;

    state.piecesCurr[0][7] = &rb1;
    state.piecesCurr[1][7] = &kb1;
    state.piecesCurr[2][7] = &bb1;
    state.piecesCurr[3][7] = &qb1;
    state.piecesCurr[4][7] = &kib1;
    state.piecesCurr[5][7] = &bb2;
    state.piecesCurr[6][7] = &kb2;
    state.piecesCurr[7][7] = &rb2;

    state.piecesCurr[0][6] = &pb1;
    state.piecesCurr[1][6] = &pb2;
    state.piecesCurr[2][6] = &pb3;
    state.piecesCurr[3][6] = &pb4;
    state.piecesCurr[4][6] = &pb5;
    state.piecesCurr[5][6] = &pb6;
    state.piecesCurr[6][6] = &pb7;
    state.piecesCurr[7][6] = &pb8;
    
    
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

    if(targetPiece != NULL && (targetPiece->Get_Colour() == currentPiece->Get_Colour())) 
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
    switch(currentPiece->Get_Descriptor().type) {
        case PAWN:
        {
            if(currentPiece->Get_Colour() == WHITE)
            {
                // CHECK FOR PIECES TO TAKE
                for(int i = -1; i <= 1; i += 2) {
                    if(currPos.validUpdate(i, 1))
                    {
                        BoardPosition temp = currPos.returnUpdate(i, 1);
                        Piece* targetPiece = state.piecesCurr[temp.x][temp.y];
                        
                        if(targetPiece != NULL && targetPiece->Get_Colour() == BLACK)
                            moveQueue.push(temp);
                    }
                }
                
                // CHECK FOR EN-PASSANT
                if(currPos.y == 4)
                {
                    Piece* targetPawn;
                    // LEFT MOVEMENT: CHECK FOR A PAWN & PREVIOUS MOVE

                    for(int i = -1; i <= 1; i += 2) {
                        if(currPos.validUpdate(i, 0)) 
                        {
                            targetPawn = state.piecesCurr[currPos.x + i][4];
                        }
                        else
                            continue;
                    
                        bool pawnAdjacent = (targetPawn != NULL && targetPawn->Get_Descriptor().type == PAWN && targetPawn->Get_Colour() == BLACK) ? 1 : 0;
                        bool adjacentLastMove = (targetPawn == state.piecesPrev[currPos.x + i][4]) ? 1 : 0;
                    
                        if(pawnAdjacent && adjacentLastMove)
                            moveQueue.push(currPos.returnUpdate(i, 1));
                    }
                }
            }
            else
            {
                for(int i = -1; i <= 1; i += 2)
                {
                    // CHECK FOR PIECES TO TAKE
                    if(currPos.validUpdate(i, -1))
                    {
                        BoardPosition temp = currPos.returnUpdate(i, -1);
                        Piece* targetPiece = state.piecesCurr[temp.x][temp.y];
                        
                        if(targetPiece != NULL && targetPiece->Get_Colour() == BLACK)
                            moveQueue.push(temp);
                    }

                    Piece* temp;
                    // CHECK FOR EN-PASSANT
                    if(currPos.validUpdate(i, 0)) 
                    {
                        temp = state.piecesCurr[currPos.x + i][3];
                    }
                    else
                        continue;
                
                    bool pawnAdjacent = (temp != NULL && temp->Get_Descriptor().type == PAWN && temp->Get_Colour() == WHITE) ? 1 : 0;
                
                    if(pawnAdjacent)
                        moveQueue.push(currPos.returnUpdate(i, -1));
                }
            }
        }
        case KING:
        {
            if(!currentPiece->hasMoved())
            {
                Piece* targetRookRight = state.piecesCurr[7][currPos.y];
                Piece* targetRookLeft  = state.piecesCurr[0][currPos.y];

                if(targetRookRight->Get_Descriptor().type == ROOK && !targetRookRight->hasMoved() && targetRookRight->Get_Colour() == currentPiece->Get_Colour())
                    moveQueue.push(currPos.returnUpdate(2, 0));
                if(targetRookLeft->Get_Descriptor().type == ROOK && !targetRookLeft->hasMoved() && targetRookLeft->Get_Colour() == currentPiece->Get_Colour())
                    moveQueue.push(currPos.returnUpdate(-2, 0));
            }
        }
        default:
        {};
    }

    return moveQueue;
        

}