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
bool elementInQueue(T searchQuery, std::queue<T> queueToSearch)
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
void BoardMoves::changeState(BoardState* statePtr)
{
    // Clear vectors
    whitePieces.clear();
    blackPieces.clear();

    // Iterate through all positions, extract 

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(statePtr->current[i][j].type() == NONE)
                continue;
            else if (statePtr->current[i][j].colour() == WHITE)
            {
                whitePieces.push_back(&statePtr->current[i][j]);
                if(statePtr->current[i][j].type() == KING)
                    whiteKing = &statePtr->current[i][j];
            }
            else
            {
                blackPieces.push_back(&statePtr->current[i][j]);
                if(statePtr->current[i][j].type() == KING)
                    whiteKing = &statePtr->current[i][j];
            }
        }
    }
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
    // Set colours
    curCol = statePtr->turn();
    oppCol = (curCol == WHITE) ? BLACK : WHITE;

    // Set forward direction
    curForward = (curCol == WHITE) ? 1 : -1;

    if(curCol == WHITE)
    {
        curPieces   = whitePieces;
        curKing     = whiteKing;
        oppPieces   = blackPieces;
        oppKing     = blackKing;
    }
    else
    {
        curPieces   = blackPieces;
        curKing     = blackKing;
        oppPieces   = whitePieces;
        oppKing     = whiteKing;
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
    for(auto it = curPieces.begin(); it != curPieces.end(); it++)
        generateMovementRange(*it);
    for(auto it = oppPieces.begin(); it != oppPieces.end(); it++)
        generateMovementRange(*it);

    // 3) Generate checks
    curChecks = returnChecksVector(curPieces);
    oppChecks = returnChecksVector(oppPieces);

    // 4) Restrict moves that put the king into check
    restrictRevealedCheckMoves(curKing);
    
    // 4) Check if the King is in check
    PieceVector kingCheckedBy = pieceCheckedBy(curKing, oppChecks);
    
    // 5) Set flags
    if(curKing != NULL && !kingCheckedBy.empty()) {
        // Set check flags
        kingCheck.first = 1;
        kingCheck.second = curKing;

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
    if(!validPiece(piece))
        return;

    // Process the movement range from the current position
    moveQueue = processMoveRange(piece, piece->moveRange());

    // Add special takes (pawn takes & enpassant)
    addSpecialTakes(piece, &moveQueue.validTakes);

    // Add queue to appropriate board location
    BoardPosition pos(piece->position());
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
    SightedQueue  sightedQueue;

    // Fetch current values
    BoardPosition pos(piece->position());

    // Initialise target values
    BoardPosition tarPos;
    Piece* tarPiece;

    // Bool to check pieces
    bool pieceInPrev = 1;

    PIECE_TYPE pieceType = piece->type();


    // Knight algorithm: Special case, don't need to think about all the other bits
    if(piece != NULL && piece->type() == KNIGHT)
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
                validMoves.push(tarPos);
            }
            else if (tarPiece->colour() == piece->colour())
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
        tarPiece = pieceByPosition(tarPos);

        // If there's a piece there, add it to our sighted pieces
        if(tarPiece != NULL)
                sightedQueue.push(std::make_pair(tarPiece, relpos_curr));
        
        // If we added a piece last time and we haven't changed the relative direction, we need to continue to the next item in the queue.
        // If we did add a piece last time, we need to see that piece has the same relative direction as the previous position.
        if(relpos_prev != SAME && pieceInPrev && relpos_curr == relpos_prev)
        {
            if(tarPiece != NULL && piece->colour() != tarPiece->colour())
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
            if (tarPiece->colour() != piece->colour())
            {
                // If it's a pawn, we can't take forward
                if(piece->type() == PAWN)
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

PieceVector BoardMoves::positionCheckedBy(BoardPosition pos, PIECE_COLOUR oppCol)
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

    // Iterate through oppoonents pieces, removing positions from our current pieces valid moves that appear in the opposing pieces list
    PositionQueue emptyQueue;
    return emptyQueue;
}
PositionQueue BoardMoves::returnSafeTakes(Piece* piece)
{
    // Fetch vector of curent pieces
    PIECE_COLOUR  col = piece->colour();
    MovementQueue pieceMovementQueue = returnMovementQueue(piece);
    std::vector<BoardPosition> validTakes = queueToVector(pieceMovementQueue.validTakes);

    PositionQueue emptyQueue;
    return emptyQueue;

    // Iterate through queue, removing positions from validTakes that appear in the opposing pieces list
}
void BoardMoves::restrictRevealedCheckMoves(Piece* piece)
{
    // Fetch the piece colour
    // PIECE_COLOUR col = piece->colour();
    // BoardPosition curPos(piece->x(), piece->y());

    // Fetch all pieces of the opposite colour
    // PieceVector opposingPieces = statePtr->returnPieceQueue((col == WHITE) ? BLACK : WHITE);

    // See which pieces have the piece as an invalid check
    // while(!opposingPieces.empty())
    // {
    //     // 1) Fetch front piece and it's position
    //     BoardPosition oppPos(opposingPieces.front()->x(), opposingPieces.front()->y());
    //     MovementQueue oppMovQueue = returnMovementQueue(oppPos);
    //     opposingPieces.pop();

    //     // 2) Search the front pieces' invalid takes queue for our position
    //     PositionQueue invalidTakes = oppMovQueue.invalidTakes;
    //     bool found = elementInQueue(invalidTakes, curPos);
        
    //     // 3) If we've found the king as an invalid target, we need to make sure the piece blocking this line of sight doesn't move       
    //     if(found)
    //     {
    //         // 3a) Fetch the relative position of the piece that's targetting our piece
    //         RELPOS relpos = BoardPosition::returnRelPos(oppPos, curPos); 

    //         // 3b) Fetch the pieces that are in the line of sight of our opposing piece
    //         SightedQueue oppSightline = oppMovQueue.sightedQueue;
    //         PieceVector sightedPieces;
            
    //         // 3c) Run through this queue, check if the piece is in the same relative direction, and wipe it's queue
    //         while(!oppSightline.empty())
    //         {
    //             Piece* temp = oppSightline.front().first;
    //             RELPOS sightedPos = oppSightline.front().second;
    //             oppSightline.pop();
                
    //             if(sightedPos == relpos)
    //                 sightedPieces.push(temp);                    
    //         }

    //         std::vector<Piece*> sightedPiecesVector = queueToVector(sightedPieces);
    //         int idx = 0;
            
    //         // Find the king in the vector
    //         for(; idx < sightedPiecesVector.size(); idx++) {
    //             if(sightedPiecesVector[idx] == piece)
    //                 break;
    //         }

    //         // If this is the only piece in the way, then it can't move
    //         if(sightedPiecesVector.size() <= 2)
    //         {
    //             Piece* temp = sightedPiecesVector[idx - 1];
    //             BoardPosition tempPos(temp->x(), temp->y());
    //             MovementQueue moveQueue = returnMovementQueue(temp);

    //             // Empty valid moves *BUG: DON'T NEED TO GET RID OF MOVEMENTS IN THE SAME RELATIVE DIRECTION*
    //             PositionQueue newInvalidMoves;
    //             PositionQueue stillValidMoves;

    //             while(!moveQueue.validMoves.empty())
    //             {
    //                 BoardPosition validPos = moveQueue.validMoves.front();
    //                 moveQueue.validMoves.pop();
    //                 if(BoardPosition::returnRelPos(oppPos, validPos) == BoardPosition::returnRelPos(oppPos, curPos))
    //                     stillValidMoves.push(validPos);
    //                 else
    //                     newInvalidMoves.push(validPos);
    //             }
    //             movementState[tempPos.x][tempPos.y].invalidMoves = appendToQueue(moveQueue.invalidMoves, newInvalidMoves);
    //             movementState[tempPos.x][tempPos.y].validMoves = stillValidMoves;

    //             PositionQueue newInvalidTakes;
    //             PositionQueue stillValidTakes;

    //             while(!moveQueue.validTakes.empty())
    //             {
    //                 BoardPosition validPos = moveQueue.validTakes.front();
    //                 moveQueue.validTakes.pop();

    //                 if(oppPos == validPos)
    //                     stillValidTakes.push(validPos);
    //                 else
    //                     newInvalidTakes.push(validPos);                    
    //             }

    //             movementState[tempPos.x][tempPos.y].invalidTakes = appendToQueue(moveQueue.invalidTakes, newInvalidTakes);
    //             movementState[tempPos.x][tempPos.y].validTakes = stillValidTakes;
                
    //         }
    //     }
    // }
}
void BoardMoves::restrictKingMoves(PIECE_COLOUR col)
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
void BoardMoves::removeInvalidCheckedMoves(PieceVector kingCheckedBy)
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
    // PieceVector currentPieces = statePtr->returnPieceQueue(curCol);

    // // If the checkedBy queue is greater than 1, remove all other moves
    // if(kingCheckedBy.size() > 1)
    // {
    //     while(!currentPieces.empty())
    //     {
    //         PositionQueue emptyPosQueue;
    //         if(currentPieces.front() != curKing)
    //         {
    //             BoardPosition pos(currentPieces.front()->x(), currentPieces.front()->y());
    //             currentPieces.pop();

    //             movementState[pos.x][pos.y].validMoves = emptyPosQueue;
    //             movementState[pos.x][pos.y].validTakes = emptyPosQueue;
    //         }
    //         else
    //             currentPieces.pop();
    //     }
    // }
    // // Else, for each piece, restrict its movement/takes to those which take the target piece
    // else
    // {
    //     while(!currentPieces.empty())
    //     {
    //         if(currentPieces.front() != curKing)
    //         {
    //             BoardPosition pos(currentPieces.front()->x(), currentPieces.front()->y());
    //             restrictToBlockingMoves(curKing, currentPieces.front(), kingCheckedBy.front(),
    //                 &movementState[pos.x][pos.y].validMoves, &movementState[pos.x][pos.y].validTakes);
    //             currentPieces.pop();
    //         }
    //         else
    //             currentPieces.pop();
    //     }
    // }
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

ChecksVector BoardMoves::returnChecksVector(const PieceVector& pieceVector)
{
    // Initialise return and all of the relevant pieces
    ChecksVector checksVector;

    for(auto it:pieceVector)
        checksVector.push_back(pieceChecks(it));

    return checksVector;
}


/****************************************************/
/*            PRIVATE HELPER-METHODS                */
/****************************************************/

void BoardMoves::addSpecialTakes(Piece* currentPiece, PositionQueue* validTakes)
{
    if(currentPiece == NULL)
        return;
    
    BoardPosition curPos(currentPiece->position());
    Move lastMove = statePtr->lastMove();
    

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
                    
                    if(targetPiece != NULL && targetPiece->colour() == oppCol)
                        validTakes->push(temp);
                }

                    // CHECK FOR EN-PASSANT: Must be on the row adjacent to backpawn line
                if(curPos.y == enpassantRow && curPos.validUpdate(i, 0) && currentPiece->colour() == curCol)
                {
                    Piece* targetPiece;
                    targetPiece = &statePtr->current[curPos.x + i][enpassantRow];
                    if(targetPiece == NULL)
                        continue;
                    
                    BoardPosition targetPos(targetPiece->position());

                    // Check for an existence piece, and that it's a pawn, and that it's the opposite colour (black)
                    bool pawnAdjacent = (targetPiece->type() == PAWN && targetPiece->colour() == oppCol) ? 1 : 0;
                    bool isLastMoved = (targetPos == lastMove.second);
                    bool lastOnHomeRow = (lastMove.first.y - lastMove.second.y == 2*curForward);
                    
                    if(pawnAdjacent && isLastMoved && lastOnHomeRow)
                        validTakes->push(curPos.returnUpdate(i, curForward));
                }
            }
            break;     
        }
        default: {};
    };
};
void BoardMoves::addCastling(Piece* king)
{
    
    if(king == NULL)
        return;
    
    BoardPosition curPos(king->position());
    PositionQueue* validMoves = &movementState[curPos.x][curPos.y].validMoves;
    PIECE_COLOUR oppCol = (king->colour() == WHITE) ? BLACK : WHITE;
 
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
