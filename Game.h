//
// Created by andra on 19.05.2023.
//

#ifndef UNTITLED1_GAME_H
#define UNTITLED1_GAME_H


#include "Board.h"
#include "ScoreBoard.h"

using namespace std;
/**
 * \class Game
 *\brief Represents a game with board players and methods to check game rules.
 * The Game class manages the game state, including the board, players, counters, and game format.
 * It provides methods to perform moves, check for valid moves, display the board, and determine the winner.
  */

class Game {
    Player player; ///< Current player (Ruby or Pearl)
    GameFormat gameFormat; ///< Game format (e.g., board size)
    bool won; ///< Flag indicating if the game has been won
    int rubyCounter; ///< Counter for Ruby's figures
    int pearlCounter; ///< Counter for Pearl's figures
public:
    Board board; ///< Game board

    /**
    @brief Constructs a new Game object with the specified board size.
    *
    * @param y The number of rows in the board. Default is 17.
    * @param x The number of columns in the board. Default is 9.
    */
    explicit Game(int y = 17, int x = 9) :
            board(y, x),
            player(Player::Ruby),
            won(false),
            rubyCounter(0),
            pearlCounter(0) {}

/**
 * @brief Chooses a figure at the specified position.
 *
 * @param y The y-coordinate of the figure.
 * @param x The x-coordinate of the figure.
 * @return A vector containing the chosen figure's coordinates [y, x].
 */
    static vector<int> chooseFigure(int y, int x) {
        vector<int> figure = {y, x};
        return figure;
    }

    /**
 * @brief Moves a figure from the specified source position to the target position.
     * And also changes players after move.
 *
 * @param yFrom The y-coordinate of the source position.
 * @param xFrom The x-coordinate of the source position.
 * @param yTo The y-coordinate of the target position.
 * @param xTo The x-coordinate of the target position.
 */
    void move(int yFrom, int xFrom, int yTo, int xTo) {
        SquareVal val = player == Player::Ruby ? SquareVal::Ruby : SquareVal::Pearl;

        if (isJump(yFrom, xFrom, yTo, xTo)) {
            board.setSquare(yFrom, xFrom, SquareVal::Empty);
        }
        board.setSquare(yTo, xTo, val);
        vector<tuple<int, int>> neighbors = board.getNeighbors(yTo, xTo);
        for (auto n: neighbors) {
            auto [r, c] = n;
            SquareVal enemy = player == Player::Ruby ? SquareVal::Pearl : SquareVal::Ruby;
            if (board.getSquare(r, c) == enemy)
                board.setSquare(r, c, val);
        }

        player = player == Player::Ruby ? Player::Pearl : Player::Ruby;
    }

    /**
 * @brief Returns a vector of valid figures for the specified player.
 *
 * @param player The player for which to find valid figures.
 * @return A vector of valid figures represented as tuples [y, x].
 */
    vector<tuple<int, int>> getValidFigures(Player) {
        vector<tuple<int, int>> validFigures;
        for (int y = 0; y < board.sizeY(); y++) {
            for (int x = 0; x < board.sizeX(); x++) {
                if (board.getSquare(y, x) == SquareVal::Ruby && player == Player::Ruby && !getValidMoves(y, x).empty()
                    || board.getSquare(y, x) == SquareVal::Pearl && player == Player::Pearl &&
                       !getValidMoves(y, x).empty()) {
                    tuple<int, int> figure = make_tuple(y, x);
                    validFigures.push_back(figure);
                }
            }
        }
        return validFigures;
    }

