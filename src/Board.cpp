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
    // Fetch movement queue for current position
    MovementQueue moveQueue = boardMoves.returnMovementQueue(curPos);

    // Fetch the target piece, return if empty.
    Piece* currentPiece = state.current[curPos.x][curPos.y];
    if(!state.pieceExists(currentPiece, state.currentTurn))
        return 0;
    
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
    {
        // Update old state
        memcpy(state.previous, state.current, sizeof(state.current));
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
        memcpy(state.previous, state.current, sizeof(state.current));

        // Begin movement task flow
        preMoveTasks(curPos, tarPos);
        movePiece(currentPiece, tarPos);
        postMoveTasks();

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
    boardFlags.pawnPromotion.second = 0;
}
/****************************************************/
/*           MOVEMENT-RELATED FUNCTIONS (TASKS)     */
/****************************************************/

void Board::preMoveTasks(BoardPosition curPos, BoardPosition tarPos)
{
    lastMove.first = curPos;
    lastMove.second = tarPos;
}
void Board::postMoveTasks()
{
    state.currentTurn = (state.currentTurn == WHITE) ? BLACK : WHITE;
    boardMoves.processState();
}

void Board::movePiece(Piece* currentPiece, BoardPosition newPos)
{   
    BoardPosition curPos = currentPiece->position();

    // Update current position in state as null
    state.current[curPos.x][curPos.y] = NULL;

    // Update the target position in state with the current piece
    state.current[newPos.x][newPos.y] = currentPiece;

    // Update the piece position
    currentPiece->updatePosition(newPos);

    /* SPECIAL CASES: PROMOTION & CASTLING */
    switch (currentPiece->type())
    {
        case KING:
        {   
            // Right-side castling
            if(curPos.x - newPos.x == -2)
            {
                BoardPosition oldRookPos = newPos.returnUpdate(1, 0);
                BoardPosition newRookPos = newPos.returnUpdate(-1, 0);
                movePiece(state.current[oldRookPos.x][oldRookPos.y], newRookPos);
            }
            // Left-side castling
            else if(curPos.x - newPos.x == 2)
            {
                BoardPosition oldRookPos = newPos.returnUpdate(-2, 0);
                BoardPosition newRookPos = newPos.returnUpdate(1, 0);
                movePiece(state.current[oldRookPos.x][oldRookPos.y], newRookPos);
            }
            break;
        }
        case PAWN:
        {
            int endrow = (currentPiece->colour() == WHITE) ? 7 : 0;
            if(currentPiece->position().y == endrow)
            {
                boardFlags.pawnPromotion.first = 1;
                boardFlags.pawnPromotion.second = currentPiece->ID();
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
    if(currentPiece->type() == PAWN && state.current[newPos.x][newPos.y] == NULL)
    {
        int forward = (currentPiece->colour() == WHITE) ? 1 : -1;
        pieceToDelete = state.current[newPos.x][newPos.y - forward];
    }
    else
    {
        pieceToDelete = state.current[newPos.x][newPos.y];
    }
    state.removePiece(pieceToDelete);
    movePiece(currentPiece, newPos);

}

MovementQueue Board::returnMovementQueue(BoardPosition pos)
{
    return boardMoves.returnMovementQueue(pos);
}