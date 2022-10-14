/*                       ZYLO                           
 A small chess program developed on C++ using SDL2.
 */


// EXTERNAL DEPENDENCIES                                
#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>


// INTERNAL DEPENDENCIES
#include "SDL_EventManager.h"


// TEMP
#include "Piece.h"

int main () {

    std::cout << "Welcome to Zylo!" << std::endl;

    SDL_EventManager obj;
    obj.RunGame();


    
    

    
                

    
    


    return 0;
}