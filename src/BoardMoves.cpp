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
/*           HELPER FUNCTIONS (TO BE MOVED)         */
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

template <typename T>
bool elementInQueue(std::queue<T> queueToSearch, T searchQuery)
{
    while(!queueToSearch.empty())
    {
        if(queueToSearch.front() == searchQuery)
            return 1;
        queueToSearch.pop();
    }
    return 0;
}

template <typename T, typename Y>
std::queue<T> applyFunctionToQueue(std::queue<T> queueToActOn, Y (*func)(T))
{
    std::queue<Y> returnQueue;
    while(!queueToActOn.empty())
    {
        returnQueue.push(func(queueToActOn.front()));
        queueToActOn.pop();
    }
    return returnQueue;
}

template <typename T>
std::queue<T> returnMatchingElements(std::queue<T> queueToSearch, std::queue<T> queueToMatch)
{
    std::vector<T> vectorToMatch = queueToVector(queueToMatch);
    std::queue<T> matchedElementsQueue;

    while(!queueToSearch.empty())
    {
        for(auto it = vectorToMatch.begin(); it != vectorToMatch.end(); it++)
        {
            if(*it == queueToSearch.front())
            {
                // Assume no repeats
                matchedElementsQueue.push(queueToSearch.front());
                vectorToMatch.erase(it);
                break;
            }
        }
        queueToSearch.pop();
    }
    return matchedElementsQueue;
}


/****************************************************/
/*                  CONSTRUCTORS                    */
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

/****************************************************/
/*                     ADMIN                        */
/****************************************************/
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
void BoardMoves::setTurnDependencies()
{
    // Current colour
    curCol = statePtr->currentTurn;
    oppCol = (curCol == WHITE) ? BLACK : WHITE;

    // Current King
    curKing = statePtr->returnPieceQueue(curCol, KING).front();
    oppKing = statePtr->returnPieceQueue(oppCol, KING).front();

}

/****************************************************/
/*                  MAIN METHODS                    */
/****************************************************/
void BoardMoves::processState()
{ 
    // 0) Pre-process: Reset board flags, fetch current turn
    resetFlags();
    resetMoves();
    setTurnDependencies();

    // 1) Fetch queue of all pieces
    PieceQueue piecesQueue = statePtr->returnPieceQueue();

    // 2) Iterate through entire list, generating movement range of all pieces
    while(!piecesQueue.empty()) {
        generateMovementRange(piecesQueue.front());
        piecesQueue.pop();
    }
    
    // 3) Generate checks
    curChecks = returnChecksQueue(curCol);
    oppChecks = returnChecksQueue(oppCol);

    // 4) Restrict moves that put the king into check
    restrictRevealedCheckMoves(curKing);
    
    // 4) Check if the King is in check
    PieceQueue kingCheckedBy = pieceCheckedBy(curKing->returnID(), oppChecks);
    
    // 5) Set flags
    if(curKing != NULL && !kingCheckedBy.empty()) {
        // Set check flags
        kingCheck.first = 1;
        kingCheck.second = curKing->returnID();

        removeInvalidCheckedMoves(kingCheckedBy);
    }
    else
    {
        // Add castling for the King
        addCastling(curKing);
    }

    // 5) Restrict movements that put the king in check
    restrictKingMoves(curCol);

    // 6) Check if we have checkmate
    verifyMate();
    
    // Done!
}
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

    PIECE_TYPE pieceType = piece->returnType();


    // Knight algorithm: Special case, don't need to think about all the other bits
    if(piece != NULL && piece->returnType() == KNIGHT)
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
                if(piece->returnType() == PAWN)
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

    MovementQueue proceesedQueue(validMoves, validTakes, invalidMoves, invalidTakes, sightedQueue);
    return proceesedQueue;  
}


