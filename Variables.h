//
// Created by andra on 19.05.2023.
//

#ifndef UNTITLED1_VARIABLES_H
#define UNTITLED1_VARIABLES_H
/**
 * \enum Player
 *
 * \brief Represents the players in the game.
 * */
enum class Player: short { Ruby, Pearl };
/**
 * \enum SquareVal
 *
 * \brief Represents the values of the game board cells.
 *
 * The SquareVal enum class defines the possible values of the cells on the game board.
 * It includes the following values:
 * - Empty: Represents an empty cell.
 * - Ruby: Represents a cell containing a Ruby figure.
 * - Pearl: Represents a cell containing a Pearl figure.
 * - Invaluable: Represents an invaluable cell that cannot be moved onto or occupied.
 */
enum class SquareVal: short {
    Empty = 0,
    Ruby = 1,
    Pearl = 2,
    Invaluable = 3
};
/**
 * \enum GameFormat
 *
 * \brief Represents the game formats.
 *.
 * It includes the following values:
 * - pvp: Represents a player versus player game format.
 * - pvai: Represents a player versus AI game format.
 */
enum class GameFormat: short {
    pvp = 1,
    pvai = 2
};
#endif //UNTITLED1_VARIABLES_H
