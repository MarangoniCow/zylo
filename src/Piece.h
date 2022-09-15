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


struct PieceDescriptor {
    std::string pieceColour;
    std::string pieceFilePath; 
};


class Piece 
{
    protected:
        static int Piece_Count;                     // Global piece count
        int Piece_ID;                               // Instance specific ID
        const PieceDescriptor& descriptor;          // Sub-class specific descriptor

    public:
        Piece(const PieceDescriptor& descriptor_) : descriptor(descriptor_) {
            Piece_Count += 1;
        };                    
        virtual ~Piece() {
            Piece_Count -= 1; 
        }

        int Get_Count();
        std::string Get_Colour() {return descriptor.pieceColour;};
        std::string Get_FilePath()  {return descriptor.pieceFilePath;};
};