/****************************************************/
/*             CHECK-RELATED FUNCTIONS              */
/****************************************************/

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
PieceQueue BoardMoves::pieceCheckedBy(PIECE_ID ID, ChecksQueue oppChecks)
{
    PieceQueue checkedByQueue;
    while(!oppChecks.empty())
    {
        PIECE_ID checkingPieceID = oppChecks.front().first;
        PieceQueue currentQueue = oppChecks.front().second;
        
        while(!currentQueue.empty())
        {
            
            if(ID == currentQueue.front()->returnID())
                checkedByQueue.push(Piece::returnIDPtr(checkingPieceID));
            currentQueue.pop();
        }
        oppChecks.pop();
    }
    return checkedByQueue;
}
PieceQueue BoardMoves::positionCheckedBy(BoardPosition pos, PIECE_COLOUR oppCol)
{
    PieceQueue oppPieceQueue = statePtr->returnPieceQueue(oppCol);
    PieceQueue oppCheckingQueue;

    while(!oppPieceQueue.empty())
    {
        MovementQueue oppMoveQueue = returnMovementQueue(oppPieceQueue.front());
        bool found = elementInQueue(oppMoveQueue.validMoves, pos);
        if(found)
            oppCheckingQueue.push(oppPieceQueue.front());
        oppPieceQueue.pop();
    }

    return oppCheckingQueue;
}


