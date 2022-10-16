/***********************************************************
 *                      BOARD.h
 * 
 *  Handles all board logic:
 *      - Keeping track of board state
 *      - Moving pieces in board state
 *      - Telling SDL_Board to update screen #### POSSIBLY NOT THE BEST PLACE FOR THIS
 * 
 * 
 * 
 ***********************************************************/

#pragma once

// INTERNAL INCLUDES
#include "Piece.h"
#include "BoardPosition.h"

// EXTERNAL INCLUDES
#include <SDL2/SDL.h>
#include <utility>

typedef std::queue<BoardPosition> PositionQueue;
typedef std::queue<PIECE_ID> IDQueue;
typedef std::pair<bool, PIECE_ID> PLAY_FLAG;

struct BoardState {
    Piece* piecesCurr[8][8];
    Piece* piecesPrev[8][8];

    BoardState() {
    for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                piecesCurr[i][j] = NULL;
                piecesPrev[i][j] = NULL; 
            };
        };
    };
};

struct BOARD_FLAGS
{
    PLAY_FLAG kingCheck;
    PLAY_FLAG kingCheckmate;
    PLAY_FLAG pawnPromotion;

    BOARD_FLAGS()
    {
        kingCheck.first = 0;
        kingCheck.second = 0;
        kingCheckmate.first = 0;
        kingCheckmate.second = 0;
        pawnPromotion.first = 0;
        pawnPromotion.second = 0;
    }
    void RESET_FLAGS()
    {
        kingCheck.first = 0;
        kingCheck.second = 0;
        kingCheckmate.first = 0;
        kingCheckmate.second = 0;
        pawnPromotion.first = 0;
        pawnPromotion.second = 0;
    }
};

struct MovementQueue {
    PositionQueue validMoves;
    PositionQueue validTakes;
    PositionQueue invalidMoves;
    PositionQueue invalidTakes;

    MovementQueue() {};
    MovementQueue(PositionQueue validMoves_, PositionQueue validTakes_, 
                    PositionQueue invalidMoves_, PositionQueue invalidTakes_)
        :   validMoves(validMoves_), validTakes(validTakes_),
            invalidMoves(invalidMoves_), invalidTakes(invalidTakes_) {};
};

class Board {

    protected:
        // MEMBER VARIABLES
        BoardState state;
        BOARD_FLAGS boardFlags;

        // PRIVATE HELPER FUNCTIONS FOR MOVEMENT
        void movePiece(Piece* curPiece, BoardPosition newPos);
        void takePiece(Piece* curPiece, BoardPosition newPos);
        void removePiece(Piece* pieceToDelete);
        

    public:
        // CONSTRUCTORS
        Board() {};
        ~Board() {};
        
        
        // STATE-RELATED FUNCTIONS
        void initialiseBoard();
        void clearBoard();
        void addPieceToState(Piece* newPiece);
        bool processUpdate(BoardPosition oldPos, BoardPosition newPos);
        void removePiece(PIECE_ID pieceToDelete);
        
        
        // MOVEMENT-RELATED FUNCTIONS
        MovementQueue generateMovementRange(BoardPosition oldPos);
        MovementQueue processMoveRange(PositionQueue moveRange, BoardPosition curPiecePos);
        void addSpecialMoves(Piece* currentPiece, PositionQueue* validMoves);
        void addSpecialTakes(Piece* currentPiece, PositionQueue* validTakes);
        
        // CHECK-RELATED FUNCTIONS
        IDQueue pieceChecks(PIECE_ID ID);
        IDQueue generateCheckedList(PIECE_COLOUR col);
        bool isChecked(PIECE_ID ID);
        bool canCastle(PIECE_ID ID, RELPOS relpos);

        // FLAG RELATED FUNCTIONS
        void resetFlags() {boardFlags.RESET_FLAGS();};

        // RETURNS
        BoardState returnState() {return state;};   
        BOARD_FLAGS returnFlags() {return boardFlags;};
        
        
};