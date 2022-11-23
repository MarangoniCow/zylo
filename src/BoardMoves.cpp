/***********************************************************
 *                BOARDMOVES IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "Piece.h"
#include "BoardState.h"
#include "BoardMoves.h"
#include "BoardDefs.h"

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
void matchingVectorElements(const T& itemToMatch, const std::vector<T>& vectorToSearch, std::vector<T>& returnVector)
{
    for(auto it:vectorToSearch)
    {
        if(itemToMatch == it)
            returnVector.push_back(it);
    }   
}

template <typename T>
bool elementInVector(const T& itemToMatch, const std::vector<T>& vectorToSearch)
{
    for(auto it:vectorToSearch)
        if(itemToMatch == it)
            return true;
    return false;
}

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
bool elementInQueue(const T& searchQuery, std::queue<T> queueToSearch)
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
BoardMoves::BoardMoves(BoardState* statePtr)
{
    changeState(statePtr);
    // Run process state
    processState();
}
void BoardMoves::changeState(BoardState* statePtr_)
{
    statePtr = statePtr_;
}

/****************************************************/
/*                     ADMIN                        */
/****************************************************/
void BoardMoves::resetFlags()
{
    kingCheck.first = 0;
    kingCheck.second = nullptr;
    kingCheckmate.first = 0;
    kingCheckmate.second = nullptr;
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
    // Set colours
    curTurn = statePtr->turn();
    oppTurn = (curTurn == WHITE) ? BLACK : WHITE;

    // Set forward direction
    curForward = (curTurn == WHITE) ? 1 : -1;

    curPieces.clear();
    oppPieces.clear();

    curChecks.clear();
    oppChecks.clear();

    /* IDEA: Use iterators to re-assign vector only if necessary */

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(statePtr->current[i][j].type() == NONE)
                continue;
            else if (statePtr->current[i][j].colour() == curTurn)
            {
                curPieces.push_back(&statePtr->current[i][j]);
            
                if(statePtr->current[i][j].type() == KING)
                    curKing = &statePtr->current[i][j];
            }
            else
            {
                oppPieces.push_back(&statePtr->current[i][j]);
                if(statePtr->current[i][j].type() == KING)
                    oppKing = &statePtr->current[i][j];
            }
        }
    }
}

/****************************************************/
/*                  MAIN METHODS                    */
/****************************************************/
void BoardMoves::processState()
{ 
    // 1) Pre-process: Reset board flags, fetch current turn
    resetFlags();
    resetMoves();
    setTurnDependencies();


    // 2) Iterate through entire list, generating movement range of all pieces
    for(auto it:curPieces) {
        generateMovementRange(it);
    }
    for(auto it:oppPieces) {
        generateMovementRange(it);
    }


    // // 3) Generate checks
    returnChecksQueue(curPieces, curChecks);
    returnChecksQueue(oppPieces, oppChecks);

    // // 4) Restrict moves that put the king into check
    restrictRevealedCheckMoves(curKing);
    
    // // 4) Check if the King is in check
    PieceVector kingCheckedBy = pieceCheckedBy(curKing, oppChecks);
    
    // 5) Set flags
    if(curKing != NULL && !kingCheckedBy.empty()) {
        // Set check flags
        kingCheck.first = 1;
        kingCheck.second = curKing;

        restrictInvalidCheckedMoves(kingCheckedBy);
    }
    else
    {
        // Add castling for the King
        addCastling(curKing);
    }

    // 5) Restrict movements that put the king in check
    restrictKingMoves(curTurn);

    // 6) Check if we have checkmate
    verifyMate();
    
    // Done!
}

void BoardMoves::generateMovementRange(Piece* piece)
{
    // Check we've not got a null piece
    if(!validPiece(piece))
        return;

    // Process the movement range from the current position
    PositionQueue moveRange;
    piece->moveRange(moveRange);
    
    MovementQueue moveQueue;   
    processMoveRange(piece, moveRange, moveQueue);

    // Add special takes (pawn takes & enpassant)
    addSpecialTakes(piece, moveQueue.validTakes);

    // Add queue to appropriate board location
    BoardPosition pos(piece->position());
    movementState[pos.x][pos.y] = moveQueue;
}

