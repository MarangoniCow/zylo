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

Board::Board(BoardState state_) : state(state_) {};
void Board::newGame() {
    state.initialiseBoard();
    boardMoves.changeState(&state);
    boardMoves.processState();
}
void Board::newState(BoardState state_) {
    state = state_;
    boardMoves.changeState(&state);
    boardMoves.processState();
}


/****************************************************/
/*            STATE-RELATED FUNCTIONS               */
/****************************************************/
bool Board::processUpdate(BoardPosition curPos, BoardPosition tarPos)
{
    // Fetch the target piece, return if no piece present
    if(!state.pieceExists(curPos))
        return 0;

    Piece currentPiece = state.current[curPos.x][curPos.y];
    
    // Fetch queue from current position
    MovementQueue moveQueue = boardMoves.movementQueue(curPos);
    
    // Iterate through queue until the new position is found.
    bool found = 0;
    
    while(!moveQueue.validTakes.empty() && !found)
    {
        BoardPosition temp = moveQueue.validTakes.front();
        moveQueue.validTakes.pop();        
        if(temp == tarPos)
            found = 1;
    }

    if(found)
        takePiece(currentPiece, tarPos);
    else
    {
        while(!moveQueue.validMoves.empty() && !found)
        {
            BoardPosition temp = moveQueue.validMoves.front();
            moveQueue.validMoves.pop();        
            if(temp == tarPos)
                found = 1;
        }
    }       
    
    if(found)
    {
        // Begin movement task flow
        movePiece(currentPiece, tarPos);
        postMoveTasks(curPos, tarPos);
        return 1;
    }
    else
        return 0;

    
}
void Board::processPromotion(PIECE_TYPE newType)
{
    // Update state
    state.promotePiece(boardFlags.pawnPromotion.second, newType);

    // Update promotion flags
    boardFlags.pawnPromotion.first = 0;
    boardFlags.pawnPromotion.second.resetPosition();

    // Process the new state
    boardMoves.processState();
}
/****************************************************/
/*           MOVEMENT-RELATED FUNCTIONS (TASKS)     */
/****************************************************/

void Board::postMoveTasks(const BoardPosition& curPos, const BoardPosition& tarPos)
{
    
    state.lastMove(Move(curPos, tarPos));
    state.turn((state.turn() == WHITE) ? BLACK : WHITE);
    
    // Process the new state: We don't process the new state if there is a promotion, as we need to wait for the new unit first
    if(!boardFlags.pawnPromotion.first)
        boardMoves.processState();
}

void Board::movePiece(Piece &currentPiece, const BoardPosition& newPos)
{   
    // Move piece inside the state
    BoardPosition curPos = currentPiece.position();
    
	state.movePiece(currentPiece, newPos);

    /* SPECIAL CASES: PROMOTION & CASTLING */
    switch (currentPiece.type())
    {
        case KING:
        {   
            // Right-side castling
            if(curPos.x - newPos.x == -2)
            {
                BoardPosition oldRookPos(7, newPos.y);
                BoardPosition newRookPos = oldRookPos.returnUpdate(-2, 0);
                Piece temp = state.current[oldRookPos.x][oldRookPos.y];
                state.movePiece(temp, newRookPos);
            }
            // Left-side castling
            else if(curPos.x - newPos.x == 2)
            {
                BoardPosition oldRookPos(0, newPos.y);
                BoardPosition newRookPos = oldRookPos.returnUpdate(2, 0);
                Piece temp = state.current[oldRookPos.x][oldRookPos.y];
                state.movePiece(temp, newRookPos);
            }
            break;
        }
        case PAWN:
        {
            int endrow = (currentPiece.colour() == WHITE) ? 7 : 0;
            if(newPos.y == endrow)
            {
                boardFlags.pawnPromotion.first = true;
                boardFlags.pawnPromotion.second = newPos;
            }

            break;            
        }
        default: {
            break;
        }
    }
    
}


void Board::takePiece(Piece &currentPiece, const BoardPosition& newPos)
{
    Piece	pieceToDelete;

    // Need to implement special rules for enpassant
    if(currentPiece.type() == PAWN && state.current[newPos.x][newPos.y].type() == NONE)
    {
        int forward = (currentPiece.colour() == WHITE) ? 1 : -1;
        pieceToDelete = state.current[newPos.x][newPos.y - forward];
    }
    // Otherwise we can just take the piece in front of us
    else
    {
        pieceToDelete = state.current[newPos.x][newPos.y];
    }

    state.removePiece(pieceToDelete.position());
}

MovementQueue Board::movementQueue(BoardPosition pos)
{
    return boardMoves.movementQueue(pos);
}