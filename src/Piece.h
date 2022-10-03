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
#include <queue>

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
        // STATIC MEMBERS FOR GLOBAL FUNCTIONALITY
        static std::vector<int> Piece_Vector;          // Global ID list
        static int Piece_Count;                        // Global piece count

        // INSTANCE SPECIFIC FUNCTIONALITY
        int Piece_ID;                                   // Unique Piece ID
        const PIECE_COLOUR col;                         // Enumerated colour
        bool flag_move;                                  // Movement flag
        Coords pos;                                     // Current position

        // CLASS SPECFIC FUNCTIONALITY
        const PieceDescriptor& descriptor;              // Sub-class specific descriptor
        
    public:
        Piece(const PieceDescriptor& descriptor_, PIECE_COLOUR col_, int x, int y) : descriptor(descriptor_), col(col_), pos(x, y) {
            Piece_Count += 1;
            Piece_ID = Piece_Vector.size();
            Piece_Vector.push_back(Piece_ID); 
            flag_move = 0; 
        };                    
        virtual ~Piece() {
            Piece_Count -= 1; 
        }

        // STATIC METHODS FOR GLOBAL FUNCTIONALITY
        static int Get_Count();
        static void OutputPieceList();

        // GETTER METHODS        
        int Get_ID();
        PIECE_COLOUR Get_Colour();
        std::string Get_FilePath();

        // SETTER METHODS
        void updatePosition(BoardPosition pos); 
        
        // FUNCTIONALITY METHODS
        virtual std::queue<BoardPosition> moveRange() = 0; 


        
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
        std::queue<BoardPosition> moveRange();
};

class Rook : public Piece
{
    private:
        static PieceDescriptor rook_description; 
    public:
        Rook(PIECE_COLOUR col_, int x, int y) : Piece(rook_description, col_, x, y){};
        std::queue<BoardPosition> moveRange();
};

class Knight : public Piece
{
    private:
        static PieceDescriptor knight_description; 
    public:
        Knight(PIECE_COLOUR col_, int x, int y) : Piece(knight_description, col_, x, y) {};
        std::queue<BoardPosition> moveRange();
};

class Bishop : public Piece
{
    private:
        static PieceDescriptor bishop_description; 
    public:
        Bishop(PIECE_COLOUR col_, int x, int y) : Piece(bishop_description, col_, x, y) {};
        std::queue<BoardPosition> moveRange();
};

class Queen : public Piece
{
    private:
        static PieceDescriptor queen_description; 
    public:
        Queen(PIECE_COLOUR col_, int x, int y) : Piece(queen_description, col_, x, y) {};
        std::queue<BoardPosition> moveRange();
};

class King : public Piece
{
    private:
        static PieceDescriptor king_description; 
    public:
        King(PIECE_COLOUR col_, int x, int y) : Piece(king_description, col_, x, y) {};
        std::queue<BoardPosition> moveRange();
};