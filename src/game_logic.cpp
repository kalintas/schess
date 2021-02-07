#include "game_logic.h"

#include <cstring>

namespace schess
{

    gameLogic::gameLogic() { m_reset(); }

    void gameLogic::m_reset()
    {
        static auto s_mirrorSet = 
        [&](const int x, const int y, const gridType type) 
        {
            const int enemyInd  = s_getIndex(x,     y);
            const int playerInd = s_getIndex(x, 7 - y); // reverse the y index

            m_board[enemyInd]  = { type, m_enemyColor , enemyInd };
            m_board[playerInd] = { type, m_playerColor, playerInd }; 
        };

        s_mirrorSet(0, 0, gridType::ROOK  );
        s_mirrorSet(1, 0, gridType::KNIGHT);
        s_mirrorSet(2, 0, gridType::BISHOP);
        s_mirrorSet(3, 0, gridType::QUEEN );
        s_mirrorSet(4, 0, gridType::KING  );
        s_mirrorSet(5, 0, gridType::BISHOP);
        s_mirrorSet(6, 0, gridType::KNIGHT);
        s_mirrorSet(7, 0, gridType::ROOK  );

        for(int i = 0; i < 8; ++i) s_mirrorSet(i, 1, gridType::PAWN);

        for(int i = 16; i < 48; ++i) m_board[i].m_empty();
		
		m_kingLocations[0] = s_getIndex(4, 7);
		m_kingLocations[1] = s_getIndex(4, 0);

    }

    // constexpr int gameLogic::s_getIndex(const int x, const int y) 
    // constexpr int gameLogic::s_getIndex(const vec2<int> v)        

    // constexpr vec2<int> gameLogic::s_getIndex(const int i) 



    bool gameLogic::m_isGridsEmpty(int index, const int stopIndex, const int inc) const
    {            
        while((index += inc) != stopIndex)
        {   
            if(!m_board[index].m_isEmpty()) return false;
        }

        return true;
    }

    bool gameLogic::m_isGridsAttacked
    (int index, const int endIndex, const int inc, const gridColor attackerColor) const
    {
        for(; index != endIndex + inc; index += inc)
        {            
            if(m_isGridAttacked(index, attackerColor)) return true;
        }

        return false;
    }


    bool gameLogic::m_isGridAttacked(const int index, const gridColor attackerColor) const
    {
        moveRecord move;

        for(int i = 0; i < 64; ++i)
        {   
            if(m_board[i].c == attackerColor && m_canPieceMove(i, index, move)) return true; 
        }

        return false;
    }

    bool gameLogic::m_isKingInCheck(const gridColor kingColor) const
    {   
        if(kingColor == gridColor::NONE) return false;

        int kingIndex = -1;

        // TODO
        // maybe it would be faster if you store kings position
        for(int i = 0; i < 64; ++i)
        {
            if(m_board[i].m_isEqual(gridType::KING, kingColor)) { kingIndex = i; break; }
		}
		
		// const int kingIndex = m_kingLocations[static_cast<int>(kingColor) - 1];
		
		// DEBUG_PRINT_VAR(kingIndex);
		// DEBUG_PRINT_VAR(kingIndex2);
		
        // ASSERT(kingIndex >= 0, 
        // "cannot find the king piece");
		
		return m_isGridAttacked(kingIndex, !kingColor);
    }

    bool gameLogic::m_isGameOver(const gridColor color)
    {
        for(int i = 0; i < 64; ++i)
        {
            if(m_board[i].c == color)
            {
                for(int t = 0; t < 64; ++t)
                {
                    if(m_isPossibleValidMove(i, t)) return false;
                }

            }
        }

        return true;
    }


    static inline int       s_getIntSign(const int val)       { return (val > 0) - (val < 0); }
    static inline vec2<int> s_getIntSign(const vec2<int> val) { return { s_getIntSign(val.x), s_getIntSign(val.y) }; }

