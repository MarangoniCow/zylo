/***********************************************************
 *                BOARDMOVES IMPLEMENTATION
 ***********************************************************/

// INTERNAL INCLUDES
#include "Piece.h"
#include "BoardState.h"
#include "BoardMoves.h"
#include "BoardDefs.h"
#include "ContainerFunctions.h"

// EXTERNAL INCLUDES
#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <functional>



/***********************************************************
 *                    CONSTRUCTORS
 ***********************************************************/
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


/***********************************************************
 *                      ADMIN
 ***********************************************************/

void BoardMoves::resetFlags()
{
    kingCheck.first = false;
    kingCheck.second.resetPosition();
    kingCheckmate.first = false;
    kingCheckmate.second.resetPosition();
}

void BoardMoves::resetMoves()
{
    Movements emptyMovements;
    PieceChecks emptyChecks;

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            m_movements.state[i][j] = emptyMovements;
            m_checks.state[i][j] = emptyChecks;
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
            Piece& piece = statePtr->current[i][j];
            if(piece.type() == NONE)
                continue;
            else if (piece.colour() == curTurn)
            {
                curPieces.push_back(&piece);
            
                if(piece.type() == KING)
                    curKing = &piece;
            }
            else
            {
                oppPieces.push_back(&piece);
                if(piece.type() == KING)
                    oppKing = &piece;
            }
        }
    }
}


/***********************************************************
 *                    MAIN METHODS
 ***********************************************************/

void BoardMoves::processState()
{ 
    // 1) Pre-process: Reset board flags, fetch current turn
    resetFlags();
    resetMoves();
    setTurnDependencies();

    // 2) Generate movement ranges and checks for all pieces
    generateMovementRange();
    generateChecksState();

    // 3) Remove revealed check moves
    removeRevealedCheckMoves(curKing);
    removeRevealedCheckMoves(oppKing);
    
    // 4) Check if the King is in check (current king only)
    PieceVector kingCheckedBy;
    pieceCheckedBy(curKing, oppChecks, kingCheckedBy);
    
    // 5) Set flags
    if(curKing != NULL && !kingCheckedBy.empty()) {
        // Set check flags
        kingCheck.first = 1;
        kingCheck.second = curKing->position();

        restrictInvalidCheckedMoves(kingCheckedBy);
        addCastling(oppKing);
    }
    else
    {
        // Add castling for the King
        addCastling(curKing);
        addCastling(oppKing);
    }

    
    // 5) Restrict movements that put the king in check
    restrictKingMoves(curTurn);
    restrictKingMoves(oppTurn);

    // 6) Check if we have checkmate
    verifyMate();
    
    // Done!
}

void BoardMoves::processMoveRange(Piece* piece, PositionQueue moveRange, Movements& moveQueue)
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
                moveQueue.validMoves.push_back(tarPos);
            }
            else if (tarPiece->colour() == piece->colour())
            {
                moveQueue.invalidMoves.push_back(tarPos);
                moveQueue.sightedVector.push_back(Sightline(tarPiece, OUTOFLINE, 0));
            }
            else
            {
                moveQueue.validTakes.push_back(tarPos);
            }
        }
    }

    int N(0);

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
        {
            moveQueue.sightedVector.push_back(Sightline(tarPiece, relpos_curr, N++));
        }
        
        // If we added a piece last time and we haven't changed the relative direction, we need to continue to the next item in the queue.
        // If we did add a piece last time, we need to see that piece has the same relative direction as the previous position.
        if(relpos_prev != SAME && pieceInPrev && relpos_curr == relpos_prev)
        {
            if(validPiece(tarPiece) && piece->colour() != tarPiece->colour())
                moveQueue.invalidTakes.push_back(tarPos);
            else
                moveQueue.invalidMoves.push_back(tarPos);
            continue;
        }
        else
            N = 0;

        // Check for a piece in the current state
        if(!validPiece(tarPiece))
        {
            // If there's nothing there, the move is valid. We can push it to the valid move list and continue as normal.
            moveQueue.validMoves.push_back(tarPos);
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
                    moveQueue.invalidMoves.push_back(tarPos);
                else
                    moveQueue.validTakes.push_back(tarPos);
            }
            else
            {
                moveQueue.invalidMoves.push_back(tarPos);
            }
            // Regardless, we know that there's a piece in the previous location we looked. Back to the start.
            pieceInPrev = 1;
        }
    }
}


