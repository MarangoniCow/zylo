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

/****************************************************/
/*                  CONSTRUCTORS                    */
/****************************************************/

void Board::initialiseBoard()
{
    // Initialise all 32 pieces with correct positions
    static Pawn pw1(WHITE, 0, 1), pw2(WHITE, 1, 1), pw3(WHITE, 2, 1), pw4(WHITE, 3, 1),
                    pw5(WHITE, 4, 1), pw6(WHITE, 5, 1), pw7(WHITE, 6, 1), pw8(WHITE, 7, 1);
    static Pawn pb1(BLACK, 0, 6), pb2(BLACK, 1, 6), pb3(BLACK, 2, 6), pb4(BLACK, 3, 6),
                    pb5(BLACK, 4, 6), pb6(BLACK, 5, 6), pb7(BLACK, 6, 6), pb8(BLACK, 7, 6);
    static Rook rw1(WHITE, 0, 0), rw2(WHITE, 7, 0), rb1(BLACK, 0, 7), rb2(BLACK, 7, 7);
    static Knight kw1(WHITE, 1, 0), kw2(WHITE, 6, 0), kb1(BLACK, 1, 7), kb2(BLACK, 6, 7);
    static Bishop bw1(WHITE, 2, 0), bw2(WHITE, 5, 0), bb1(BLACK, 2, 7), bb2(BLACK, 5, 7);
    static Queen qw1(WHITE, 3, 0), qb1(BLACK, 3, 7);
    static King kiw1(WHITE, 4, 0), kib1(BLACK, 4, 7);
   
   // Return the list of pieces from Piece, iterate over it and update BoardState accordingly.
    std::vector<Piece*> pieceList = Piece::returnInstanceList();
    for(auto it = pieceList.begin(); it != pieceList.end(); it++)
    {
        Piece* temp = *it;

        if(temp != NULL)
            state.piecesCurr[temp->returnPosition().x][temp->returnPosition().y] = temp;
    }
}

/****************************************************/
/*            STATE-RELATED FUNCTIONS               */
/****************************************************/
void Board::addPieceToState(Piece* piece)
{

    if(piece == NULL) {
        std::cout << "Cannot add piece: nullptr" << std::endl;
        return;
    }

    BoardPosition pos = piece->returnPosition();
    if(pos.validPosition())
        state.piecesCurr[pos.x][pos.y] = piece;
    else
        std::cout << "Cannot add piece: invalid position" << std::endl;
}

void Board::processClick(BoardPosition curPos, BoardPosition tarPos)
{
    // Determine the target piece
    Piece* currentPiece = state.piecesCurr[curPos.x][curPos.y];

    // If there isn't anything to be found, we don't do anything.
    if (currentPiece == NULL)
        return;

    

    
    /* CODE-ARCHITECTURE FLAW: 
    * This requires us to have already generated the movement range for the piece at curPos.
    */

    // Otherwise, iterate through the current list of moves for that piece.
    bool found = 0;
    
    while(!takeMoves.empty() && !found)
    {
        BoardPosition temp = takeMoves.front();
        takeMoves.pop();        
        if(temp == tarPos)
            found = 1;
    }

    if(found)
    {
        // Update old state
        memcpy(state.piecesPrev, state.piecesCurr, sizeof(state.piecesCurr));
        takePiece(currentPiece, tarPos);
    }
        
    
    while(!validMoves.empty() && !found)
    {
        BoardPosition temp = validMoves.front();
        validMoves.pop();        
        if(temp == tarPos)
            found = 1;
    }
    
    if(found)
    {
        // Update old state
        memcpy(state.piecesPrev, state.piecesCurr, sizeof(state.piecesCurr));
        movePiece(currentPiece, tarPos);
    }
        
    else
        return;

    
  
    
    

    
}

/****************************************************/
/*           MOVEMENT-RELATED FUNCTIONS             */
/****************************************************/

void Board::movePiece(Piece* currentPiece, BoardPosition newPos)
{
    BoardPosition curPos = currentPiece->returnPosition();

    // Update current position in state as null
    state.piecesCurr[curPos.x][curPos.y] = NULL;

    // Update the target position in state with the current piece
    state.piecesCurr[newPos.x][newPos.y] = currentPiece;

    // Update the piece position
    currentPiece->updatePosition(newPos);

    /* SPECIAL CASES: PROMOTION & CASTLING */
    switch (currentPiece->returnDescriptor().type)
    {
        case KING:
        {   
            // Right-side castling
            if(curPos.x - newPos.x == -2)
            {
                BoardPosition oldRookPos = newPos.returnUpdate(1, 0);
                BoardPosition newRookPos = newPos.returnUpdate(-1, 0);
                movePiece(state.piecesCurr[oldRookPos.x][oldRookPos.y], newRookPos);
            }
            // Left-side castling
            else if(curPos.x - newPos.x == 2)
            {
                BoardPosition oldRookPos = newPos.returnUpdate(-2, 0);
                BoardPosition newRookPos = newPos.returnUpdate(1, 0);
                movePiece(state.piecesCurr[oldRookPos.x][oldRookPos.y], newRookPos);
            }
            break;
        }
        case PAWN:
        {
            break;            
        }
        default: {
            break;
        }
    }
    
}
void Board::takePiece(Piece* currentPiece, BoardPosition newPos)
{
    Piece* pieceToDelete;
    /* SPECIAL CASE: EN-PASSANT */
    if(currentPiece->returnDescriptor().type == PAWN && state.piecesCurr[newPos.x][newPos.y] == NULL)
    {
        int forward = (currentPiece->returnColour() == WHITE) ? 1 : -1;
        pieceToDelete = state.piecesCurr[newPos.x][newPos.y - forward];
    }
    else
    {
        pieceToDelete = state.piecesCurr[newPos.x][newPos.y];
    }
    removePiece(pieceToDelete);
    movePiece(currentPiece, newPos);

}

void Board::removePiece(Piece* pieceToDelete)
{
    // Get current position
    BoardPosition pos = pieceToDelete->returnPosition();

    // Update board state to null
    state.piecesCurr[pos.x][pos.y] = NULL;

    // Delete the piece, destructor will take care of the rest
    // delete(pieceToDelete);
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

      
    // Initialise movement queue
    std::queue<BoardPosition> moveQueue;

    // Empty the move queues
    validMoves = moveQueue;
    takeMoves = moveQueue;
    invalidMoves = moveQueue;

    
    // Determine the piece in the old position
    Piece* currentPiece = state.piecesCurr[curPos.x][curPos.y];

    if (currentPiece == nullptr)
        return;
   
    // Generate move range
    moveQueue = currentPiece->moveRange();

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

