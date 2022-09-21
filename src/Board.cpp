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

    // Populate vector: PAWNS
    pieceList.push_back(pw1);
    pieceList.push_back(pw2);
    pieceList.push_back(pw3);
    pieceList.push_back(pw4);
    pieceList.push_back(pw5);
    pieceList.push_back(pw6);
    pieceList.push_back(pw7);
    pieceList.push_back(pw8);
    pieceList.push_back(pb1);
    pieceList.push_back(pb2);
    pieceList.push_back(pb3);
    pieceList.push_back(pb4);
    pieceList.push_back(pb5);
    pieceList.push_back(pb6);
    pieceList.push_back(pb7);
    pieceList.push_back(pb8);

    // Populate vector: ROOKS, KNIGHTS, BISHOPS, KINGS, QUEENS
    pieceList.push_back(rw1);
    pieceList.push_back(rw2);
    pieceList.push_back(rb1);
    pieceList.push_back(rb2);

    pieceList.push_back(kw1);
    pieceList.push_back(kw2);
    pieceList.push_back(kb1);
    pieceList.push_back(kb2);

    pieceList.push_back(rw1);
    pieceList.push_back(rw2);
    pieceList.push_back(rb1);
    pieceList.push_back(rb2);

    pieceList.push_back(bw1);
    pieceList.push_back(bw2);
    pieceList.push_back(bb1);
    pieceList.push_back(bb2);

    pieceList.push_back(qw1);
    pieceList.push_back(qb1);
    pieceList.push_back(kiw1);
    pieceList.push_back(kib1);


    // Load pawns
    for(int i = 0; i < 8; i++ )
    {
        gameBoard->LoadTexture(1, i, pw1.Get_FilePath().c_str());
        gameBoard->LoadTexture(6, i, pb1.Get_FilePath().c_str());
    }

    // Load rooks
    gameBoard->LoadTexture(0, 0, rw1.Get_FilePath().c_str());
    gameBoard->LoadTexture(0, 7, rw2.Get_FilePath().c_str());
    gameBoard->LoadTexture(7, 0, rb1.Get_FilePath().c_str());
    gameBoard->LoadTexture(7, 7, rb2.Get_FilePath().c_str());
    
    // Load knights
    gameBoard->LoadTexture(0, 1, kw1.Get_FilePath().c_str());
    gameBoard->LoadTexture(0, 6, kw2.Get_FilePath().c_str());
    gameBoard->LoadTexture(7, 1, kb1.Get_FilePath().c_str());
    gameBoard->LoadTexture(7, 6, kb2.Get_FilePath().c_str());

    // Load bishops
    gameBoard->LoadTexture(0, 2, bw1.Get_FilePath().c_str());
    gameBoard->LoadTexture(0, 5, bw2.Get_FilePath().c_str());
    gameBoard->LoadTexture(7, 2, bb1.Get_FilePath().c_str());
    gameBoard->LoadTexture(7, 5, bb2.Get_FilePath().c_str());

    // Load queens
    gameBoard->LoadTexture(0, 4, qw1.Get_FilePath().c_str());
    gameBoard->LoadTexture(7, 4, qb1.Get_FilePath().c_str());

    // Load kings
    gameBoard->LoadTexture(0, 3, kiw1.Get_FilePath().c_str());
    gameBoard->LoadTexture(7, 3, kib1.Get_FilePath().c_str());

    
        


    
}

// char Board::Board_coordinates = {{'A1', 'B'