    bool gameLogic::m_canBishopMove(int srcInd, int destInd, vec2<int> mov, vec2<int> movAbs) const
    {
        if(movAbs.x != movAbs.y) return false;

        const int indexInc =  s_getIndex(s_getIntSign(mov));
        
        return m_isGridsEmpty(srcInd, destInd, indexInc);
    }

    bool gameLogic::m_canRookMove(int srcInd, int destInd, vec2<int> mov) const
    {
        if((mov.x == 0) == (mov.y == 0)) return false;

        const int indexInc = s_getIndex(s_getIntSign(mov));

        return m_isGridsEmpty(srcInd, destInd, indexInc);
    }

    bool gameLogic::m_canPieceMove
    (const int srcInd, const int destInd, moveRecord& move) const
    {        
        const boardGrid srcGrid  = m_board[srcInd];
        const boardGrid destGrid = m_board[destInd];
        
        if(srcGrid.c == destGrid.c) return false;

        const bool playerTurn = srcGrid.c == m_playerColor;

        const vec2<int> vSrcInd  = s_getIndex(srcInd);
        const vec2<int> vDestInd = s_getIndex(destInd);
        
        const vec2<int> mov    = vDestInd - vSrcInd;
        const vec2<int> movAbs = mov.m_abs();

        move.first = m_createSingleMove(srcInd, destInd);

        switch(srcGrid.t)
        {
        case gridType::PAWN:
        
            if(playerTurn != (mov.y < 0)) break;

            if     (movAbs.y == 2) 
            {   
                // pawns y position must be 1 or 6 (player or computer) to able jump 2 squares
                if(movAbs.x == 0 && destGrid.m_isEmpty() && vSrcInd.y == 1 + playerTurn * 5)
                { return true; }
            }
            else if(movAbs.y == 1)
            {
                if(movAbs.x == !destGrid.m_isEmpty())
                {
                    // promotion
                    if(vDestInd.y == !playerTurn * 7)
                    {
                        // change gridType to queen
                        move.first.srcGrid.t = gridType::QUEEN;
                        
                        // put empty grid to current position
                        // this helps by changing queen type to pawn
                        // when m_undoMove happens
                        move.second = { srcInd, srcInd, {}, srcGrid };
                    }

                    return true;
                }

                // en passant 
                if(movAbs.x == 1 && vSrcInd.y == 4 - playerTurn)
                {
                    const int destPawnInd = srcInd + s_getIntSign(mov.x);
                    const boardGrid destPawn = m_board[destPawnInd];

                    if(destPawn.m_isEqual(gridType::PAWN, !srcGrid.c) && m_isPieceJustPlayed(destPawn.id))
                    {   
                        // move the empty grid to captured pawn initially
                        // then move the piece
                        move.second = move.first;
                        move.first  = { srcInd, destPawnInd, {}, destPawn };
                        return true;
                    }

                }
            }
        break;
        case gridType::KNIGHT:
            if((movAbs.x == 1 && movAbs.y == 2) || (movAbs.x == 2 && movAbs.y == 1)) 
            { return true; }

        break;
        case gridType::BISHOP:
            if(m_canBishopMove(srcInd, destInd, mov, movAbs)) return true;

        break;
        case gridType::ROOK:
            if(m_canRookMove(srcInd, destInd, mov)) return true;
            
        break;
        case gridType::KING:
        
            if(movAbs.x <= 1 && movAbs.y <= 1) return true;

            // castling
            if(mov.y == 0 && movAbs.x == 2)
            {
                const int rookInd = s_getIndex((mov.x > 0) * 7, playerTurn * 7);

                // should be rook on a valid castling
                const boardGrid rookGrid = m_board[rookInd];

                if(rookGrid.m_isEqual(gridType::ROOK, srcGrid.c))
                {
                    // check for rook and king if they played 
                    if(m_isPiecePlayed(rookGrid.id) || m_isPiecePlayed(srcGrid.id)) break;

                    const int posVec = s_getIntSign(mov.x);

                    if(
                        m_isGridsEmpty    (srcInd, rookInd, posVec) && 
                        !m_isGridsAttacked(srcInd, destInd, posVec, !srcGrid.c))
                    {
                        move.second = m_createSingleMove(rookInd, destInd - s_getIntSign(mov.x));
                        return true;
                    }

                }
            }
        break;
        case gridType::QUEEN:
            if(m_canBishopMove(srcInd, destInd, mov, movAbs) || m_canRookMove(srcInd, destInd, mov))
            { return true; }

        break;
        case gridType::NONE:
        break;
        }

        return  false;
    }