/***********************************************************
 *                    MOVEMENT & CHECK STATES
 ***********************************************************/

void BoardMoves::generateMovementRange()
{

    auto methodology = [=](Piece* piece)
    {
        // Check we've got a valid piece
        if(!validPiece(piece))
            return;

        // Process the movement range from the current position
        BoardPosition pos(piece->position());
        PositionQueue moveRange;
        piece->moveRange(moveRange);
        
        Movements& moveQueue = m_movements.state[pos.x][pos.y];
        processMoveRange(piece, moveRange, moveQueue);

        // Add special takes (pawn takes & enpassant)
        addPawnMoves(piece, moveQueue.validTakes);
    };

    for (auto piece:curPieces)
            methodology(piece);
    for (auto piece:oppPieces)
            methodology(piece);
}

void BoardMoves::generateChecksState()
{

    auto methodology = [=](Piece* piece)
    {
        // Check for valid piece
        if(!validPiece(piece))
            return;
        
        // Fetch the list of valid takes from movement state
        BoardPosition pos(piece->position());
        PositionVector& pieceTakes = m_movements.state[pos.x][pos.y].validTakes;

        // Initialise a queue for pieces and the pieceChecks return (ID/queue pair)
        PieceVector pieceVector;
        PieceChecks pieceChecks;

        for(auto it:pieceTakes)
            pieceVector.push_back(pieceByPosition(it));
        
        // Return
        pieceChecks.first  = piece;
        pieceChecks.second = pieceVector;

        // Store information for future use and return
        m_checks.state[pos.x][pos.y] = pieceChecks; 
        
        if(piece->colour() == curTurn)
            curChecks.push_back(pieceChecks);
        else
            oppChecks.push_back(pieceChecks);
    };

    for(auto piece:curPieces)
        methodology(piece);
    for(auto piece:oppPieces)
        methodology(piece);
    
    
}


/***********************************************************
 *                    SPECIAL MOVES
 ***********************************************************/

void BoardMoves::addPawnMoves(Piece* currentPiece, PositionVector& validTakes)
{
    if(!validPiece(currentPiece))
        return;
    
    BoardPosition   curPos(currentPiece->position());
    Move            lastMove = statePtr->lastMove();

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
                        validTakes.push_back(temp);
                }

                    // CHECK FOR EN-PASSANT: Must be on the row adjacent to backpawn line
                if(curPos.y == enpassantRow && curPos.validUpdate(i, 0) && currentPiece->colour() == curTurn)
                {
                    Piece* targetPiece = &statePtr->current[curPos.x + i][enpassantRow];
                    if(!validPiece(targetPiece, PAWN))
                        continue;
                    BoardPosition targetPos(targetPiece->position());

                    // Check for an existence piece, and that it's a pawn, and that it's the opposite colour
                    bool pawnAdjacent = (targetPiece->type() == PAWN && targetPiece->colour() != curCol) ? 1 : 0;
                    bool isLastMoved = (targetPos == lastMove.second);
                    bool lastOnHomeRow = (lastMove.first.y - lastMove.second.y == 2*curForward);
                    
                    if(pawnAdjacent && isLastMoved && lastOnHomeRow)
                        validTakes.push_back(curPos.returnUpdate(i, curForward));
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
    PositionVector& validMoves = m_movements.state[curPos.x][curPos.y].validMoves;
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
            validMoves.push_back(curPos.returnUpdate(2, 0));
        if (rook_condition(0, curPos.y) && clearLeft && leftCheckedBy.empty())
            validMoves.push_back(curPos.returnUpdate(-2, 0));
    }
}

void BoardMoves::verifyMate()
{
    bool moveFound = 0;
    if(kingCheck.first)
    {
        for(auto it:curPieces)
        {
            Movements movements = getMoves(it->position());
            if(!movements.validMoves.empty() || !movements.validTakes.empty())
            {
                moveFound = 1;
                break;
            }
        }
    }

    kingCheckmate.first = !moveFound;
    kingCheckmate.second = curKing->position();
}


/***********************************************************
 *                    CHECK-RELATED FUNCTIONS
 ***********************************************************/