    /**
 * @brief Returns a vector of valid moves for the figure at the specified position.
 *
 * @param y The y-coordinate of the figure's position.
 * @param x The x-coordinate of the figure's position.
 * @return A vector of valid moves represented as tuples [y, x].
 */
    vector<tuple<int, int>> getValidMoves(int y, int x) const {
        vector<tuple<int, int>> validMoves = board.getValidMoves(y, x);
        return validMoves;
    }

/**
 * @brief Displays the valid moves for the figure at the specified position.
 *
 * @param y The y-coordinate of the figure's position.
 * @param x The x-coordinate of the figure's position.
 */
    void showValidMoves(int y, int x) const {
        vector<tuple<int, int>> validMoves = board.getValidMoves(y, x);
        for (auto tup: validMoves) {
            cout << "(" << get<0>(tup) << ", " << get<1>(tup) << ") ";
        }
        cout << endl;
    }

/**
 * @brief Checks if the move from the source position to the target position is a jump.
 *
 * @param yFrom The y-coordinate of the source position.
 * @param xFrom The x-coordinate of the source position.
 * @param yTo The y-coordinate of the target position.
 * @param xTo The x-coordinate of the target position.
 * @return True if the move is a jump, false otherwise.
 */
    bool isJump(int yFrom, int xFrom, int yTo, int xTo) const {
        tuple<int, int> move = make_tuple(yTo, xTo);
        for (auto tup: board.getNeighbors(yFrom, xFrom)) {
            if (tup == move) return false;
        }
        return true;
    }

/**
 * @brief Checks if the figure at the specified position has no valid moves.
 *
 * @param y The y-coordinate of the figure's position.
 * @param x The x-coordinate of the figure's position.
 * @return True if the figure has no valid moves, false otherwise.
 */
    bool noMoves(int y, int x) {
        if (getValidMoves(y, x).empty() || board.isEmpty(y, x)) {
            cout << "This figure has no moves. Select another." << endl;
            return true;
        } else {
            return false;
        }
    }

/**
 * @brief Checks if the figure at the specified position belongs to the current player.
 *
 * @param y The y-coordinate of the figure's position.
 * @param x The x-coordinate of the figure's position.
 * @return True if the figure belongs to the current player, false otherwise.
 */
    bool yourFigure(int y, int x) const {
        if ((board.getSquare(y, x) == SquareVal::Ruby && getPlayer() == Player::Ruby)
            || (board.getSquare(y, x) == SquareVal::Pearl && getPlayer() == Player::Pearl)) {
            return true;
        }
        return false;
    }

/**
 * @brief Checks if the game has been won.
 *
 * If one player has no valid figures or if both players have no valid figures, the game is end.
 * The winner is determined by counting the number of players squares on the board.
 * If the counts are equal, it is a draw.
 */
    void checkWon() {
        if (getValidFigures(Player::Ruby).empty() && !getValidFigures(Player::Pearl).empty()) {
            for (int y = 0; y < board.sizeY(); y++) {
                for (int x = 0; x < board.sizeX(); x++) {
                    if (board.getSquare(y, x) == SquareVal::Empty) pearlCounter++;
                }
            }
            winConditional();
        } else if (getValidFigures(Player::Pearl).empty() && !getValidFigures(Player::Ruby).empty()) {
            for (int y = 0; y < board.sizeY(); y++) {
                for (int x = 0; x < board.sizeX(); x++) {
                    if (board.getSquare(y, x) == SquareVal::Empty) rubyCounter++;
                }
            }
            winConditional();
        } else if (getValidFigures(Player::Pearl).empty() && getValidFigures(Player::Ruby).empty()) {
            winConditional();
        }
    }

/**
 * @brief Checks if the game has been won.
 *
 * @return True if the game has been won, false otherwise.
 */
    bool isWon() {
        checkWon();
        return won;
    }

/**
 * @brief Displays the winner and the game result.
 */
    void winConditional() {
        won = true;
        if (rubyCounter > pearlCounter) {
            cout << "RUBY WINS!!!" << endl;
        } else if (pearlCounter > rubyCounter) {
            cout << "PEARL WINS!!!" << endl;
        } else {
            cout << "DRAW!!!" << endl;
        }
        ScoreBoard::updateScoreboard("scoreBoard",rubyCounter,pearlCounter);
    }

/**
 * @brief Prints the current state of the game board.
 */
    void printBoard() const {
        cout << board;
    }

/**
 * @brief Returns the game format.
 *
 * @return The game format.
 */
    GameFormat getGameFormat() const {
        return gameFormat;
    }

/**
 * @brief Sets the game format.
 *
 * @param gameFormat The game format to set.
 */
    void setGameFormat(GameFormat gameFormat) {
        Game::gameFormat = gameFormat;
    }

/**
 * @brief Returns the current player.
 *
 * @return The current player.
 */
    Player getPlayer() const {
        return player;
    }

/**
 * @brief Returns the counter for Ruby's figures.
 *
 * @return The counter for Ruby's figures.
 */
    int getRubyCounter() const {
        return rubyCounter;
    }

/**
 * @brief Sets the counter for Ruby's figures.
 */
    void setRubyCounter() {
        Game::rubyCounter = board.countFigures(SquareVal::Ruby);
    }

/**
 * @brief Returns the counter for Pearl's figures.
 *
 * @return The counter for Pearl's figures.
 */
    int getPearlCounter() const {
        return pearlCounter;
    }

/**
 * @brief Sets the counter for Pearl's figures.
 */
    void setPearlCounter() {
        Game::pearlCounter = board.countFigures(SquareVal::Pearl);
    }

/**
 * @brief Default destructor.
 */
    ~Game() = default;
};
#endif //UNTITLED1_GAME_H
