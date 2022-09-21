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


struct PieceDescriptor {
    std::string pieceColour;
    std::string pieceFilePath; 
};


class Piece 
{
    protected:
        static std::vector<int> ID_list;            // Global ID list
        static int Piece_Count;                     // Global piece count
        int Piece_ID;                               // Instance specific ID
        const PieceDescriptor& descriptor;          // Sub-class specific descriptor

    public:
        Piece(const PieceDescriptor& descriptor_) : descriptor(descriptor_) {
            Piece_Count += 1;
            Piece_ID = ID_list.size();
            ID_list.push_back(Piece_ID); 
        };                    
        virtual ~Piece() {
            Piece_Count -= 1; 
        }

        int Get_Count();
        int Get_ID();
        std::string Get_Colour();
        std::string Get_FilePath();
        
};