    void gameLogic::m_playMove(const singleMove& move)
    {
        ASSERT(move.srcInd >= 0 && move.destInd >= 0 && move.srcInd < 64 && move.destInd < 64,
        "gameLogic::m_playMove out of range");

        // m_board[move.destInd] = m_board[move.srcInd];
        m_board[move.destInd] = move.srcGrid;
        m_board[move.srcInd].m_empty();
    }

    bool gameLogic::m_isPossibleValidMove(const int srcInd, const int destInd)
    {
        moveRecord move;

        const bool validMove = m_getMoveRecord(srcInd, destInd, move);

        m_undoMove(move);

        return validMove;
    }

    bool gameLogic::m_getMoveRecord(const int srcInd, const int destInd, moveRecord& move)
    {
        if(!m_canPieceMove(srcInd, destInd, move)) return false;

        // order matters
        m_playMove(move.first);

        if(move.second.has_value()) m_playMove(move.second.value());

        return !m_isKingInCheck(move.first.srcGrid.c);
    }

    bool gameLogic::m_playIfValid(const int srcInd, const int destInd)
    {
        moveRecord move;

        if(!m_canPieceMove(srcInd, destInd, move)) return false;

        // order matters
        m_playMove(move.first);

        if(move.second.has_value()) m_playMove(move.second.value());

		// const auto& first = move.first;
		// int& kingIndex = m_kingLocations[static_cast<int>(first.srcGrid.c) - 1];
		// const int oldKingIndex = kingIndex;
		
		// if(first.srcGrid.t == gridType::KING) kingIndex = first.destInd;

        if(m_isKingInCheck(move.first.srcGrid.c))
        {                
            m_undoMove(move);
			// kingIndex = oldKingIndex;
            return false;
        }
		
        m_records.push_back(std::move(move));
		
        return true;
    }


    // let the user choose promotion

    const int gameLogic::s_maxStaticEvaluation =  39;
    const int gameLogic::s_minStaticEvaluation = -39;

    void gameLogic::m_playEnemy()
    {
        // return;

        const bool gameEnd = m_isGameOver(m_enemyColor); 

        DEBUG_PRINT_VAR(gameEnd);
        

        if(gameEnd) return;

        // for(int srcInd, destInd; ;)
        // {   
            // srcInd = rand() % 64, destInd = rand() % 64;

            // if(m_board[srcInd].c == m_enemyColor)
            // {
                // if(m_playIfValid(srcInd, destInd))
                // {
                    // DEBUG_PRINT_VAR(m_getStaticEvaluation());
                    // return;
                // }
            // }
        // }

        // for(int i = 0; i < 64; ++i)
        // {
            // if(m_board[i].c == m_enemyColor)
            // {
                // for(int t = 0; t < 64; ++t)
                // {
                    // if(m_playIfValid(i, t)) return;
                // }
            // }

        // }

        int result = s_maxStaticEvaluation;
        int alpha = s_minStaticEvaluation, beta = s_maxStaticEvaluation;

        moveRecord move;
        
        int srcInd, destInd;
        

        for(int i = 0; i < 64; ++i)
        {
            if(m_board[i].c != m_enemyColor) continue;

            for(int t = 0; t < 64; ++t)
            {
                if(m_getMoveRecord(i, t, move))
                {
                    const int currVal = m_minimaxBoard(2, true, alpha, beta);

                    if(currVal < result)
                    { 
                        result = currVal;
                    
                        srcInd = i; 
                        destInd = t;
                    }
                }

                m_undoMove(move);
            }
        }

        // possible unnecessary check in m_isKingInCheck
        m_playIfValid(srcInd, destInd);
    }

