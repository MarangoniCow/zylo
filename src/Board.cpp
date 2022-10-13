/***********************************************************
 *                      BOARD IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "Board.h"
#include "Piece.h"

// EXTERNAL INCLUDES
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <queue>


void Board::initialiseBoard()
{
  
    static Pawn pw1(WHITE, 0, 1), pw2(WHITE, 1, 1), pw3(WHITE, 2, 1), pw4(WHITE, 3, 1), pw5(WHITE, 4, 1), pw6(WHITE, 5, 1), pw7(WHITE, 6, 1), pw8(WHITE, 7, 1);
    static Pawn pb1(BLACK, 0, 6), pb2(BLACK, 1, 6), pb3(BLACK, 2, 6), pb4(BLACK, 3, 6), pb5(BLACK, 4, 6), pb6(BLACK, 5, 6), pb7(BLACK, 6, 6), pb8(BLACK, 7, 6);

    static Rook rw1(WHITE, 0, 0), rw2(WHITE, 7, 0), rb1(BLACK, 0, 7), rb2(BLACK, 7, 7);
    static Knight kw1(WHITE, 1, 0), kw2(WHITE, 6, 0), kb1(BLACK, 1, 7), kb2(BLACK, 6, 7);
    static Bishop bw1(WHITE, 2, 0), bw2(WHITE, 5, 0), bb1(BLACK, 2, 7), bb2(BLACK, 5, 7);
    static Queen qw1(WHITE, 3, 0), qb1(BLACK, 3, 7);
    static King kiw1(WHITE, 4, 0), kib1(BLACK, 4, 7);
   
    std::vector<Piece*> pieceList = Piece::returnInstanceList();
    for(auto it = pieceList.begin(); it != pieceList.end(); it++)
    {
        Piece* temp = *it;

        if(temp != NULL)
            state.piecesCurr[temp->returnPosition().x][temp->returnPosition().y] = temp;
    }
    
    
    
}

void Board::addPieceToState(Piece* piece)
{

    if(piece == NULL) {
        std::cout << "Cannot add piece: null ptr" << std::endl;
        return;
    }
        
    BoardPosition pos = piece->returnPosition();
    if(pos.validPosition())
        state.piecesCurr[pos.x][pos.y] = piece;
    else
        std::cout << "Cannot add piece: invalid position" << std::endl;
}

void Board::movePiece(BoardPosition curPos, BoardPosition tarPos)
{
    // Determine the target piece
    Piece* currentPiece = state.piecesCurr[curPos.x][curPos.y];

    // If there isn't anything to be found, we don't do anything.
    if (currentPiece == NULL)
        return;

    bool found = 0;
    
    while(!takeMoves.empty())
    {
        validMoves.push(takeMoves.front());
        takeMoves.pop();
    }
    
    while(!validMoves.empty() && !found)
    {
        BoardPosition temp = validMoves.front();
        validMoves.pop();        
        if(temp == tarPos)
        {
            found = 1;
        }
            
    }
    
    if(!found)
        return;

    // Update old state
    memcpy(state.piecesPrev, state.piecesCurr, sizeof(state.piecesCurr));
  
    // Update current position in state as null
    state.piecesCurr[curPos.x][curPos.y] = NULL;
    // Update the target position in state with the current piece
    state.piecesCurr[tarPos.x][tarPos.y] = currentPiece; 

    // Update the piece position
    currentPiece->updatePosition(tarPos); 
}

void Board::processMoveQueue(std::queue<BoardPosition> moveQueue, BoardPosition curPos)
{
    // Helper functions, clean up later
    RELPOS relpos_curr = SAME;
    RELPOS relpos_prev = SAME;

    
    

    // Preinitialise helper variables
    BoardPosition tarPos;
    Piece* targetPiece;

    // Bool to check pieces
    bool pieceInPrev = 1;

    // Shorthand for current piece
    Piece* currentPiece = state.piecesCurr[curPos.x][curPos.y];

    // Knight algorithm: Special case, don't need to think about all the other bits
    if(currentPiece != NULL && currentPiece->returnDescriptor().type == KNIGHT)
    {
        while(!moveQueue.empty())
        {
            // Fetch target position and piece
            tarPos = moveQueue.front();
            moveQueue.pop();
            targetPiece = state.piecesCurr[tarPos.x][tarPos.y];

            if(targetPiece != NULL)
                std::cout << "Target piece colour: " << targetPiece->returnColour() << std::endl;
            

            // Check if the target piece exists, and check its colour
            if (targetPiece == NULL)
            {
                validMoves.push(tarPos);
            }
            else if (targetPiece->returnColour() == currentPiece->returnColour())
            {
                invalidMoves.push(tarPos);
            }
            else
            {
                takeMoves.push(tarPos);
            }
        }
        return;
    }

    
    std::cout << "NON-KNIGHT PIECE PROCESS QUEUE" << std::endl;
    while(!moveQueue.empty())
    {
        // Fetch the target position
        tarPos = moveQueue.front();
        moveQueue.pop();
        std::cout << "TARGET POSITION: " << tarPos.x << ", " << tarPos.y << std::endl;
        
        // Update relative position
        relpos_prev = relpos_curr;   
        relpos_curr = BoardPosition::returnRelPos(curPos, tarPos);
        std::cout << "POST: REL_POS_CURR: " << relpos_curr << ", REL_POS_PREV: " << relpos_prev << std::endl;

        

        
        
        
        // If we added a piece last time and we haven't changed the relative direction, we need to continue to the next item in the queue.
        // If we did add a piece last time, we need to see that piece has the same relative direction as the previous position.
        if(relpos_prev != SAME && pieceInPrev && relpos_curr == relpos_prev)
        {
            
            invalidMoves.push(tarPos);
            continue;
        }

        // Otherwise, fetch the piece in the currently targetted position
        targetPiece = state.piecesCurr[tarPos.x][tarPos.y];

        // Check for a piece in the current state
        if(targetPiece == NULL)
        {
            // If there's nothing there, the move is valid. We can push it to the valid move list and continue as normal.
            validMoves.push(tarPos);
            // There's no piece, so our if condition at the beginning won't fail.
            pieceInPrev = 0;

        }
        // If it isn't null, that means there's something there. We need to check if it's the same colour.
        else 
        {
                       
            if (targetPiece->returnColour() != currentPiece->returnColour())
            {
                // If it's a pawn, we can't take forward
                if(currentPiece->returnDescriptor().type == PAWN)
                    invalidMoves.push(tarPos);
                else
                    takeMoves.push(tarPos);
            }
            else
            {
                invalidMoves.push(tarPos);
            }
            // Regardless, we know that there's a piece in the previous location we looked. Back to the start.
            pieceInPrev = 1;
        }
    }

}

void Board::generateMovementRange(BoardPosition curPos) {

    std::cout << "Generate movement range" << std::endl;

    
    // Initialise movement queue
    std::queue<BoardPosition> moveQueue;

    std::cout << "Empty move list" << std::endl;

    // Empty the move queues
    validMoves = moveQueue;
    takeMoves = moveQueue;
    invalidMoves = moveQueue;

    std::cout << "Determine piece" << std::endl;
    
    // Determine the piece in the old position
    Piece* currentPiece = state.piecesCurr[curPos.x][curPos.y];

    std::cout << "Check that current piece isn't null" << std::endl;
    std::cout << currentPiece << std::endl;

    if (currentPiece == nullptr)
        return;

   
    // Generate move range
    std::cout << "Call move range" << std::endl;
    moveQueue = currentPiece->moveRange();

    std::cout << "Call process move queue" << std::endl;
    // Process the movement range from the current position
    processMoveQueue(moveQueue, curPos);


                    
    

    



    
        // Special moves list
    switch(currentPiece->returnDescriptor().type)
    {
        case PAWN:
        {
            // Initialise
            PIECE_COLOUR oppositeCol;
            int enpassantRow;
            int forward;

            if(currentPiece->returnColour() == WHITE)
            {
                oppositeCol = BLACK;
                enpassantRow = 4;
                forward = 1;               
            }
            else
            {
                oppositeCol = WHITE;
                enpassantRow = 3;
                forward = -1;                
            }

            // GENERATE TAKE & EN-PASSANT: For loop repeats move queue for left/right instructions
            for(int i = -1; i <= 1; i += 2)
            {
                // CHECK FOR TAKE: Check if we're at the edge or not
                if(curPos.validUpdate(i, forward))
                {
                    BoardPosition temp = curPos.returnUpdate(i, forward);
                    Piece* targetPiece = state.piecesCurr[temp.x][temp.y];
                    
                    if(targetPiece != NULL && targetPiece->returnColour() == oppositeCol)
                        takeMoves.push(temp);
                }

                    // CHECK FOR EN-PASSANT: Must be on the row adjacent to backpawn line
                if(curPos.y == enpassantRow && curPos.validUpdate(i, 0))
                {
                    Piece* targetPiece;
                    targetPiece = state.piecesCurr[curPos.x + i][enpassantRow];
                    
                    // Check for an existence piece, and that it's a pawn, and that it's the opposite colour (black)
                    bool pawnAdjacent = (targetPiece != NULL && targetPiece->returnDescriptor().type == PAWN && targetPiece->returnColour() == oppositeCol) ? 1 : 0;
                    // Check that if it was there last move (needs to be an updated move)
                    bool adjacentLastMove = (targetPiece == state.piecesPrev[curPos.x + i][enpassantRow]) ? 1 : 0;
                    
                    if(pawnAdjacent && !adjacentLastMove)
                        takeMoves.push(curPos.returnUpdate(i, forward));
                }
            }     
        }
        case KING:
        {
            if(!currentPiece->hasMoved())
            {
                Piece* targetRookRight = state.piecesCurr[7][curPos.y];
                Piece* targetRookLeft  = state.piecesCurr[0][curPos.y];

                if(targetRookRight->returnDescriptor().type == ROOK && !targetRookRight->hasMoved() && targetRookRight->returnColour() == currentPiece->returnColour())
                    validMoves.push(curPos.returnUpdate(2, 0));
                if(targetRookLeft->returnDescriptor().type == ROOK && !targetRookLeft->hasMoved() && targetRookLeft->returnColour() == currentPiece->returnColour())
                    validMoves.push(curPos.returnUpdate(-2, 0));
            }
        }
        default:
        {};
    }
    
    
}

bool Board::updatePiecePosition(int Piece_ID, BoardPosition newPos)
{
    // 1) UPDATE PIECE WITHIN PIECE CLASS
    Piece* targetPiece = Piece::returnInstanceList()[Piece_ID];

    if(targetPiece->returnPosition().validUpdate(newPos))
        targetPiece->updatePosition(newPos);
    else
        return 0;

    // 2) UPDATE STATE
    state.piecesCurr[targetPiece->returnPosition().x][targetPiece->returnPosition().y] = targetPiece;
    return 1;
}

void Board::removePiece(int Piece_ID)
{
    Piece* targetPiece = Piece::returnInstanceList()[Piece_ID];
    
    //  1) UPDATE STATE
    state.piecesCurr[targetPiece->returnPosition().x][targetPiece->returnPosition().y] = nullptr;

    // 2) DELETE PIECE
    delete(targetPiece);

    


}