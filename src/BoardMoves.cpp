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
#include <functional>


/****************************************************/
/*                  HELPER FUNCTIONS                */
/****************************************************/

template <typename T>
std::vector<T> queueToVector(std::queue<T> queueToConvert)
{
    std::vector<T> returnVector;
    while(!queueToConvert.empty())
    {
        returnVector.push_back(queueToConvert.front());
        queueToConvert.pop();
    }
    return returnVector;
}

template <typename T>
std::queue<T> vectorToQueue(std::vector<T> vectorToConvert)
{
    std::queue<T> returnQueue;
    for(auto it = vectorToConvert.begin(); it != vectorToConvert.end(); it++)
        returnQueue.push(*it);
    return returnQueue;
}

template <typename T>
std::queue<T> appendToQueue(std::queue<T> queueToAppend, std::queue<T> appendQueue)
{
    while(!appendQueue.empty())
    {
        queueToAppend.push(appendQueue.front());
        appendQueue.pop();
    }
    return queueToAppend;
}

template <typename T>
void emptyQueue(std::queue<T>* queueToEmpty)
{
    std::queue<T> emptyQueue;
    *queueToEmpty = emptyQueue;
}


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
void BoardMoves::resetMoves()
{
    MovementQueue emptyMovementQueue;
    PieceChecks emptyChecksQueue;

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            movementState[i][j] = emptyMovementQueue;
            checksState[i][j] = emptyChecksQueue;
        }
    }
}
void BoardMoves::fetchTurn()
{
    curCol = statePtr->currentTurn;
    oppCol = (curCol == WHITE) ? BLACK : WHITE;
}


/****************************************************/
/*                  MAIN METHODS                    */
/****************************************************/
void BoardMoves::processState()
{
    // 0) Pre-process: Reset board flags, fetch current turn
    resetFlags();
    resetMoves();
    fetchTurn();

    // 1) Fetch queue of all pieces
    PieceQueue piecesQueue = statePtr->returnPieceQueue();

    // 2) Iterate through entire list, generating movement range of all pieces
    while(!piecesQueue.empty()) {
        generateMovementRange(piecesQueue.front());
        piecesQueue.pop();
    }

    // 3) Generate checks for opposite colour
    oppChecks = generateChecksList(oppCol);

    // 4) Check if the King is in check
    Piece* kingPtr = statePtr->returnPieceQueue(curCol, KING).front();
    if(isChecked(kingPtr->returnID(), oppChecks)) {
        // Set check flags
        kingCheck.first = 1;
        kingCheck.second = kingPtr->returnID();
    }
    else
    {
        // Add castling for the King
        addSpecialMoves(kingPtr);
    }

    // 5) Restrict the movement range of all pieces to avoid putting King into check
    refineMovementRange(curCol);
    
    // Done!
}

void BoardMoves::refineMovementRange(PIECE_COLOUR col)
{
    // 1) Restrict king moves that put him in check
    Piece* kingPtr = statePtr->returnPieceQueue(curCol, KING).front();
    BoardPosition kingPos = kingPtr->returnPosition();
    movementState[kingPos.x][kingPos.y].validMoves = returnNoncheckedMoves(kingPtr);

    // 2) Restrict moves that put the king into check
    restrictRevealedCheckMoves(kingPtr);

}


/****************************************************/
/*           MOVEMENT-RELATED FUNCTIONS (PUBLIC)     */
/****************************************************/


void BoardMoves::generateMovementRange(Piece* piece)
{
    // Initialise local movement queue variables
    MovementQueue moveQueue;

    // Check we've not got a null piece
    if(!statePtr->pieceExists(piece))
        return;

    // Process the movement range from the current position
    moveQueue = processMoveRange(piece, piece->moveRange());

    // Add special takes (pawn takes & enpassant)
    addSpecialTakes(piece, &moveQueue.validTakes);

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
    SightedQueue sightedQueue;

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
                sightedQueue.push(std::make_pair(tarPiece, relpos_curr));
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
        tarPiece = statePtr->current[tarPos.x][tarPos.y];

        // If there's a piece there, add it to our sighted pieces
        if(tarPiece != NULL)
                sightedQueue.push(std::make_pair(tarPiece, relpos_curr));
        
        // If we added a piece last time and we haven't changed the relative direction, we need to continue to the next item in the queue.
        // If we did add a piece last time, we need to see that piece has the same relative direction as the previous position.
        if(relpos_prev != SAME && pieceInPrev && relpos_curr == relpos_prev)
        {
            if(tarPiece != NULL && piece->returnColour() != tarPiece->returnColour())
                invalidTakes.push(tarPos);
            else
                invalidMoves.push(tarPos);

            

            continue;
        }       

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
                {
                    invalidMoves.push(tarPos);
                }
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

    MovementQueue proceesedQueue(validMoves, validTakes, invalidMoves, invalidTakes, sightedQueue);
    return proceesedQueue;  
}


