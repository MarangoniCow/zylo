/***********************************************************
 *                BOARDMOVES IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "Piece.h"
#include "BoardState.h"
#include "BoardMoves.h"

// EXTERNAL INCLUDES
#include <iostream>
#include <vector>
#include <queue>
#include <utility>


/****************************************************/
/*              CONSTRUCTOR & ADMIN                 */
/****************************************************/
BoardMoves::BoardMoves()
{
    resetFlags();
}
BoardMoves::BoardMoves(BoardState* statePtr_)
{
    statePtr = statePtr_;
    // Run process state
    processState();
}

void BoardMoves::resetFlags()
{
    kingCheck.first = 0;
    kingCheck.second = 0;
    kingCheckmate.first = 0;
    kingCheckmate.second = 0;
}



void BoardMoves::processState()
{
    // 0) Pre-process: Reset board flags
    resetFlags();

    // 1) Fetch queue of all pieces
    PieceQueue piecesQueue = statePtr->returnPieceQueue();

    // 2) Iterate through entire list, generating movement range of all pieces
    while(!piecesQueue.empty())
    {
        Piece* piece = piecesQueue.front();
        piecesQueue.pop();
        generateMovementRange(piece);
    }

    // 3) Generate checks

    // 4) Refine list    
}


/****************************************************/
/*           MOVEMENT-RELATED FUNCTIONS (PUBLIC)     */
/****************************************************/


void BoardMoves::generateMovementRange(Piece* piece)
{
    // Initialise local movement queue variables
    MovementQueue moveQueue;

    // Check we've not got a null piece
    if(statePtr->pieceExists(piece))
        return;

    // Process the movement range from the current position
    moveQueue = processMoveRange(piece, piece->moveRange());

    // Add special takes (pawn takes & enpassant)
    // addSpecialTakes(piece, &moveQueue.validTakes);
    // addSpecialMoves(piece, &moveQueue.validMoves);

    // Add queue to appropriate board location
    BoardPosition pos = piece->returnPosition();
    movementState[pos.x][pos.y] = moveQueue;
}


