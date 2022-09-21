/*                       ZYLO                           
 A small chess program developed on C++ using SDL2.
 */


// EXTERNAL DEPENDENCIES                                
#include <iostream>
#include <SDL2/SDL.h>


// INTERNAL DEPENDENCIES
#include "Board.h"
#include "SDL_Board.h"
#include "SDL_EventManager.h"


// TEMP
#include "PieceTypes.h"

int main () {

    std::cout << "Welcome to Zylo!" << std::endl;

    SDL_EventManager obj;
    obj.RunGame();

    Pawn* testPiece1 = new Pawn; 
    Pawn* testPiece2 = new Pawn; 
    Pawn* testPiece3 = new Pawn; 
    Pawn* testPiece4 = new Pawn; 

    std::cout << testPiece1->Get_Count() << std::endl;

    delete(testPiece3);

    std::cout << testPiece1->Get_Count() << std::endl;

    std::cout << testPiece1->Get_Colour() << std::endl;

    std::cout << testPiece1->Get_FilePath() << std::endl;
    
    std::cout << "ID 1: "<< testPiece1->Get_ID() << std::endl;
    std::cout << "ID 4: "<< testPiece4->Get_ID() << std::endl;

    
    

    
                

    
    


    return 0;
}