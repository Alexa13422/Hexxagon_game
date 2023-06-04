//
// Created by andra on 19.05.2023.
//




#ifndef UNTITLED1_AI_H
#define UNTITLED1_AI_H

#include "Board.h"


/**
@class AI
@brief The AI class represents an artificial intelligence player in the game.
The AI class is responsible for computing the best move for the AI player based on the current game board.
It evaluates the valid moves for each figure and selects the move that maximizes the number of enemy neighbors.
*/
class AI {
private:
    Board board; /**< The game board. */
public:
/**
* @brief Default constructor for the AI class.
*/
    AI() = default;

    /**
  * @brief Computes the best move for the AI player.
  *
  * Given a vector of valid figures for the AI player, this method evaluates the possible moves for each figure.
  * It counts the number of enemy neighbors for each move and selects the move that maximizes the count.
  * If multiple moves have the same maximum count, they are all considered as possible move variations.
  *
  * @param validFigures A vector of valid figures for the AI player.
  * @return A vector of move variations represented as tuples [yFigure, xFigure, yMove, xMove].
  */
    vector<tuple<int,int,int,int>> computeMove(const vector<tuple<int, int>>& validFigures) {
        vector<tuple<int,int,int,int>> moveVariations;
        int enemyNeighbors = 0;
        int counter = 0;
        for (auto figure: validFigures) {
            auto [yFigure, xFigure] = figure;
            auto validMoves = board.getValidMoves(yFigure, xFigure);
            for (auto move: validMoves) {
                auto [yMove, xMove] = move;
                enemyNeighbors = countEnemies(board.getNeighbors(yMove, xMove));
                if (enemyNeighbors > counter) {
                    counter = enemyNeighbors;
                    moveVariations.clear();
                }
                if (enemyNeighbors == counter)
                    moveVariations.emplace_back(yFigure, xFigure, yMove, xMove);
            }
        }
        return moveVariations;
    }

/**
 * @brief Counts the number of enemy neighbors in a given vector of neighbors.
 *
 * @param neighbors A vector of neighbors represented as tuples [rNeighbor, cNeighbor].
 * @return The number of enemy neighbors.
 */
    int countEnemies(const vector<tuple<int, int>>& neighbors){
        int enemyNeighbors = 0;
        for(auto neighbor : neighbors){
            auto [rNeighbour, cNeighbor] = neighbor;
            if (board.getSquare(rNeighbour,cNeighbor) == SquareVal::Ruby) enemyNeighbors++;
        }
        return enemyNeighbors;
    }

/**
 * @brief Sets the game board for the AI.
 *
 * @param board The game board to set.
 */
    void setBoard(const Board &board) {
        AI::board = board;
    }

};
#endif //UNTITLED1_AI_H
