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
typedef std::pair<PIECE_ID, IDQueue> PieceChecks;
typedef std::queue<PieceChecks> ChecksQueue;
typedef std::pair<bool, PIECE_ID> PLAY_FLAG;

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

struct BoardState {
    Piece* piecesCurr[8][8];
    Piece* piecesPrev[8][8];
    ChecksQueue currentChecks;
    BOARD_FLAGS boardFlags;
    PIECE_COLOUR currentTurn;

    BoardState()
    {
        currentTurn = WHITE;
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                piecesCurr[i][j] = NULL;
                piecesPrev[i][j] = NULL; 
            };
        };
    };
    void clearChecksQueue()
    {
        while(!currentChecks.empty())
        {
            currentChecks.pop();
        }
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
        void preMoveTasks();
        void postMoveTasks();
        
        
        // MOVEMENT-RELATED FUNCTIONS
        MovementQueue generateMovementRange(BoardPosition oldPos);
        MovementQueue processMoveRange(PositionQueue moveRange, BoardPosition curPiecePos);
        void addSpecialMoves(Piece* currentPiece, PositionQueue* validMoves);
        void addSpecialTakes(Piece* currentPiece, PositionQueue* validTakes);
        
        // CHECK-RELATED FUNCTIONS
        PieceChecks pieceChecks(PIECE_ID ID);
        ChecksQueue generateChecksList(PIECE_COLOUR col);
        bool isChecked(PIECE_ID ID);
        bool canCastle(PIECE_ID ID, RELPOS relpos);
        PIECE_ID fetchKingID(PIECE_COLOUR col);

        // FLAG RELATED FUNCTIONS
        void resetFlags() {state.boardFlags.RESET_FLAGS();};

        // RETURNS
        BoardState returnState() {return state;};   
        
        
};