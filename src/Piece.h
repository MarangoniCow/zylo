/***********************************************************
 *                      PIECE.H
 * 
 *  Class to handle all pieces
 * 
 * 
 ***********************************************************/
#pragma once

// INTERNAL INCLUDES

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
};


class Piece 
{
    protected:
        static std::vector<int> ID_list;            // Global ID list
        static int Piece_Count;                     // Global piece count
        int Piece_ID;                               // Instance specific ID
        const PieceDescriptor& descriptor;          // Sub-class specific descriptor
        const PIECE_COLOUR col;
        

    public:
        Piece(const PieceDescriptor& descriptor_, PIECE_COLOUR col_) : descriptor(descriptor_), col(col_) {
            Piece_Count += 1;
            Piece_ID = ID_list.size();
            ID_list.push_back(Piece_ID); 
        };                    
        virtual ~Piece() {
            Piece_Count -= 1; 
        }

        int Get_Count();
        int Get_ID();
        PIECE_COLOUR Get_Colour();
        std::string Get_FilePath();
        
};



/***********************************************************
 *                   PIECE SUB-CLASSES
 ***********************************************************/

class Pawn : public Piece
{
    private:
        static PieceDescriptor pawn_description; 
    public:
        Pawn(PIECE_COLOUR col_) : Piece(pawn_description, col_) {};
};

class Rook : public Piece
{
    private:
        static PieceDescriptor rook_description; 
    public:
        Rook(PIECE_COLOUR col_) : Piece(rook_description, col_){};
};

class Knight : public Piece
{
    private:
        static PieceDescriptor knight_description; 
    public:
        Knight(PIECE_COLOUR col_) : Piece(knight_description, col_) {};
};

class Bishop : public Piece
{
    private:
        static PieceDescriptor bishop_description; 
    public:
        Bishop(PIECE_COLOUR col_) : Piece(bishop_description, col_) {};
};

class Queen : public Piece
{
    private:
        static PieceDescriptor queen_description; 
    public:
        Queen(PIECE_COLOUR col_) : Piece(queen_description, col_) {};
};

class King : public Piece
{
    private:
        static PieceDescriptor king_description; 
    public:
        King(PIECE_COLOUR col_) : Piece(king_description, col_) {};
};
