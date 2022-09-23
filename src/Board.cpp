#include "Board.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "SDL_Board.h"
#include "Piece.h"



void Board::renderCurrentState()
{

    
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {

            Piece* current = boardState[i][j];
            if (current != NULL)
                gameBoard->LoadTexture(i, j, current->Get_FilePath());
        }
    }
    
            
}

void Board::initialiseBoard()
{
  
    static Pawn pw1(WHITE, 0, 1), pw2(WHITE, 1, 1), pw3(WHITE, 2, 1), pw4(WHITE, 3, 1), pw5(WHITE, 4, 1), pw6(WHITE, 5, 1), pw7(WHITE, 6, 1), pw8(WHITE, 7, 1);
    static Pawn pb1(BLACK, 0, 6), pb2(BLACK, 1, 6), pb3(BLACK, 2, 6), pb4(BLACK, 3, 6), pb5(BLACK, 4, 6), pb6(BLACK, 5, 6), pb7(BLACK, 6, 6), pb8(BLACK, 7, 6);

    static Rook rw1(WHITE, 0, 0), rw2(WHITE, 7, 0), rb1(BLACK, 0, 7), rb2(BLACK, 7, 7);
    static Knight kw1(WHITE, 1, 0), kw2(WHITE, 6, 0), kb1(BLACK, 1, 7), kb2(BLACK, 6, 7);
    static Bishop bw1(WHITE, 2, 0), bw2(WHITE, 5, 0), bb1(BLACK, 2, 7), bb2(BLACK, 5, 7);
    static Queen qw1(WHITE, 3, 0), qb1(BLACK, 3, 7);
    static King kiw1(WHITE, 4, 0), kib1(BLACK, 4, 7);

    

    boardState[0][0] = &rw1;
    boardState[1][0] = &kw1;
    boardState[2][0] = &bw1;
    boardState[3][0] = &qw1;
    boardState[4][0] = &kiw1;
    boardState[5][0] = &bw2;
    boardState[6][0] = &kw2;
    boardState[7][0] = &rw2;   

    boardState[0][1] = &pw1;
    boardState[1][1] = &pw2;
    boardState[2][1] = &pw3;
    boardState[3][1] = &pw4;
    boardState[4][1] = &pw5;
    boardState[5][1] = &pw6;
    boardState[6][1] = &pw7;
    boardState[7][1] = &pw8;

    boardState[0][7] = &rb1;
    boardState[1][7] = &kb1;
    boardState[2][7] = &bb1;
    boardState[3][7] = &qb1;
    boardState[4][7] = &kib1;
    boardState[5][7] = &bb2;
    boardState[6][7] = &kb2;
    boardState[7][7] = &rb2;

    boardState[0][6] = &pb1;
    boardState[1][6] = &pb2;
    boardState[2][6] = &pb3;
    boardState[3][6] = &pb4;
    boardState[4][6] = &pb5;
    boardState[5][6] = &pb6;
    boardState[6][6] = &pb7;
    boardState[7][6] = &pb8;
    
    
}

void Board::movePiece(BoardPosition oldPos, BoardPosition newPos)
{

    // Determine the piece
    Piece* currentPiece = boardState[oldPos.x][oldPos.y];

    if (currentPiece == NULL)
        return;
    
    // Check for a piece in the new position
    Piece* targetPiece = boardState[newPos.x][newPos.y];

    /* IF VALID: ADD IN LATER */

    // Update old position
    boardState[oldPos.x][oldPos.y] = NULL;
    // Update the new position
    boardState[newPos.x][newPos.y] = currentPiece; 

    // Update the piece position
    currentPiece->updatePosition(newPos); 

    /* DELETE OLD PIECE IF REQUIRED */

    

    // Clear current square and new square
    gameBoard->ClearSquare(oldPos.x, oldPos.y);

    if(targetPiece != NULL)
        gameBoard->ClearSquare(newPos.x, newPos.y);

    // Render piece into new position.
    gameBoard->LoadTexture(newPos.x, newPos.y, currentPiece->Get_FilePath());

}