/****************************************************/
/*              REFINEMENT-FUNCTIONS                */
/****************************************************/
PositionQueue BoardMoves::returnSafeMoves(Piece* piece)
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
PositionQueue BoardMoves::returnSafeTakes(Piece* piece)
{
    // Fetch vector of curent pieces
    PIECE_COLOUR  col = piece->returnColour();
    MovementQueue pieceMovementQueue = returnMovementQueue(piece);
    std::vector<BoardPosition> validTakes = queueToVector(pieceMovementQueue.validTakes);

    // Fetch queue of opposing pieces
    PieceQueue opposingPieces = statePtr->returnPieceQueue(col == WHITE ? BLACK : WHITE); 

    // Iterate through queue, removing positions from validTakes that appear in the opposing pieces list
    while(!opposingPieces.empty())
    {
        PositionQueue curOppInvalidMoves = returnMovementQueue(opposingPieces.front()).invalidMoves;
        opposingPieces.pop();

        while(!curOppInvalidMoves.empty())
        {
            BoardPosition temp = curOppInvalidMoves.front();
            curOppInvalidMoves.pop();
            for(auto it = validTakes.begin(); it != validTakes.end(); it++)
            {
                if(temp == *it) {
                    validTakes.erase(it);
                    break;
                }
            }
        }
    }
    return vectorToQueue(validTakes);
}
void BoardMoves::restrictRevealedCheckMoves(Piece* revealedPiece)
{
    // Fetch the piece colour
    PIECE_COLOUR col = revealedPiece->returnColour();
    BoardPosition curPos = revealedPiece->returnPosition();

    // Fetch all pieces of the opposite colour
    PieceQueue opposingPieces = statePtr->returnPieceQueue((col == WHITE) ? BLACK : WHITE);

    // See which pieces have the piece as an invalid check
    while(!opposingPieces.empty())
    {
        // 1) Fetch front piece and it's position
        BoardPosition oppPos = opposingPieces.front()->returnPosition();
        MovementQueue oppMovQueue = returnMovementQueue(oppPos);
        opposingPieces.pop();

        // 2) Search the front pieces' invalid takes queue for our position
        PositionQueue invalidTakes = oppMovQueue.invalidTakes;
        bool found = elementInQueue(invalidTakes, curPos);
        
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
                if(sightedPiecesVector[idx] == revealedPiece)
                    break;
            }

            // If this is the only piece in the way, then it can't move
            if(sightedPiecesVector.size() <= 2)
            {
                Piece* temp = sightedPiecesVector[idx - 1];
                BoardPosition tempPos = temp->returnPosition();
                MovementQueue moveQueue = returnMovementQueue(temp);

                // Empty valid moves *BUG: DON'T NEED TO GET RID OF MOVEMENTS IN THE SAME RELATIVE DIRECTION*
                PositionQueue newInvalidMoves;
                PositionQueue stillValidMoves;

                while(!moveQueue.validMoves.empty())
                {
                    BoardPosition validPos = moveQueue.validMoves.front();
                    moveQueue.validMoves.pop();
                    if(BoardPosition::returnRelPos(oppPos, validPos) == BoardPosition::returnRelPos(oppPos, curPos))
                        stillValidMoves.push(validPos);
                    else
                        newInvalidMoves.push(validPos);
                }
                movementState[tempPos.x][tempPos.y].invalidMoves = appendToQueue(moveQueue.invalidMoves, newInvalidMoves);
                movementState[tempPos.x][tempPos.y].validMoves = stillValidMoves;

                PositionQueue newInvalidTakes;
                PositionQueue stillValidTakes;

                while(!moveQueue.validTakes.empty())
                {
                    BoardPosition validPos = moveQueue.validTakes.front();
                    moveQueue.validTakes.pop();

                    if(oppPos == validPos)
                        stillValidTakes.push(validPos);
                    else
                        newInvalidTakes.push(validPos);                    
                }

                movementState[tempPos.x][tempPos.y].invalidTakes = appendToQueue(moveQueue.invalidTakes, newInvalidTakes);
                movementState[tempPos.x][tempPos.y].validTakes = stillValidTakes;
                
            }
        }
    }
}
void BoardMoves::restrictKingMoves(PIECE_COLOUR col)
{
    BoardPosition kingPos = curKing->returnPosition();
    movementState[kingPos.x][kingPos.y].validMoves = returnSafeMoves(curKing);
    movementState[kingPos.x][kingPos.y].validTakes = returnSafeTakes(curKing);
}
void BoardMoves::restrictToBlockingMoves(Piece* pieceToProtect, Piece* pieceToMove, Piece* checkingPiece,
                                PositionQueue* validMoves, PositionQueue* validTakes)
{
    // 1) Fetch positions of all pieces
    BoardPosition posToProtect  = pieceToProtect->returnPosition();
    BoardPosition posToMove     = pieceToMove->returnPosition();
    BoardPosition posOfCheck    = checkingPiece->returnPosition();
    
    PositionQueue posOfCheckQueue;
    posOfCheckQueue.push(posOfCheck);

    // 2) Fetch all relative positions between the check position and the position to protect
    PositionQueue blockingPositions = BoardPosition::returnRelativePositions(posOfCheck, posToProtect);
    
    // 3) Update moves with positions that match
    *validMoves = returnMatchingElements(blockingPositions, *validMoves);
    *validTakes = returnMatchingElements(posOfCheckQueue, *validTakes);
}
void BoardMoves::removeInvalidCheckedMoves(PieceQueue kingCheckedBy)
{

    /**********************************************
     * Assume the King is in check. Then, only the following moves are legal:
     * 
     * 1) King moves that move the king out of check (dealt with seperately)
     * 2) Moves that block the checking piece line of sight to the King
     * 3) Moves that take the checking piece
     * 
     * Both 2) and 3) are only valid if and only if only one piece is checking the king.
     * Otherwise, the king must move out of check
     * 
     ***********************************************/

    // Fetch current pieces
    PieceQueue currentPieces = statePtr->returnPieceQueue(curCol);

    // If the checkedBy queue is greater than 1, remove all other moves
    if(kingCheckedBy.size() > 1)
    {
        while(!currentPieces.empty())
        {
            PositionQueue emptyPosQueue;
            if(currentPieces.front() != curKing)
            {
                BoardPosition pos = currentPieces.front()->returnPosition();
                currentPieces.pop();

                movementState[pos.x][pos.y].validMoves = emptyPosQueue;
                movementState[pos.x][pos.y].validTakes = emptyPosQueue;
            }
            else
                currentPieces.pop();
        }
    }
    // Else, for each piece, restrict its movement/takes to those which take the target piece
    else
    {
        while(!currentPieces.empty())
        {
            if(currentPieces.front() != curKing)
            {
                BoardPosition pos = currentPieces.front()->returnPosition();
                restrictToBlockingMoves(curKing, currentPieces.front(), kingCheckedBy.front(),
                    &movementState[pos.x][pos.y].validMoves, &movementState[pos.x][pos.y].validTakes);
                currentPieces.pop();
            }
            else
                currentPieces.pop();
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
ChecksQueue BoardMoves::returnChecksQueue(PIECE_COLOUR col)
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


/****************************************************/
/*            PRIVATE HELPER-METHODS                */
/****************************************************/

void BoardMoves::addSpecialTakes(Piece* currentPiece, PositionQueue* validTakes)
{
    if(currentPiece == NULL)
        return;
    
    BoardPosition curPos = currentPiece->returnPosition();
    

    // Special moves list
    switch(currentPiece->returnType())
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
                    bool pawnAdjacent = (targetPiece->returnType() == PAWN && targetPiece->returnColour() == oppositeCol) ? 1 : 0;

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
void BoardMoves::addCastling(Piece* currentPiece)
{
    
    if(currentPiece == NULL)
        return;
    
    BoardPosition curPos = currentPiece->returnPosition();
    PositionQueue* validMoves = &movementState[curPos.x][curPos.y].validMoves;
    PIECE_COLOUR oppCol = (currentPiece->returnColour() == WHITE) ? BLACK : WHITE;
    

    // Special moves list
    switch(currentPiece->returnType())
    {
        case KING:
        {
            if(!currentPiece->hasMoved())
            {
                
                Piece* rookRight = statePtr->current[7][curPos.y];
                Piece* rookLeft  = statePtr->current[0][curPos.y];

                auto rook_conditions = [=](Piece* tarRook)
                {
                    bool rookConditions = (tarRook != NULL && tarRook->returnType() == ROOK && !tarRook->hasMoved() 
                    && tarRook->returnColour() == currentPiece->returnColour()) ? 1 : 0;
                    return rookConditions;
                };

                bool clearRight = (statePtr->current[5][curPos.y] == NULL && statePtr->current[6][curPos.y] == NULL) ? 1 : 0;
                bool clearLeft  = (statePtr->current[3][curPos.y] == NULL && statePtr->current[2][curPos.y] == NULL) ? 1 : 0;

                PieceQueue rightCheckedBy = positionCheckedBy(curPos.returnUpdate(1, 0), oppCol);
                PieceQueue leftCheckedBy = positionCheckedBy(curPos.returnUpdate(-1, 0), oppCol);

                if(rookLeft == NULL || rookRight == NULL)
                    break;
                if (rook_conditions(rookRight) && clearRight && rightCheckedBy.empty())
                    validMoves->push(curPos.returnUpdate(2, 0));
                if (rook_conditions(rookLeft) && clearLeft && leftCheckedBy.empty())
                    validMoves->push(curPos.returnUpdate(-2, 0));
            }
            break;
        }        
        default: {
            break;
        };
    }
    
}

void BoardMoves::verifyMate()
{
    bool moveFound = 0;
    if(kingCheck.first)
    {
        PieceQueue currentPieces = statePtr->returnPieceQueue(curCol);
        while(!currentPieces.empty() && !moveFound)
        {
            MovementQueue moveQueue = returnMovementQueue(currentPieces.front());
            currentPieces.pop();
            if(!moveQueue.validMoves.empty() || !moveQueue.validTakes.empty())
                moveFound = 1;
        }
    }
    kingCheckmate.first = !moveFound;
    kingCheckmate.second = curKing->returnID();
}

