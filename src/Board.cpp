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
    * I'm generating the same movement queue twice. I could just pass it as a new argument?
    */

    MovementQueue moveQueue = generateMovementRange(currentPiece->returnPosition());

    // Otherwise, iterate through the current list of moves for that piece.
    bool found = 0;
    
    while(!moveQueue.validTakes.empty() && !found)
    {
        BoardPosition temp = moveQueue.validTakes.front();
        moveQueue.validTakes.pop();        
        if(temp == tarPos)
            found = 1;
    }

    if(found)
    {
        // Update old state
        memcpy(state.piecesPrev, state.piecesCurr, sizeof(state.piecesCurr));
        takePiece(currentPiece, tarPos);
    }
        
    
    while(!moveQueue.validMoves.empty() && !found)
    {
        BoardPosition temp = moveQueue.validMoves.front();
        moveQueue.validMoves.pop();        
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

    /* BUG: DESTRUCTOR FOR DERIVED CLASS NOT PROPERLY CALLED */
    // delete(pieceToDelete);
}

MovementQueue Board::processMoveRange(PositionQueue moveRange, BoardPosition curPos)
{
    // Helper/local variable names.
    RELPOS relpos_curr = SAME;
    RELPOS relpos_prev = SAME;  

    PositionQueue validMoves;
    PositionQueue validTakes;
    PositionQueue invalidMoves;
    PositionQueue invalidTakes;
    

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
        while(!moveRange.empty())
        {
            // Fetch target position and piece
            tarPos = moveRange.front();
            moveRange.pop();
<<<<<<< HEAD
            targetPiece = state.piecesCurr[tarPos.x][tarPos.y];          
=======
            targetPiece = state.piecesCurr[tarPos.x][tarPos.y];
>>>>>>> main

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
                validTakes.push(tarPos);
            }
        }
        
        MovementQueue proceesedQueue(validMoves, validTakes, invalidMoves, invalidTakes);
        return proceesedQueue;  
    }

    while(!moveRange.empty())
    {
        // Fetch the target position
        tarPos = moveRange.front();
        moveRange.pop();
        
        // Update relative position
        relpos_prev = relpos_curr;   
        relpos_curr = BoardPosition::returnRelPos(curPos, tarPos);
        
        // If we added a piece last time and we haven't changed the relative direction, we need to continue to the next item in the queue.
        // If we did add a piece last time, we need to see that piece has the same relative direction as the previous position.
        if(relpos_prev != SAME && pieceInPrev && relpos_curr == relpos_prev)
        {
            targetPiece = state.piecesCurr[tarPos.x][tarPos.y];

            if(targetPiece != NULL && currentPiece->returnColour() != targetPiece->returnColour())
                invalidTakes.push(tarPos);
            else
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
                    validTakes.push(tarPos);
            }
            else
            {
                invalidMoves.push(tarPos);
            }
            // Regardless, we know that there's a piece in the previous location we looked. Back to the start.
            pieceInPrev = 1;
        }
    }

    MovementQueue proceesedQueue(validMoves, validTakes, invalidMoves, invalidTakes);
    return proceesedQueue;  

}

MovementQueue Board::generateMovementRange(BoardPosition curPos) {

      
    // Initialise local movement queue variables. Slightly lazy as I introduced MovementQueue after I created the multiple queue implementation.
    PositionQueue moveRange;
    MovementQueue processedQueue;

    // Determine the piece in the old position
    Piece* currentPiece = state.piecesCurr[curPos.x][curPos.y];

    if (currentPiece == nullptr)
        return processedQueue;
   
    // Generate move range
    moveRange = currentPiece->moveRange();

    // Process the movement range from the current position
    processedQueue = processMoveRange(moveRange, curPos);
    
    // Add special moves & takes
    addSpecialMoves(currentPiece, &processedQueue.validMoves);
    addSpecialTakes(currentPiece, &processedQueue.validTakes);

    return processedQueue;   
}

