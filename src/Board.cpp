/***********************************************************
 *                      BOARD IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "Board.h"
#include "Piece.h"

// EXTERNAL INCLUDES
#include <iostream>
#include <vector>
#include <queue>
#include <utility>

/****************************************************/
/*              CONSTRUCTOR & ADMIN                 */
/****************************************************/

/****************************************************/
/*            STATE-RELATED FUNCTIONS               */
/****************************************************/
void Board::processPromotion(PIECE_TYPE newType)
{
    // Update state
    state.updatePiece(state.boardFlags.pawnPromotion.second, newType);

    // Update promotion flags
    state.boardFlags.pawnPromotion.first = 0;
    state.boardFlags.pawnPromotion.second = 0;
}
bool Board::processUpdate(BoardPosition curPos, BoardPosition tarPos)
{
    // Determine the target piece
    Piece* currentPiece = state.piecesCurr[curPos.x][curPos.y];

    // If there isn't anything to be found, we don't do anything.
    if (currentPiece == NULL)
        return 0;

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

        // Begin movement task flow
        preMoveTasks();
        movePiece(currentPiece, tarPos);
        postMoveTasks();
        return 1;
    }
    else
        return 0;
    
}

/****************************************************/
/*           MOVEMENT-RELATED FUNCTIONS (TASKS)     */
/****************************************************/

void Board::preMoveTasks()
{
    // Flags are always reset BEFORE moving occurs so that movement can properly generate flags
    state.boardFlags.RESET_FLAGS();
    state.clearChecksQueue();
}
void Board::postMoveTasks()
{
    // TO DO: Re-arrange movement flags

    // Set turn
    state.currentTurn = (state.currentTurn == WHITE) ? BLACK : WHITE;

    // Check if King is in check
    PIECE_ID fetchKingID(state.currentTurn);    
}
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
            int endrow = (currentPiece->returnColour() == WHITE) ? 7 : 0;
            if(currentPiece->returnPosition().y == endrow)
            {
                state.boardFlags.pawnPromotion.first = 1;
                state.boardFlags.pawnPromotion.second = currentPiece->returnID();
            }

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
    state.removePiece(pieceToDelete);
    movePiece(currentPiece, newPos);

}


/****************************************************/
/*           MOVEMENT-RELATED FUNCTIONS (PUBLIC)     */
/****************************************************/


MovementQueue Board::generateMovementRange(BoardPosition curPos) {

    // Initialise local movement queue variables. Slightly lazy as I introduced MovementQueue after I created the multiple queue implementation.
    PositionQueue moveRange;
    MovementQueue processedQueue;

    // Determine the piece in the old position
    Piece* currentPiece = state.piecesCurr[curPos.x][curPos.y];

    if (currentPiece == nullptr)
        return processedQueue;
    else if (currentPiece->returnColour() != state.currentTurn)
        return processedQueue;
   
    // Generate move range
    moveRange = currentPiece->moveRange();

    // Process the movement range from the current position
    processedQueue = processMoveRange(moveRange, curPos);
    // Add special takes (pawn takes & enpassant)
    addSpecialTakes(currentPiece, &processedQueue.validTakes);

    // KING-CHECKED ACTIONS:=
    // 1) Fetch the appropriate King's ID
    // 2) Check that he's not in check
    // 3) If he is, update the board flags
    // 4) Place restrictions on the rest of our movement
    PIECE_ID kingID = fetchKingID(currentPiece->returnColour());
    if(isChecked(kingID))
    {
        state.boardFlags.kingCheck.first = true;
        state.boardFlags.kingCheck.second = kingID;
        if(currentPiece->returnID() != kingID)
        {
            MovementQueue emptyQueue;
            return emptyQueue;
        }        
    }  
    
    // Add special moves (castling)
    addSpecialMoves(currentPiece, &processedQueue.validMoves);

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
                if (rightRookConditions && clearRight && canCastle(currentPiece->returnID(), RIGHT))
                    validMoves->push(curPos.returnUpdate(2, 0));
                if (leftRookConditions && clearLeft  && canCastle(currentPiece->returnID(), LEFT))
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
        default: {
            break;
        };
    };
};
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
            targetPiece = state.piecesCurr[tarPos.x][tarPos.y];

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


/****************************************************/
/*             CHECK-RELATED FUNCTIONS              */
/****************************************************/