    bool gameLogic::m_isPiecePlayed(const int id) const
    {
        for(const auto& record : m_records)
        {
            if(record.first.srcGrid.id == id) return true;
            
            if(record.second.has_value())
            {
                if(record.second.value().srcGrid.id == id) return true;
            }
        }

        return false;
    }


    bool gameLogic::m_isPieceJustPlayed(const int id) const
    {
        if(!m_records.empty())
        {
            const moveRecord last = m_records.back();

            if(last.first.srcGrid.id == id) return true;
                
            if(last.second.has_value())
            {
                if(last.second.value().srcGrid.id == id) return true;
            }
        }

        return false;
    }

    void gameLogic::m_undoMove(const singleMove& move)
    {
        m_board[move.srcInd]  = move.srcGrid;
        m_board[move.destInd] = move.destGrid;
    }

    void gameLogic::m_undoMove(const moveRecord& record)
    {
        // order matters
        m_undoMove(record.first);

        if(record.second.has_value()) m_undoMove(record.second.value());
    }

    void gameLogic::m_undoMove()
    {        
        if(m_records.empty()) return;

        m_undoMove(m_records.back());
        
        m_records.pop_back();
    }

    constexpr int gameLogic::s_getPieceValues(const gridType t)
    {
        switch(t)
        {
        case gridType::PAWN:
        return 1;
        case gridType::KNIGHT:
        case gridType::BISHOP:
        return 3;
        case gridType::ROOK:
        return 5;
        case gridType::QUEEN:
        return 9;
        case gridType::KING:
        case gridType::NONE:
        break;
        }

        return 0;
    }

    

    int gameLogic::m_getStaticEvaluation() const
    {
        int result = 0;

        for(int i = 0; i < 64; ++i)
        {
            const int value = s_getPieceValues(m_board[i].t);
         
            if     (m_board[i].c == m_playerColor) result += value;
            else if(m_board[i].c == m_enemyColor)  result -= value;
        }

        return result;
    }

    int gameLogic::m_minimaxBoard(const int depth, const bool maximizing, int alpha, int beta)
    {   
        if(depth == 0) return m_getStaticEvaluation();

        if(maximizing)
        {
            int result = s_minStaticEvaluation;
            moveRecord move;
            
            for(int i = 0; i < 64; ++i)
            {
                if(m_board[i].c != m_playerColor) continue;

                for(int t = 0; t < 64; ++t)
                {
                    if(m_getMoveRecord(i, t, move))
                    {
                        const int currVal = m_minimaxBoard(depth - 1, !maximizing, alpha, beta);

                        if(currVal > result) result = currVal;
                        if(alpha > currVal)   alpha = currVal;

                        if(beta <= alpha)
                        {
                            m_undoMove(move);
                            return result;
                        }
                    }

                    m_undoMove(move);
                }
            }

            return result;
        }
        else
        {
            int result = s_maxStaticEvaluation;
            moveRecord move;
            
            for(int i = 0; i < 64; ++i)
            {
                if(m_board[i].c != m_enemyColor) continue;

                for(int t = 0; t < 64; ++t)
                {
                    if(m_getMoveRecord(i, t, move))
                    {
                        const int currVal = m_minimaxBoard(depth - 1, !maximizing, alpha, beta);

                        if(currVal < result) result = currVal;
                        if(currVal < beta)     beta = currVal;

                        if(beta <= alpha)
                        {
                            m_undoMove(move);
                            return result;
                        }
                    }

                    m_undoMove(move);
                }
            }

            return result;
        }


    }



}