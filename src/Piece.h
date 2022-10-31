/***********************************************************
 *                      PIECE.H
 * 
 *  Class to handle all pieces and movement ranges
 *      - file path
 *      - colour
 *      - piece type
 *      - Global piece list & ID
 * 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "BoardPosition.h"

// EXTERNAL INCLUDES
#include <string>
#include <vector>
#include <queue>


typedef unsigned int PIECE_ID;
typedef std::queue<BoardPosition> PositionQueue;

enum PIECE_COLOUR {
    WHITE = 0,
    BLACK = 1
};

enum PIECE_TYPE {
    PAWN = 0,
    ROOK = 1,
    KNIGHT = 2,
    BISHOP = 3,
    QUEEN = 4,
    KING = 5
};

/*      PIECE DESCRIPTOR
* A means to provide static data to subclasses.
* This is now redundant, as I've changed the implementation of graphic objects.
* However, I've not removed it yet because it's a cute method which I'd like to keep around :)
*/

struct PieceDescriptor {

    std::string bmpPath;        // REDUNDANT: TO BE REMOVED
    PIECE_TYPE type;
};


class Piece 
{
    protected:
        // STATIC MEMBERS FOR GLOBAL FUNCTIONALITY
        static std::vector<Piece*> Piece_instanceList;          // Global ID list
        static std::vector<Piece*> whitePieces;
        static std::vector<Piece*> blackPieces;
        static int Piece_count;                                 // Global piece count

        // CLASS SPECFIC FUNCTIONALITY
        const PieceDescriptor& descriptor;                      // Sub-class specific descriptor

        // INSTANCE SPECIFIC FUNCTIONALITY
        PIECE_ID ID;                                           // Unique Piece ID
        PIECE_COLOUR col;                                 // Enumerated colour
        PIECE_TYPE type;
        BoardPosition pos;                                     // Current position
        bool flag_move;                                         // Movement flag


        
    public:
        Piece(const PieceDescriptor& descriptor_, PIECE_COLOUR col_, int x, int y) : descriptor(descriptor_), col(col_), pos(x, y) {
            // Assert global methodology
            Piece_count += 1;
            ID = Piece_instanceList.size();
            Piece_instanceList.push_back(this); 
            flag_move = 0;
            type = descriptor.type;
        };
        Piece(const PieceDescriptor& descriptor_, PIECE_COLOUR col_, int x, int y, PIECE_ID ID_) : descriptor(descriptor_), col(col_), pos(x, y), ID(ID_) {
            Piece_instanceList[ID_] = this;
            flag_move = 1; 
            type = descriptor.type;
        };
        virtual ~Piece() {
            Piece_count -= 1;
            Piece_instanceList[ID] = nullptr;
        }

        // STATIC METHODS FOR GLOBAL FUNCTIONALITY
        static int returnCount() {return Piece_count;};
        static std::vector<Piece*> returnInstanceList() {return Piece_instanceList;};
        static Piece* returnIDPtr(PIECE_ID ID_) {return Piece_instanceList[ID_];}; 
        static int returnTotalPieces() {return size(Piece_instanceList);};

        // NON-STATIC RETURNS
        PIECE_TYPE      returnType() {return type;};
        BoardPosition   returnPosition() {return pos;};
        PIECE_COLOUR    returnColour() {return col;};
        PIECE_ID        returnID() {return ID;};
        
        

        // FLAGS
        bool hasMoved() {return flag_move;};

        // SETTER METHODS
        void updatePosition(BoardPosition pos);
        
        // FUNCTIONALITY METHODS
        virtual PositionQueue moveRange() = 0; 


        
};



/***********************************************************
 *                   PIECE SUB-CLASSES
 ***********************************************************/

class Pawn : public Piece
{
    private:
        static PieceDescriptor descriptor; 
    public:
        Pawn(PIECE_COLOUR col_, int x, int y) : Piece(descriptor, col_, x, y) {};
        Pawn(PIECE_COLOUR col_, int x, int y, PIECE_ID ID) : Piece(descriptor, col_, x, y, ID) {};

        ~Pawn() {};
        PositionQueue moveRange();
};

class Rook : public Piece
{
    private:
        static PieceDescriptor descriptor; 
    public:
        Rook(PIECE_COLOUR col_, int x, int y) : Piece(descriptor, col_, x, y){};
        Rook(PIECE_COLOUR col_, int x, int y, PIECE_ID ID) : Piece(descriptor, col_, x, y, ID){};
        ~Rook() {};
        PositionQueue moveRange();
};

class Knight : public Piece
{
    private:
        static PieceDescriptor descriptor; 
    public:
        Knight(PIECE_COLOUR col_, int x, int y) : Piece(descriptor, col_, x, y) {};
        Knight(PIECE_COLOUR col_, int x, int y, PIECE_ID ID) : Piece(descriptor, col_, x, y, ID) {};
        ~Knight() {};
        PositionQueue moveRange();
};

class Bishop : public Piece
{
    private:
        static PieceDescriptor descriptor; 
    public:
        Bishop(PIECE_COLOUR col_, int x, int y) : Piece(descriptor, col_, x, y) {};
        Bishop(PIECE_COLOUR col_, int x, int y, PIECE_ID ID) : Piece(descriptor, col_, x, y, ID) {};
        ~Bishop() {};
        PositionQueue moveRange();
};

class Queen : public Piece
{
    private:
        static PieceDescriptor descriptor; 
    public:
        Queen(PIECE_COLOUR col_, int x, int y) : Piece(descriptor, col_, x, y) {};
        Queen(PIECE_COLOUR col_, int x, int y, PIECE_ID ID) : Piece(descriptor, col_, x, y, ID) {};
        ~Queen() {};
        PositionQueue moveRange();
};

class King : public Piece
{
    private:
        static PieceDescriptor descriptor; 
    public:
        King(PIECE_COLOUR col_, int x, int y) : Piece(descriptor, col_, x, y) {};
        King(PIECE_COLOUR col_, int x, int y, PIECE_ID ID) : Piece(descriptor, col_, x, y, ID) {};
        ~King() {};
        PositionQueue moveRange();
};
