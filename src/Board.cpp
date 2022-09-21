#include "Board.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "SDL_Board.h"
#include "Piece.h"


void Board::initialiseBoard()
{

    Pawn pw1(WHITE), pw2(WHITE), pw3(WHITE), pw4(WHITE), pw5(WHITE), pw6(WHITE), pw7(WHITE), pw8(WHITE);
    Pawn pb1(BLACK), pb2(BLACK), pb3(BLACK), pb4(BLACK), pb5(BLACK), pb6(BLACK), pb7(BLACK), pb8(BLACK);

    Rook rw1(WHITE), rw2(WHITE), rb1(BLACK), rb2(BLACK);
    Knight kw1(WHITE), kw2(WHITE), kb1(BLACK), kb2(BLACK);
    Bishop bw1(WHITE), bw2(WHITE), bb1(BLACK), bb2(BLACK);
    Queen qw1(WHITE), qb1(BLACK);
    King kiw1(WHITE), kib1(BLACK); 

    // Initialise boardState
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            boardState[i][j] = -1;

    // Update board state with piece ID
    




    // Load pawns
    for(int i = 0; i < 8; i++ )
    {
        gameBoard->LoadTexture(i, 1, pw1.Get_FilePath().c_str());
        gameBoard->LoadTexture(i, 6, pb1.Get_FilePath().c_str());
    }

    // Load rooks
    gameBoard->LoadTexture(0, 0, rw1.Get_FilePath().c_str());
    gameBoard->LoadTexture(7, 0, rw2.Get_FilePath().c_str());
    gameBoard->LoadTexture(0, 7, rb1.Get_FilePath().c_str());
    gameBoard->LoadTexture(7, 7, rb2.Get_FilePath().c_str());
    
    // Load knights
    gameBoard->LoadTexture(1, 0, kw1.Get_FilePath().c_str());
    gameBoard->LoadTexture(6, 0, kw2.Get_FilePath().c_str());
    gameBoard->LoadTexture(1, 7, kb1.Get_FilePath().c_str());
    gameBoard->LoadTexture(6, 7, kb2.Get_FilePath().c_str());

    // Load bishops
    gameBoard->LoadTexture(2, 0, bw1.Get_FilePath().c_str());
    gameBoard->LoadTexture(5, 0, bw2.Get_FilePath().c_str());
    gameBoard->LoadTexture(2, 7, bb1.Get_FilePath().c_str());
    gameBoard->LoadTexture(5, 7, bb2.Get_FilePath().c_str());

    // Load queens
    gameBoard->LoadTexture(3, 0, qw1.Get_FilePath().c_str());
    gameBoard->LoadTexture(3, 7, qb1.Get_FilePath().c_str());

    // Load kings
    gameBoard->LoadTexture(4, 0, kiw1.Get_FilePath().c_str());
    gameBoard->LoadTexture(4, 7, kib1.Get_FilePath().c_str());

    
        


    
}

// char Board::Board_coordinates = {{'A1', 'B'


