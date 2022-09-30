/***********************************************************
 *                      PIECE.H
 * 
 *  Class to handle all pieces
 * 
 * 
 ***********************************************************/
#pragma once

// INTERNAL INCLUDES
#include "Coords.h"

// EXTERNAL INCLUDES
#include <string>
#include <vector>
#include <list>

enum PIECE_COLOUR {
    WHITE = 0,
    BLACK = 1
};

struct PieceDescriptor {

    std::string filePath;
    std::string Piece_type;
};


class Piece 
{
    protected:
        static std::vector<int> Inst_list;            // Global ID list
        static int Piece_Count;                     // Global piece count
        int Inst_ID;                               // Instance specific ID
        const PieceDescriptor& descriptor;          // Sub-class specific descriptor
        const PIECE_COLOUR col;
        Coords pos;
        
        

    public:
        Piece(const PieceDescriptor& descriptor_, PIECE_COLOUR col_, int x, int y) : descriptor(descriptor_), col(col_), pos(x, y) {
            Piece_Count += 1;
            Inst_ID = Inst_list.size();
            Inst_list.push_back(Inst_ID); 
        };                    
        virtual ~Piece() {
            Piece_Count -= 1; 
        }

        int Get_Count();
        int Get_ID();
        PIECE_COLOUR Get_Colour();
        std::string Get_FilePath();
        bool updatePosition(int TAR_x, int TAR_y); 
        bool updatePosition(BoardPosition pos); 
        static void OutputPieceList();
        
};



/***********************************************************
 *                   PIECE SUB-CLASSES
 ***********************************************************/

class Pawn : public Piece
{
    private:
        static PieceDescriptor pawn_description; 
    public:
        Pawn(PIECE_COLOUR col_, int x, int y) : Piece(pawn_description, col_, x, y) {};
};

class Rook : public Piece
{
    private:
        static PieceDescriptor rook_description; 
    public:
        Rook(PIECE_COLOUR col_, int x, int y) : Piece(rook_description, col_, x, y){};
};

class Knight : public Piece
{
    private:
        static PieceDescriptor knight_description; 
    public:
        Knight(PIECE_COLOUR col_, int x, int y) : Piece(knight_description, col_, x, y) {};
};

class Bishop : public Piece
{
    private:
        static PieceDescriptor bishop_description; 
    public:
        Bishop(PIECE_COLOUR col_, int x, int y) : Piece(bishop_description, col_, x, y) {};
};

class Queen : public Piece
{
    private:
        static PieceDescriptor queen_description; 
    public:
        Queen(PIECE_COLOUR col_, int x, int y) : Piece(queen_description, col_, x, y) {};
};

class King : public Piece
{
    private:
        static PieceDescriptor king_description; 
    public:
        King(PIECE_COLOUR col_, int x, int y) : Piece(king_description, col_, x, y) {};
};