void BoardMoves::pieceCheckedBy(Piece* piece, const ChecksVector& oppChecks, PieceVector& checkedByVector)
{
    for(auto it:oppChecks)
    {
        Piece* checkingPiece = it.first;
        PieceVector checkedVector = it.second;
        if(elementInVector(piece, checkedVector))
            checkedByVector.push_back(checkingPiece);
    }
}

PieceVector BoardMoves::positionCheckedBy(BoardPosition pos, COLOUR oppCol)
{
    
    PieceVector oppCheckingVector;

    for(auto it:oppPieces)
    {
        Movements& oppMoves = m_movements.state[it->x()][it->y()];
        if(elementInVector(pos, oppMoves.validMoves))
            oppCheckingVector.push_back(it);

    }
    return oppCheckingVector;
}


/***********************************************************
/*                  REFINEMENT-FUNCTIONS                
 ***********************************************************/

PositionVector BoardMoves::safeMoves(Piece* piece)
{
    
    COLOUR  col = piece->colour();
    PieceVector& enemyPieces = (col == curTurn) ? oppPieces : curPieces;
    PositionVector validMoves = m_movements.state[piece->x()][piece->y()].validMoves;
    BoardPosition curPos = piece->position();
    

    // Iterate through queue, removing positions from validMoves that appear in the opposing pieces list
    for(auto opp:enemyPieces)
    {
        PositionVector& curOppValidMoves = m_movements.state[opp->x()][opp->y()].validMoves;
        PositionVector& curOppInvalidMoves = m_movements.state[opp->x()][opp->y()].invalidMoves;
        BoardPosition oppPos = opp->position();
        RELPOS curRelPos = BoardPosition::returnRelPos(oppPos, curPos);

        for(auto temp:curOppValidMoves)
        {
            for(auto it = validMoves.begin(); it != validMoves.end(); it++)
            {
                if(temp == *it) {
                    validMoves.erase(it);
                    break;
                }
            }
        }

        // We also need to remove invalid moves that are in the same direction as the checking piece
        for(auto temp:curOppInvalidMoves)
        {
            for(auto it = validMoves.begin(); it != validMoves.end(); it++)
            {
                if(temp == *it && BoardPosition::returnRelPos(oppPos, temp) == curRelPos) {
                    validMoves.erase(it);
                    break;
                }
            }
        }
    }

    

    return validMoves;
}

PositionVector BoardMoves::safeTakes(Piece* piece)
{
    // Fetch vector of curent pieces
    COLOUR  col = piece->colour();
    PieceVector& enemyPieces = (col == curTurn) ? oppPieces : curPieces;

    // Vector to fill and vector to search    
    PositionVector newValidTakes;
    const PositionVector& validTakes = m_movements.state[piece->x()][piece->y()].validTakes;

    for(auto take:validTakes)
    {
        for(auto opp:enemyPieces)
        {
            SightedVector& oppSightVector = m_movements.state[opp->x()][opp->y()].sightedVector;

            for(auto sight:oppSightVector)
            {
                if(sight.tarPiece->position() == take && sight.n_Blocking == 0)
                    goto multiContinue;
            }
        }

        newValidTakes.push_back(take);
        multiContinue:;
    }
    return newValidTakes;
}

