#pragma once

#include "utility.h"

#include <vector>

namespace schess
{
    enum class gridType : int
    {
        NONE = -1,   
        KING = 0,
        QUEEN,
        ROOK,
        BISHOP,
        KNIGHT,
        PAWN
    };

    enum class gridColor : int
    {
        NONE  = 0,
        WHITE = 1,
        BLACK = 2
    };

    inline gridColor operator! (const gridColor color)
    {
        return (color == gridColor::WHITE) ? gridColor::BLACK : gridColor::WHITE;
    }

    class boardGrid
    {
    public:

        gridType t  = gridType::NONE;
        gridColor c = gridColor::NONE;

        int id = -1;

        // position in the texture
        inline vec2<float> m_getVec() const 
        { 
            return { static_cast<float>(t), static_cast<float>(c) }; 
        }
        
        inline bool m_isEqual(const gridType type, const gridColor color) const
        { return t == type && c == color; }

        inline bool m_isEmpty() const { return t == gridType::NONE; }
        inline void m_empty() { *this = boardGrid{}; }
    };
    

    class gameLogic
    {
    public:

        gameLogic();

        void m_reset();

        bool m_playIfValid(const int srcInd, const int destInd);

        inline const boardGrid& m_getAt(const int i) const { return m_board[i]; }

        static inline constexpr int s_getIndex(const int x, const int y) { return   y * 8 +   x; }
        static inline int       	s_getIndex(const vec2<int> v)		 { return v.y * 8 + v.x; }
        static inline vec2<int>     s_getIndex(const int i) 			 { return { i % 8, i / 8 }; }


        // inline bool m_isSelectable(const int index) const { return m_board[index].c == m_playerColor; }
        inline bool m_isSelectable(const int index) const { return !m_board[index].m_isEmpty();       }
		
        void m_undoMove();

    private:
        boardGrid m_board[64] = {};

        gridColor m_enemyColor  = gridColor::BLACK;
        gridColor m_playerColor = gridColor::WHITE;
	
		
		int m_kingLocations[2] = {};
		
	private:

        struct singleMove
        {
            int srcInd = 0;
            int destInd = 0;

            boardGrid srcGrid;
            boardGrid destGrid;
        };

        inline singleMove m_createSingleMove(const int srcInd, const int destInd) const
        {
            return { srcInd, destInd, m_board[srcInd], m_board[destInd] };
        }

        struct moveRecord
        {
        public:
            singleMove first;
            std::optional<singleMove> second;
        };

        std::vector<moveRecord> m_records;
	
        // checks the last played move
        bool m_isPieceJustPlayed(const int id) const;
        bool m_isPiecePlayed    (const int id) const;

        void m_undoMove(const moveRecord& record);
        void m_undoMove(const singleMove& move);

	private:

        bool m_isKingInCheck(const gridColor kingColor) const;
        
        bool m_canBishopMove(int srcInd, int destInd, vec2<int> mov, vec2<int> movAbs) const;
        bool m_canRookMove  (int srcInd, int destInd, vec2<int> mov) const;


        bool m_canPieceMove(const int srcInd, const int destInd, moveRecord& move) const;
        void m_playMove    (const singleMove& move);
        
        bool m_isGridsEmpty   (int index, const int stopIndex, const int inc) const;
        bool m_isGridsAttacked(int index, const int stopIndex, const int inc, const gridColor attackerColor) const;
        bool m_isGridAttacked (const int index, const gridColor attackerColor) const;

        
        bool m_isGameOver(const gridColor color);
        bool m_isPossibleValidMove(const int srcInd, const int destInd);

        bool m_getMoveRecord(const int srcInd, const int destInd, moveRecord& move);

        static constexpr int s_getPieceValues(const gridType t);

        static const int s_maxStaticEvaluation;
        static const int s_minStaticEvaluation;

        int m_getStaticEvaluation() const;
        int m_minimaxBoard(const int depth, const bool maximizing, int alpha, int beta);

    public:
        void m_playEnemy();
    };

    

}