void Board::addSpecialMoves(Piece* currentPiece, PositionQueue* validMoves)
{
    if(currentPiece == NULL)
        return;
    
    BoardPosition curPos = currentPiece->returnPosition();
    

    // Special moves list
    switch(currentPiece->returnDescriptor().type)
    {
        case PAWN:
        {
            // Promotion will go here
        }
        case KING:
        {
            if(!currentPiece->hasMoved())
            {
                

                Piece* rookRight = state.piecesCurr[7][curPos.y];
                Piece* rookLeft  = state.piecesCurr[0][curPos.y];

                

                bool rightRookConditions = (rookRight != NULL && rookRight->returnDescriptor().type == ROOK && !rookRight->hasMoved() 
                    && rookRight->returnColour() == currentPiece->returnColour()) ? 1 : 0;
                bool leftRookConditions = (rookLeft != NULL && rookLeft->returnDescriptor().type == ROOK && !rookLeft->hasMoved()
                    && rookLeft->returnColour() == currentPiece->returnColour()) ? 1 : 0;

                bool clearRight = (state.piecesCurr[5][curPos.y] == NULL && state.piecesCurr[6][curPos.y] == NULL) ? 1 : 0;
                bool clearLeft  = (state.piecesCurr[3][curPos.y] == NULL && state.piecesCurr[2][curPos.y] == NULL) ? 1 : 0;

                if(rookLeft == NULL || rookRight == NULL)
                    break;
                if (rightRookConditions && clearRight)
                    validMoves->push(curPos.returnUpdate(2, 0));
                if (leftRookConditions && clearLeft)
                    validMoves->push(curPos.returnUpdate(-2, 0));
            }
            break;
        }
        default: {
            break;
        };
    }
    

}

void Board::addSpecialTakes(Piece* currentPiece, PositionQueue* validTakes)
{
    if(currentPiece == NULL)
        return;
    
    BoardPosition curPos = currentPiece->returnPosition();
    

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
                        validTakes->push(temp);
                }

                    // CHECK FOR EN-PASSANT: Must be on the row adjacent to backpawn line
                if(curPos.y == enpassantRow && curPos.validUpdate(i, 0))
                {
                    Piece* targetPiece;
                    targetPiece = state.piecesCurr[curPos.x + i][enpassantRow];
                    if(targetPiece == NULL)
                        continue;
                    
                    // Check for an existence piece, and that it's a pawn, and that it's the opposite colour (black)
                    bool pawnAdjacent = (targetPiece->returnDescriptor().type == PAWN && targetPiece->returnColour() == oppositeCol) ? 1 : 0;

                    // Check to see if it was on the home row last move
                    Piece* targetPiecePrev = state.piecesPrev[curPos.x + i][enpassantRow + 2*forward];
                    if(targetPiecePrev == NULL)
                        continue;

                    bool lastOnHomeRow = ( targetPiece->returnID() == targetPiecePrev->returnID()) ? 1 : 0;
                    
                    if(pawnAdjacent && lastOnHomeRow)
                        validTakes->push(curPos.returnUpdate(i, forward));
                }
            }
            break;     
        }
<<<<<<< HEAD
        case KING:
        {
            if(!currentPiece->hasMoved())
            {
                

                Piece* targetRookRight = state.piecesCurr[7][curPos.y];
                Piece* targetRookLeft  = state.piecesCurr[0][curPos.y];

                bool rightRookConditions = (targetRookRight->returnDescriptor().type == ROOK && !targetRookRight->hasMoved() 
                    && targetRookRight->returnColour() == currentPiece->returnColour()) ? 1 : 0;
                bool leftRookConditions = (targetRookLeft->returnDescriptor().type == ROOK && !targetRookLeft->hasMoved()
                    && targetRookLeft->returnColour() == currentPiece->returnColour()) ? 1 : 0;

                bool clearRight = (state.piecesCurr[5][curPos.y] == NULL && state.piecesCurr[6][curPos.y] == NULL) ? 1 : 0;
                bool clearLeft  = (state.piecesCurr[3][curPos.y] == NULL && state.piecesCurr[2][curPos.y] == NULL) ? 1 : 0;

                if(targetRookLeft == NULL || targetRookRight == NULL)
                    break;
                if (rightRookConditions && clearRight)
                    validMoves.push(curPos.returnUpdate(2, 0));
                if (leftRookConditions && clearLeft)
                    validMoves.push(curPos.returnUpdate(-2, 0));
                    //  && canCastle(currentPiece->returnID(), LEFT)
            }
            break;
        }