MovementQueue BoardMoves::processMoveRange(Piece* piece, PositionQueue moveRange)
{
    // Helper/local variables
    RELPOS relpos_curr = SAME;
    RELPOS relpos_prev = SAME;  

    // Queues to fill! 
    PositionQueue validMoves;
    PositionQueue validTakes;
    PositionQueue invalidMoves;
    PositionQueue invalidTakes;

    // Fetch current values
    BoardPosition pos = piece->returnPosition();

    // Initialise target values
    BoardPosition tarPos;
    Piece* tarPiece;

    // Bool to check pieces
    bool pieceInPrev = 1;


    // Knight algorithm: Special case, don't need to think about all the other bits
    if(piece != NULL && piece->returnDescriptor().type == KNIGHT)
    {
        while(!moveRange.empty())
        {
            // Fetch target position and piece
            tarPos = moveRange.front();
            moveRange.pop();
            tarPiece = statePtr->current[tarPos.x][tarPos.y];

            // Check if the target piece exists, and check its colour
            if (tarPiece == NULL)
            {
                validMoves.push(tarPos);
            }
            else if (tarPiece->returnColour() == piece->returnColour())
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
        relpos_curr = BoardPosition::returnRelPos(pos, tarPos);
        
        // If we added a piece last time and we haven't changed the relative direction, we need to continue to the next item in the queue.
        // If we did add a piece last time, we need to see that piece has the same relative direction as the previous position.
        if(relpos_prev != SAME && pieceInPrev && relpos_curr == relpos_prev)
        {
            tarPiece = statePtr->current[tarPos.x][tarPos.y];

            if(tarPiece != NULL && piece->returnColour() != tarPiece->returnColour())
                invalidTakes.push(tarPos);
            else
                invalidMoves.push(tarPos);

            continue;
        }

        // Otherwise, fetch the piece in the currently targetted position
        tarPiece = statePtr->current[tarPos.x][tarPos.y];

        // Check for a piece in the current state
        if(tarPiece == NULL)
        {
            // If there's nothing there, the move is valid. We can push it to the valid move list and continue as normal.
            validMoves.push(tarPos);
            // There's no piece, so our if condition at the beginning won't fail.
            pieceInPrev = 0;

        }
        // If it isn't null, that means there's something there. We need to check if it's the same colour.
        else 
        {
                       
            if (tarPiece->returnColour() != piece->returnColour())
            {
                // If it's a pawn, we can't take forward
                if(piece->returnDescriptor().type == PAWN)
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





void BoardMoves::addSpecialMoves(Piece* currentPiece, PositionQueue* validMoves)
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
                
                Piece* rookRight = statePtr->current[7][curPos.y];
                Piece* rookLeft  = statePtr->current[0][curPos.y];              

                bool rightRookConditions = (rookRight != NULL && rookRight->returnDescriptor().type == ROOK && !rookRight->hasMoved() 
                    && rookRight->returnColour() == currentPiece->returnColour()) ? 1 : 0;
                bool leftRookConditions = (rookLeft != NULL && rookLeft->returnDescriptor().type == ROOK && !rookLeft->hasMoved()
                    && rookLeft->returnColour() == currentPiece->returnColour()) ? 1 : 0;

                bool clearRight = (statePtr->current[5][curPos.y] == NULL && statePtr->current[6][curPos.y] == NULL) ? 1 : 0;
                bool clearLeft  = (statePtr->current[3][curPos.y] == NULL && statePtr->current[2][curPos.y] == NULL) ? 1 : 0;

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
void BoardMoves::addSpecialTakes(Piece* currentPiece, PositionQueue* validTakes)
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
                    Piece* targetPiece = statePtr->current[temp.x][temp.y];
                    
                    if(targetPiece != NULL && targetPiece->returnColour() == oppositeCol)
                        validTakes->push(temp);
                }

                    // CHECK FOR EN-PASSANT: Must be on the row adjacent to backpawn line
                if(curPos.y == enpassantRow && curPos.validUpdate(i, 0))
                {
                    Piece* targetPiece;
                    targetPiece = statePtr->current[curPos.x + i][enpassantRow];
                    if(targetPiece == NULL)
                        continue;
                    
                    // Check for an existence piece, and that it's a pawn, and that it's the opposite colour (black)
                    bool pawnAdjacent = (targetPiece->returnDescriptor().type == PAWN && targetPiece->returnColour() == oppositeCol) ? 1 : 0;

                    // Check to see if it was on the home row last move
                    Piece* targetPiecePrev = statePtr->previous[curPos.x + i][enpassantRow + 2*forward];
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



/****************************************************/
/*                      RETURNS                     */
/****************************************************/

MovementQueue BoardMoves::returnMovementQueue(BoardPosition pos)
{
    return movementState[pos.x][pos.y];
}



/****************************************************/
/*           MOVEMENT-RELATED FUNCTIONS (PUBLIC)     */
/****************************************************/



/****************************************************/
/*             CHECK-RELATED FUNCTIONS              */
/****************************************************/

/* METHOD ONE FOR CASTLING:
* 1) Generate the movement queues of ALL pieces
* 2) Check if the king is being checked by anything
* 3) Add as conditional
*/


PieceChecks BoardMoves::pieceChecks(PIECE_ID ID)
{
    // // Queue for checked pieces
    PieceChecks checkedPieces;

    // // Get the piece...
    // Piece* piecePtr = Piece::returnIDPtr(ID);

    // // Generate the queue of all its pieces with special takes
    // PositionQueue moveRange = piecePtr->moveRange();
    // MovementQueue moveQueue = processMoveRange(moveRange, piecePtr->returnPosition());
    // addSpecialTakes(piecePtr, &moveQueue.validTakes);


    // // Iteate through list of takes and add ID to checkedPieces.
    // while(!moveQueue.validTakes.empty())
    // {
    //     BoardPosition targetPos = moveQueue.validTakes.front();
    //     moveQueue.validTakes.pop();
    //     checkedPieces.push(statePtr->current[targetPos.x][targetPos.y]->returnID());
    // }
    // return std::make_pair(ID, checkedPieces);
    return checkedPieces;
}
ChecksQueue BoardMoves::generateChecksList(PIECE_COLOUR col)
{
    // Initialise return
    ChecksQueue checksQueue;

    // // Iterate over all board positions
    // for(int i = 0; i < 8; i++)
    // {
    //     for(int j = 0; j < 8; j++)
    //     {
    //         PieceChecks tempQueue;
    //         PIECE_ID tempID;
    //         // Collect the current piece from state
    //         Piece* piecePtr = statePtr->current[i][j];

    //         // Skip over it if it's null or it's a different colour to the one we're checking
    //         if(piecePtr == NULL || piecePtr->returnColour() != col)
    //             continue;
    //         // Else, fetch the list of pieces that that piece checks
    //         else
    //         {
    //             tempID = piecePtr->returnID();
    //             tempQueue = pieceChecks(tempID);
    //         }
                
    //         checksQueue.push(tempQueue);
    //     }
    // }
    // // statePtr->currentChecks = checksQueue;
    return checksQueue;
}
bool BoardMoves::isChecked(PIECE_ID ID)
{

    // // Retrieve pointer of interest
    // Piece* piecePtr = Piece::returnIDPtr(ID);

    // // Fetch the list of pieces checked by the opposite colour
    // PIECE_COLOUR col = (piecePtr->returnColour() == WHITE) ? BLACK : WHITE;

    // // If the queue is empty, generate it
    // if(statePtr->currentChecks.empty())
    // {
    //     generateChecksList(col);
    // }
    
    // // Iterate through the checks list
    // ChecksQueue queueToCheck = statePtr->currentChecks;

    // bool found = 0;
    // while(!queueToCheck.empty() && !found)
    // {
    //     PieceChecks tempQueue = queueToCheck.front();
    //     queueToCheck.pop();

    //     while(!tempQueue.second.empty())
    //     {
    //         found = (ID == tempQueue.second.front()) ? 1 : 0;
    //         tempQueue.second.pop();
    //     }
    // }
    return 0;
}
bool BoardMoves::isCheckmated(PIECE_ID ID)
{
    return 0;
}
bool BoardMoves::canCastle(PIECE_ID ID, RELPOS relpos)
{
    /* POTENTIAL CODING DESIGN FLAW */
    // Modifying the 'current state' struct and returning it. Potential for going wrong, but seems a neat trick....

    // BoardState stateToRestore = state;
    // BoardPosition kingPos = Piece::returnIDPtr(ID)->returnPosition();
    // Piece* kingPtr = statePtr->current[kingPos.x][kingPos.y];
    
    // // // Decide if we're castling right or left
    // int castlingDirection = (relpos == RIGHT) ? 1 : -1;

    // // // Temporarily add the king to the new positions
    // statePtr->current[kingPos.x + castlingDirection][kingPos.y] = kingPtr;
    // statePtr->current[kingPos.x + 2*castlingDirection][kingPos.y] = kingPtr;

    // // Run is checked!
    // bool inCheck = isChecked(ID);

    // // Restore the statePtr->..
    // state = stateToRestore;

    // // Done :)
    // return !inCheck;
    return 0;
}

PIECE_ID BoardMoves::fetchKingID(PIECE_COLOUR col)
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