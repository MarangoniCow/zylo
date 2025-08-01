/***********************************************************
 *                      GAMEPLAY HISTORY
 * 
 * Keeps track of the gameplay history
 * 
 ***********************************************************/

// INTERNAL INCLUDES
#include "Board.h"

// EXTERNAL INCLUDES
#include <vector>
#include <memory>

typedef unsigned int TURN;
typedef std::shared_ptr<BoardState> StatePtr;
typedef std::vector<StatePtr> HistoryVector;



class GameplayHistory
{
    protected:
        TURN			m_turnCount;
        HistoryVector	m_historyVec;
        
    public:
        GameplayHistory() {};
        GameplayHistory(const BoardState& initState) {
            appendHistory(initState);
        }
        ~GameplayHistory()
        {
            m_historyVec.clear();
        }
        
        void appendHistory(const BoardState &state);
        void truncateHistory(TURN n);
        void newGame(const BoardState& newState);
        StatePtr returnState(TURN n) const;
        TURN     returnTurn() const {return m_turnCount;};
};