=======
>>>>>>> main
        default: {
            break;
        };
    }
}









/****************************************************/
/*           MOVEMENT-RELATED FUNCTIONS             */
/****************************************************/

/* METHOD ONE FOR CASTLING:
* 1) Generate the movement queues of ALL pieces
* 2) Check if the king is being checked by anything
* 3) Add as conditional
*/


IDQueue Board::pieceChecks(PIECE_ID ID)
{
    IDQueue checkedPieces;
    Piece* piecePtr = Piece::returnIDPtr(ID);
    MovementQueue moveQueue = generateMovementRange(piecePtr->returnPosition());

    while(!moveQueue.validTakes.empty())
    {
        BoardPosition targetPos = moveQueue.validTakes.front();
        moveQueue.validTakes.pop();
        checkedPieces.push(state.piecesCurr[targetPos.x][targetPos.y]->returnID());
    }
    return checkedPieces;
}

IDQueue Board::generateCheckedList(PIECE_COLOUR col)
{
    // Initialise return
    IDQueue checkedQueue;

    // Iterate over all board positions
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            IDQueue tempQueue;

            // Collect the current piece from state
            Piece* piecePtr = state.piecesCurr[i][j];

            // Skip over it if it's null or it's a different colour
            if(piecePtr == NULL || piecePtr->returnColour() != col)
                continue;
            else
                tempQueue = pieceChecks(piecePtr->returnID());

            while(!tempQueue.empty())
            {
                checkedQueue.push(tempQueue.front());
                tempQueue.pop();
            }                
        }
    }
    return checkedQueue;
}

bool Board::isChecked(PIECE_ID ID)
{

    /*          CODING FLAW FOUND           
    * When iterating through all the pieces to generate their movements, I inevitably call a king.
    * When I call a king, I run 'isChecked'.
    * This means I generate my movement list again, and hence I create an infinite loop.
    * 
    * There are few workarounds:
    * 1) I can change when 'isChecked' is called, that way I won't run into a loop
    * 2) I can omit kings from 'isChecked', since a king can't check someone. I don't like this.
    * 3) I can add a vector describing all the checked pieces and make sure I only iterate through it once.
    * 
    * How frustrating.
    * 
    */

    // Retrieve pointer of interest
    Piece* piecePtr = Piece::returnIDPtr(ID);

    // Fetch the list of pieces checked by the opposite colour
    PIECE_COLOUR col = (col == WHITE) ? BLACK : WHITE;
    IDQueue checkedQueue = generateCheckedList(col);

    bool found = 0;
    while(!checkedQueue.empty() && !found)
    {
        found = (ID == checkedQueue.front()) ? 1 : 0;
        checkedQueue.pop();
    }
    return found;
}

bool Board::canCastle(PIECE_ID ID, RELPOS relpos)
{
    /* POTENTIAL CODING DESIGN FLAW */
    // Modifying the 'current state' struct and returning it. Potential for going wrong, but seems a neat trick....

    BoardState stateToRestore = state;
    BoardPosition kingPos = Piece::returnIDPtr(ID)->returnPosition();
    Piece* kingPtr = state.piecesCurr[kingPos.x][kingPos.y];
    
    // // Decide if we're castling right or left
    // int castlingDirection = (relpos == RIGHT) ? 1 : -1;

    // // Temporarily add the king to the new positions
    // state.piecesCurr[kingPos.x + castlingDirection][kingPos.y] = kingPtr;
    // state.piecesCurr[kingPos.x + 2*castlingDirection][kingPos.y] = kingPtr;

    // Run is checked!
    bool inCheck = isChecked(ID);

    // Restore the state...
    state = stateToRestore;

    // Done :)
    return inCheck;
}