void BoardMoves::removeRevealedCheckMoves(Piece* piece)
{
    // Fetch the piece colour
    COLOUR col = piece->colour();
    BoardPosition curPos(piece->position());
    const PieceVector& pieceVector = (col == curTurn) ? curPieces : oppPieces;


    for(auto it:pieceVector)
    {
        // 1) Fetch front position and movement
        BoardPosition oppPos(it->position());
        Movements& oppMovQueue = m_movements.state[oppPos.x][oppPos.y];

        // 2) Search the front pieces' invalid takes queue for our position
        bool found = elementInVector(curPos, oppMovQueue.invalidTakes);

        // 3) If we've found the king as an invalid target, we need to make sure the piece blocking this line of sight doesn't move     
        if(found)
        {
            // 3a) Fetch the relative position of the piece that's targetting our piece
            RELPOS relpos = BoardPosition::returnRelPos(oppPos, curPos); 

            // 3b) Fetch the pieces that are in the line of sight of our opposing piece
            SightedVector& oppSightline = oppMovQueue.sightedVector;
            PieceVector sightedPieces;
            
            // 3c) Run through this queue, check if the piece is in the same relative direction, and wipe it's queue
            for(auto it:oppSightline)
            {
                Piece* temp = it.tarPiece;
                RELPOS sightedPos = it.relPos;

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
            if(idx <= 2)
            {
                Piece* temp = sightedPieces[idx - 1];
                Movements& movements = m_movements.state[temp->x()][temp->y()];

                // Empty valid moves *BUG: DON'T NEED TO GET RID OF MOVEMENTS IN THE SAME RELATIVE DIRECTION*
                PositionVector newInvalidMoves;
                PositionVector stillValidMoves;

                for(auto validPos:movements.validMoves)
                {
                    if(BoardPosition::returnRelPos(oppPos, validPos) == BoardPosition::returnRelPos(oppPos, curPos))
                        stillValidMoves.push_back(validPos);
                    else
                        newInvalidMoves.push_back(validPos);
                }

                appendToVector(movements.invalidMoves, newInvalidMoves);
                movements.validMoves = stillValidMoves;

                PositionVector newInvalidTakes;
                PositionVector stillValidTakes;

                for(auto validPos:movements.validTakes)
                {
                    if(oppPos == validPos)
                        stillValidTakes.push_back(validPos);
                    else
                        newInvalidTakes.push_back(validPos);   
                }

                appendToVector(movements.invalidTakes, newInvalidTakes);
                movements.validTakes = stillValidTakes;
            }
        }
    }
}

void BoardMoves::restrictKingMoves(COLOUR col)
{
    BoardPosition kingPos(curKing->position());
    m_movements.state[kingPos.x][kingPos.y].validMoves = safeMoves(curKing);
    m_movements.state[kingPos.x][kingPos.y].validTakes = safeTakes(curKing);
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
        for(auto piece:curPieces)
        {
            if(piece->type() == KING)
                continue;
            
            appendToVector (m_movements.state[piece->x()][piece->y()].validMoves,
                             m_movements.state[piece->x()][piece->y()].invalidMoves);
            appendToVector  (m_movements.state[piece->x()][piece->y()].validTakes,
                             m_movements.state[piece->x()][piece->y()].invalidTakes);
            m_movements.state[piece->x()][piece->y()].validMoves.clear();
            m_movements.state[piece->x()][piece->y()].validTakes.clear();
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
                                    m_movements.state[pos.x][pos.y].validMoves,
                                    m_movements.state[pos.x][pos.y].validTakes);
        }

        // If the king is in check, need to remove movements in the same direction as the checking piece
        BoardPosition curPos = curKing->position();
        PositionVector& kingValidMoves = m_movements.state[curKing->x()][curKing->y()].validMoves;
        for(auto piece:kingCheckedBy)
        {
            BoardPosition oppPos = piece->position();
            RELPOS oppRelPos    = BoardPosition::returnRelPos(oppPos, curPos);
            const Movements& oppMoves = getMoves(oppPos);

            for(auto pos:oppMoves.invalidMoves)
            {
                for(auto it = kingValidMoves.begin(); it != kingValidMoves.end(); it++)
                {
                    if(pos == *it && oppRelPos == BoardPosition::returnRelPos(oppPos, pos))
                    {
                        kingValidMoves.erase(it);
                        break;
                    }
                }
            }
        }        
    }
}

void BoardMoves::restrictToBlockingMoves(Piece* pieceToProtect, Piece* pieceToMove, Piece* checkingPiece,
                                PositionVector& validMoves, PositionVector& validTakes)
{
    // 1) Fetch positions of all pieces
    BoardPosition posToProtect(pieceToProtect->position());
    BoardPosition posToMove(pieceToMove->position());
    BoardPosition posOfCheck(checkingPiece->position());
    
    PositionVector posOfCheckVector;
    posOfCheckVector.push_back(posOfCheck);

    // 2) Fetch all relative positions between the check position and the position to protect
    PositionVector blockingPositions = BoardPosition::returnRelativePositions(posOfCheck, posToProtect);
    
    // 3) Update moves with positions that match
    validMoves = returnMatchingElements(blockingPositions, validMoves);
    validTakes = returnMatchingElements(posOfCheckVector, validTakes);
}


/****************************************************/
/*                      RETURNS                     */
/****************************************************/

Piece* BoardMoves::pieceByPosition(BoardPosition pos)
{
    Piece* piece = &statePtr->current[pos.x][pos.y];
    return piece;
}