void BoardMoves::addSpecialMoves(Piece* currentPiece)
{
    
    if(currentPiece == NULL)
        return;
    
    BoardPosition curPos = currentPiece->returnPosition();
    PositionQueue* validMoves = &movementState[curPos.x][curPos.y].validMoves;
    

    // Special moves list
    switch(currentPiece->returnDescriptor().type)
    {
        case KING:
        {
            if(!currentPiece->hasMoved())
            {
                
                Piece* rookRight = statePtr->current[7][curPos.y];
                Piece* rookLeft  = statePtr->current[0][curPos.y];

                auto rook_conditions = [=](Piece* tarRook)
                {
                    bool rookConditions = (tarRook != NULL && tarRook->returnDescriptor().type == ROOK && !tarRook->hasMoved() 
                    && tarRook->returnColour() == currentPiece->returnColour()) ? 1 : 0;
                    return rookConditions;
                };

                bool clearRight = (statePtr->current[5][curPos.y] == NULL && statePtr->current[6][curPos.y] == NULL) ? 1 : 0;
                bool clearLeft  = (statePtr->current[3][curPos.y] == NULL && statePtr->current[2][curPos.y] == NULL) ? 1 : 0;

                if(rookLeft == NULL || rookRight == NULL)
                    break;
                if (rook_conditions(rookRight) && clearRight && canCastle(currentPiece->returnID(), RIGHT))
                    validMoves->push(curPos.returnUpdate(2, 0));
                if (rook_conditions(rookLeft) && clearLeft  && canCastle(currentPiece->returnID(), LEFT))
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
/*             CHECK-RELATED FUNCTIONS              */
/****************************************************/


ChecksQueue BoardMoves::generateChecksList(PIECE_COLOUR col)
{
    // Initialise return and all of the relevant pieces
    ChecksQueue checksQueue;
    PieceQueue colouredPieces = statePtr->returnPieceQueue(col);

    while(!colouredPieces.empty())
    {
        checksQueue.push(pieceChecks(colouredPieces.front()));
        colouredPieces.pop();
    }
    
    return checksQueue;
}
PieceChecks BoardMoves::pieceChecks(Piece* piece)
{
    // Fetch the list of valid takes from movement state
    BoardPosition pos = piece->returnPosition();
    PositionQueue pieceTakes = movementState[pos.x][pos.y].validTakes;

    // Initialise a queue for pieces and the pieceChecks return (ID/queue pair)
    PieceQueue pieceQueue;
    PieceChecks pieceChecks;

    // Convert board positions to pieces    
    while(!pieceTakes.empty())
    {
        pieceQueue.push(statePtr->returnPiece(pieceTakes.front()));
        pieceTakes.pop();
    }

    // Return
    pieceChecks.first = piece->returnID();
    pieceChecks.second = pieceQueue;

    // Store information for future use and return
    checksState[pos.x][pos.y] = pieceChecks; 
    return pieceChecks;
}
bool BoardMoves::isChecked(PIECE_ID ID, ChecksQueue checksQueue)
{
    while(!checksQueue.empty())
    {
        PieceQueue currentQueue = checksQueue.front().second;
        checksQueue.pop();
        while(!currentQueue.empty())
        {
            if(ID == currentQueue.front()->returnID())
                return 1;
            currentQueue.pop();
        }
        
    }
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
    return 1;
}


/****************************************************/
/*              REFINEMENT-FUNCTIONS                */
/****************************************************/
PositionQueue BoardMoves::returnNoncheckedMoves(Piece* piece)
{
    // Fetch vector of curent pieces
    PIECE_COLOUR  col = piece->returnColour();
    MovementQueue pieceMovementQueue = returnMovementQueue(piece);
    std::vector<BoardPosition> validMoves = queueToVector(pieceMovementQueue.validMoves);

    // Fetch queue of opposing pieces
    PieceQueue opposingPieces = statePtr->returnPieceQueue(col == WHITE ? BLACK : WHITE); 

    // Iterate through queue, removing positions from validMoves that appear in the opposing pieces list
    while(!opposingPieces.empty())
    {
        PositionQueue curOppValidMoves = returnMovementQueue(opposingPieces.front()).validMoves;
        opposingPieces.pop();

        while(!curOppValidMoves.empty())
        {
            BoardPosition temp = curOppValidMoves.front();
            curOppValidMoves.pop();
            for(auto it = validMoves.begin(); it != validMoves.end(); it++)
            {
                if(temp == *it) {
                    validMoves.erase(it);
                    break;
                }
            }
        }
    }
    return vectorToQueue(validMoves);
}

void BoardMoves::restrictRevealedCheckMoves(Piece* piece)
{
    // Fetch the piece colour
    PIECE_COLOUR col = piece->returnColour();
    BoardPosition curPos = piece->returnPosition();

    // Fetch all pieces of the opposite colour
    PieceQueue opposingPieces = statePtr->returnPieceQueue((col == WHITE) ? BLACK : WHITE);

    // See which pieces have the King as an invalid check
    while(!opposingPieces.empty())
    {
        // 1) Fetch front piece
        BoardPosition oppPos = opposingPieces.front()->returnPosition();
        opposingPieces.pop();
        MovementQueue oppMovQueue = returnMovementQueue(oppPos);

        // 2) Fetch the front pieces' invalid takes queue for our current piece
        PositionQueue invalidTakes = oppMovQueue.invalidTakes;
        bool found = 0;
        while(!invalidTakes.empty() && !found)
        {
            BoardPosition oppCheckPos;
            oppCheckPos = invalidTakes.front();
            invalidTakes.pop();
            if(statePtr->returnPiece(oppCheckPos) == piece)
                found = 1;
        }

        /***********INCOMPLETE METHOD***********/
        // 3) If we've found the king as an invalid target, we need to make sure the piece blocking this line of sight doesn't move       
        if(found)
        {
            // 3a) Fetch the relative position of the piece that's targetting our piece
            RELPOS relpos = BoardPosition::returnRelPos(oppPos, curPos); 

            // 3b) Fetch the pieces that are in the line of sight of our opposing piece
            SightedQueue oppSightline = oppMovQueue.sightedQueue;
            PieceQueue sightedPieces;
            
            // 3c) Run through this queue, check if the piece is in the same relative direction, and wipe it's queue
            while(!oppSightline.empty())
            {
                Piece* temp = oppSightline.front().first;
                RELPOS sightedPos = oppSightline.front().second;
                oppSightline.pop();
                
                if(sightedPos == relpos)
                    sightedPieces.push(temp);                    
            }

            std::vector<Piece*> sightedPiecesVector = queueToVector(sightedPieces);
            int idx = 0;
            
            // Find the king in the vector
            for(; idx < sightedPiecesVector.size(); idx++) {
                if(sightedPiecesVector[idx] == piece)
                    break;
            }

            if(sightedPiecesVector.size() <= 2)
            {
                Piece* temp = sightedPiecesVector[idx - 1];
                MovementQueue moveQueue = returnMovementQueue(temp);
                movementState[temp->returnPosition().x][temp->returnPosition().y].invalidMoves = appendToQueue(moveQueue.invalidMoves, moveQueue.validMoves);
                emptyQueue(&movementState[temp->returnPosition().x][temp->returnPosition().y].validMoves);                                
            }
        }
    }
}





/****************************************************/
/*                      RETURNS                     */
/****************************************************/

MovementQueue BoardMoves::returnMovementQueue(BoardPosition pos)
{
    return movementState[pos.x][pos.y];
}
MovementQueue BoardMoves::returnMovementQueue(Piece* piece)
{
    BoardPosition pos = piece->returnPosition();
    return returnMovementQueue(pos);
}