void BoardMoves::processMoveRange(Piece* piece, PositionQueue moveRange, MovementQueue& moveQueue)
{
    // Helper/local variables to help compare changes in position 
    RELPOS relpos_curr = SAME;
    RELPOS relpos_prev = SAME;  

    // Fetch current values
    BoardPosition pos(piece->position());

    // Initialise target values
    BoardPosition tarPos;
    Piece* tarPiece;

    // Bool to check pieces
    bool pieceInPrev = 1;

    PIECE_TYPE pieceType = piece->type();


    // Knight algorithm: Special case, don't need to think about all the other bits
    if(validPiece(piece) && piece->type() == KNIGHT)
    {
        while(!moveRange.empty())
        {
            // Fetch target position and piece
            tarPos = moveRange.front();
            moveRange.pop();
            tarPiece = pieceByPosition(tarPos);

            // Check if the target piece exists, and check its colour
            if (!validPiece(tarPiece))
            {
                moveQueue.validMoves.push(tarPos);
            }
            else if (tarPiece->colour() == piece->colour())
            {
                moveQueue.invalidMoves.push(tarPos);
                moveQueue.sightedQueue.push(std::make_pair(tarPiece, relpos_curr));
            }
            else
            {
                moveQueue.validTakes.push(tarPos);
            }
        }
    }

    while(!moveRange.empty())
    {
        // Fetch the target position
        tarPos = moveRange.front();
        moveRange.pop();
        
        // Update relative position
        relpos_prev = relpos_curr;   
        relpos_curr = BoardPosition::returnRelPos(pos, tarPos);
        tarPiece = pieceByPosition(tarPos);

        // If there's a piece there, add it to our sighted pieces
        if(validPiece(tarPiece))
                moveQueue.sightedQueue.push(std::make_pair(tarPiece, relpos_curr));
        
        // If we added a piece last time and we haven't changed the relative direction, we need to continue to the next item in the queue.
        // If we did add a piece last time, we need to see that piece has the same relative direction as the previous position.
        if(relpos_prev != SAME && pieceInPrev && relpos_curr == relpos_prev)
        {
            if(validPiece(tarPiece) && piece->colour() != tarPiece->colour())
                moveQueue.invalidTakes.push(tarPos);
            else
                moveQueue.invalidMoves.push(tarPos);
            continue;
        }       

        // Check for a piece in the current state
        if(!validPiece(tarPiece))
        {
            // If there's nothing there, the move is valid. We can push it to the valid move list and continue as normal.
            moveQueue.validMoves.push(tarPos);
            // There's no piece, so our if condition at the beginning won't fail.
            pieceInPrev = 0;
        }
        // If it isn't null, that means there's something there. We need to check if it's the same colour.
        else 
        {
            if (tarPiece->colour() != piece->colour())
            {
                // If it's a pawn, we can't take forward
                if(piece->type() == PAWN)
                    moveQueue.invalidMoves.push(tarPos);
                else
                    moveQueue.validTakes.push(tarPos);
            }
            else
            {
                moveQueue.invalidMoves.push(tarPos);
            }
            // Regardless, we know that there's a piece in the previous location we looked. Back to the start.
            pieceInPrev = 1;
        }
    }
}


/****************************************************/
/*             CHECK-RELATED FUNCTIONS              */
/****************************************************/

PieceChecks BoardMoves::pieceChecks(Piece* piece)
{
    // Fetch the list of valid takes from movement state
    BoardPosition pos(piece->position());
    PositionQueue pieceTakes = movementState[pos.x][pos.y].validTakes;

    // Initialise a queue for pieces and the pieceChecks return (ID/queue pair)
    PieceVector pieceVector;
    PieceChecks pieceChecks;

    // Convert board positions to pieces    
    while(!pieceTakes.empty())
    {
        pieceVector.push_back(pieceByPosition(pieceTakes.front()));
        pieceTakes.pop();
    }

    // Return
    pieceChecks.first = piece;
    pieceChecks.second = pieceVector;

    // Store information for future use and return
    checksState[pos.x][pos.y] = pieceChecks; 
    return pieceChecks;
}
PieceVector BoardMoves::pieceCheckedBy(Piece* piece, const ChecksVector& oppChecks)
{
    PieceVector checkedByVector;

    for(auto it:oppChecks)
    {
        Piece* checkingPiece = it.first;
        PieceVector checkedVector = it.second;
        matchingVectorElements(piece, checkedVector, checkedByVector);
    }

    return checkedByVector;
}

PieceVector BoardMoves::positionCheckedBy(BoardPosition pos, COLOUR oppCol)
{
    
    PieceVector oppCheckingVector;

    for(auto it:oppPieces)
    {
        MovementQueue oppMoveQueue = returnMovementQueue(it);
        if(elementInQueue(pos, oppMoveQueue.validMoves))
            oppCheckingVector.push_back(it);

    }
    return oppCheckingVector;
}