/* METHOD ONE FOR CASTLING:
* 1) Generate the movement queues of ALL pieces
* 2) Check if the king is being checked by anything
* 3) Add as conditional
*/


PieceChecks Board::pieceChecks(PIECE_ID ID)
{
    // Queue for checked pieces
    IDQueue checkedPieces;

    // Get the piece...
    Piece* piecePtr = Piece::returnIDPtr(ID);

    // Generate the queue of all its pieces with special takes
    PositionQueue moveRange = piecePtr->moveRange();
    MovementQueue moveQueue = processMoveRange(moveRange, piecePtr->returnPosition());
    addSpecialTakes(piecePtr, &moveQueue.validTakes);


    // Iteate through list of takes and add ID to checkedPieces.
    while(!moveQueue.validTakes.empty())
    {
        BoardPosition targetPos = moveQueue.validTakes.front();
        moveQueue.validTakes.pop();
        checkedPieces.push(state.piecesCurr[targetPos.x][targetPos.y]->returnID());
    }
    return std::make_pair(ID, checkedPieces);
}
ChecksQueue Board::generateChecksList(PIECE_COLOUR col)
{
    // Initialise return
    ChecksQueue checksQueue;

    // Iterate over all board positions
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            PieceChecks tempQueue;
            PIECE_ID tempID;
            // Collect the current piece from state
            Piece* piecePtr = state.piecesCurr[i][j];

            // Skip over it if it's null or it's a different colour to the one we're checking
            if(piecePtr == NULL || piecePtr->returnColour() != col)
                continue;
            // Else, fetch the list of pieces that that piece checks
            else
            {
                tempID = piecePtr->returnID();
                tempQueue = pieceChecks(tempID);
            }
                
            checksQueue.push(tempQueue);
        }
    }
    state.currentChecks = checksQueue;
    return checksQueue;
}
bool Board::isChecked(PIECE_ID ID)
{

    // Retrieve pointer of interest
    Piece* piecePtr = Piece::returnIDPtr(ID);

    // Fetch the list of pieces checked by the opposite colour
    PIECE_COLOUR col = (piecePtr->returnColour() == WHITE) ? BLACK : WHITE;

    // If the queue is empty, generate it
    if(state.currentChecks.empty())
    {
        generateChecksList(col);
    }
    
    // Iterate through the checks list
    ChecksQueue queueToCheck = state.currentChecks;

    bool found = 0;
    while(!queueToCheck.empty() && !found)
    {
        PieceChecks tempQueue = queueToCheck.front();
        queueToCheck.pop();

        while(!tempQueue.second.empty())
        {
            found = (ID == tempQueue.second.front()) ? 1 : 0;
            tempQueue.second.pop();
        }
    }
    return found;
}
bool Board::isCheckmated(PIECE_ID ID)
{
    return 0;
}
bool Board::canCastle(PIECE_ID ID, RELPOS relpos)
{
    /* POTENTIAL CODING DESIGN FLAW */
    // Modifying the 'current state' struct and returning it. Potential for going wrong, but seems a neat trick....

    BoardState stateToRestore = state;
    BoardPosition kingPos = Piece::returnIDPtr(ID)->returnPosition();
    Piece* kingPtr = state.piecesCurr[kingPos.x][kingPos.y];
    
    // // Decide if we're castling right or left
    int castlingDirection = (relpos == RIGHT) ? 1 : -1;

    // // Temporarily add the king to the new positions
    state.piecesCurr[kingPos.x + castlingDirection][kingPos.y] = kingPtr;
    state.piecesCurr[kingPos.x + 2*castlingDirection][kingPos.y] = kingPtr;

    // Run is checked!
    bool inCheck = isChecked(ID);

    // Restore the state...
    state = stateToRestore;

    // Done :)
    return !inCheck;
}
PIECE_ID Board::fetchKingID(PIECE_COLOUR col)
{
    bool found = 0;

    for(int ID = 0; ID < Piece::returnTotalPieces(); ID++)
    {
        Piece* tempPiece = Piece::returnIDPtr(ID);
        if(tempPiece == NULL)
            continue;
        
        found = (tempPiece->returnDescriptor().type == KING && tempPiece->returnColour() == col) ? 1 : 0;
        if(found)
            return ID;
    }
    std::cout << "Error: No king on the board!" << std::endl;
    return 0;
    
}