/****************************************************/
/*              REFINEMENT-FUNCTIONS                */
/****************************************************/
PositionQueue BoardMoves::returnSafeMoves(Piece* piece)
{
    // Fetch vector of curent pieces
    COLOUR  col = piece->colour();
    MovementQueue pieceMovementQueue = returnMovementQueue(piece);
    std::vector<BoardPosition> validMoves = queueToVector(pieceMovementQueue.validMoves);
    

    // Iterate through queue, removing positions from validMoves that appear in the opposing pieces list
    for(auto it:oppPieces)
    {
        PositionQueue curOppValidMoves = returnMovementQueue(it).validMoves;
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
    COLOUR  col = piece->colour();
    MovementQueue pieceMovementQueue = returnMovementQueue(piece);
    std::vector<BoardPosition> validTakes = queueToVector(pieceMovementQueue.validTakes);

    // Iterate through queue, removing positions from validTakes that appear in the opposing pieces list
    for(auto it:oppPieces)
    {
        PositionQueue curOppInvalidMoves = returnMovementQueue(oppPieces.front()).invalidMoves;
        
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
void BoardMoves::restrictRevealedCheckMoves(Piece* piece)
{
    // Fetch the piece colour
    COLOUR col = piece->colour();
    BoardPosition curPos(piece->position());

    for(auto it:oppPieces)
    {
        // 1) Fetch front position and movement
        BoardPosition oppPos(it->position());
        MovementQueue oppMovQueue = movementState[oppPos.x][oppPos.y];

        // 2) Search the front pieces' invalid takes queue for our position
        bool found = elementInQueue(curPos, oppMovQueue.invalidTakes);

        // 3) If we've found the king as an invalid target, we need to make sure the piece blocking this line of sight doesn't move     
        if(found)
        {
            // 3a) Fetch the relative position of the piece that's targetting our piece
            RELPOS relpos = BoardPosition::returnRelPos(oppPos, curPos); 

            // 3b) Fetch the pieces that are in the line of sight of our opposing piece
            SightedQueue oppSightline = oppMovQueue.sightedQueue;
            PieceVector sightedPieces;
            
            // 3c) Run through this queue, check if the piece is in the same relative direction, and wipe it's queue
            while(!oppSightline.empty())
            {
                Piece* temp = oppSightline.front().first;
                RELPOS sightedPos = oppSightline.front().second;
                oppSightline.pop();
                
                if(sightedPos == relpos)
                    sightedPieces.push_back(temp);                    
            }

            int idx = 0;
            
            // Find the king in the vector
            for(; idx < sightedPieces.size(); idx++) {
                if(sightedPieces[idx] == piece)
                    break;
            }

            // If this is the only piece in the way, then it can't move
            if(sightedPieces.size() <= 2)
            {
                Piece* temp = sightedPieces[idx - 1];
                BoardPosition tempPos(temp->position());
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
void BoardMoves::restrictKingMoves(COLOUR col)
{
    BoardPosition kingPos(curKing->position());
    movementState[kingPos.x][kingPos.y].validMoves = returnSafeMoves(curKing);
    movementState[kingPos.x][kingPos.y].validTakes = returnSafeTakes(curKing);
}
void BoardMoves::restrictToBlockingMoves(Piece* pieceToProtect, Piece* pieceToMove, Piece* checkingPiece,
                                PositionQueue* validMoves, PositionQueue* validTakes)
{
    // 1) Fetch positions of all pieces
    BoardPosition posToProtect(pieceToProtect->position());
    BoardPosition posToMove(pieceToMove->position());
    BoardPosition posOfCheck(checkingPiece->position());
    
    PositionQueue posOfCheckQueue;
    posOfCheckQueue.push(posOfCheck);

    // 2) Fetch all relative positions between the check position and the position to protect
    PositionQueue blockingPositions = BoardPosition::returnRelativePositions(posOfCheck, posToProtect);
    
    // 3) Update moves with positions that match
    *validMoves = returnMatchingElements(blockingPositions, *validMoves);
    *validTakes = returnMatchingElements(posOfCheckQueue, *validTakes);
}
void BoardMoves::restrictInvalidCheckedMoves(const PieceVector& kingCheckedBy)
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
    // If the checkedBy queue is greater than 1, remove all other moves
    if(kingCheckedBy.size() > 1)
    {
        for(auto it:curPieces)
        {
            if(it->type() == KING)
                continue;
            
            BoardPosition pos = it->position();
            emptyQueue(&movementState[pos.x][pos.y].validMoves);
            emptyQueue(&movementState[pos.x][pos.y].validTakes);
        }
    }
    // Else, for each piece, restrict its movement/takes to those which take the target piece
    else
    {
        for(auto it:curPieces)
        {
            if(it->type() == KING)
                continue;
            
            BoardPosition pos = it->position();
            restrictToBlockingMoves(curKing, it, kingCheckedBy.front(),
                                    &movementState[pos.x][pos.y].validMoves,
                                    &movementState[pos.x][pos.y].validTakes);
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
    BoardPosition pos(piece->position());
    return returnMovementQueue(pos);
}

void BoardMoves::returnChecksQueue(const PieceVector& pieceVector, ChecksVector& checksVector)
{
    for(auto it:pieceVector)
        checksVector.push_back(pieceChecks(it));
}


/****************************************************/
/*            PRIVATE HELPER-METHODS                */
/****************************************************/

void BoardMoves::addSpecialTakes(Piece* currentPiece, PositionQueue& validTakes)
{
    if(!validPiece(currentPiece))
        return;
    
    BoardPosition curPos(currentPiece->position());
    Move lastMove = statePtr->lastMove();

    // Need current 
    COLOUR curCol = currentPiece->colour();
    
    

    // Special moves list
    switch(currentPiece->type())
    {
        case PAWN:
        {
            int enpassantRow = (curCol == WHITE) ? 4 : 3;

            // GENERATE TAKE & EN-PASSANT: For loop repeats move queue for left/right instructions
            for(int i = -1; i <= 1; i += 2)
            {
                // CHECK FOR TAKE: Check if we're at the edge or not
                if(curPos.validUpdate(i, curForward))
                {
                    BoardPosition temp = curPos.returnUpdate(i, curForward);
                    Piece* targetPiece = &statePtr->current[temp.x][temp.y];
                    
                    if(validPiece(targetPiece) && targetPiece->colour() != curCol)
                        validTakes.push(temp);
                }

                    // CHECK FOR EN-PASSANT: Must be on the row adjacent to backpawn line
                if(curPos.y == enpassantRow && curPos.validUpdate(i, 0) && currentPiece->colour() == curTurn)
                {
                    Piece* targetPiece;
                    targetPiece = &statePtr->current[curPos.x + i][enpassantRow];
                    if(targetPiece == NULL)
                        continue;
                    
                    BoardPosition targetPos(targetPiece->position());

                    // Check for an existence piece, and that it's a pawn, and that it's the opposite colour
                    bool pawnAdjacent = (targetPiece->type() == PAWN && targetPiece->colour() != curCol) ? 1 : 0;
                    bool isLastMoved = (targetPos == lastMove.second);
                    bool lastOnHomeRow = (lastMove.first.y - lastMove.second.y == 2*curForward);
                    
                    if(pawnAdjacent && isLastMoved && lastOnHomeRow)
                        validTakes.push(curPos.returnUpdate(i, curForward));
                }
            }
            break;     
        }
        default: {};
    };
};
void BoardMoves::addCastling(Piece* king)
{
    
    if(!validPiece(king))
        return;
    
    BoardPosition curPos(king->position());
    PositionQueue* validMoves = &movementState[curPos.x][curPos.y].validMoves;
    COLOUR oppCol = (king->colour() == WHITE) ? BLACK : WHITE;
 
    if(!king->moved())
    {
        
        auto rook_condition = [=](int x, int y)
        {
            bool    rookExists = statePtr->current[x][y].type()     == ROOK;
            bool    rookCol    = statePtr->current[x][y].colour()   == king->colour();
            bool    rookMoved  = statePtr->current[x][y].moved()    == false;
            return  rookExists && rookCol && rookMoved;
        };

        bool        clearRight = !statePtr->pieceExists(BoardPosition(5, curPos.y)) 
                                && !statePtr->pieceExists(BoardPosition(6, curPos.y));
        bool        clearLeft  = !statePtr->pieceExists(BoardPosition(3, curPos.y))
                                && !statePtr->pieceExists(BoardPosition(2, curPos.y));
        
        PieceVector rightCheckedBy = positionCheckedBy(curPos.returnUpdate(1, 0), oppCol);
        PieceVector leftCheckedBy  = positionCheckedBy(curPos.returnUpdate(-1, 0), oppCol);

        if (rook_condition(7, curPos.y) && clearRight && rightCheckedBy.empty())
            validMoves->push(curPos.returnUpdate(2, 0));
        if (rook_condition(0, curPos.y) && clearLeft && leftCheckedBy.empty())
            validMoves->push(curPos.returnUpdate(-2, 0));
    }
}

void BoardMoves::verifyMate()
{
    bool moveFound = 0;
    if(kingCheck.first)
    {
        for(auto it:curPieces)
        {
            MovementQueue moveQueue = returnMovementQueue(it);
            if(!moveQueue.validMoves.empty() || !moveQueue.validTakes.empty())
            {
                moveFound = 1;
                break;
            }
        }
    }

    kingCheckmate.first = !moveFound;
    kingCheckmate.second = curKing;
}


Piece* BoardMoves::pieceByPosition(BoardPosition pos)
{
    Piece* piece = &statePtr->current[pos.x][pos.y];
    return piece;
}
