//
// Created by andra on 19.05.2023.
//



#ifndef UNTITLED1_BOARD_H
#define UNTITLED1_BOARD_H

#include <vector>
#include <tuple>
#include <iostream>
#include <iomanip>
#include "Variables.h"


using namespace std;
/**
 * \class Board
 *
 * \brief Represents the game board.
 *
 * The Board class manages the game board, which is a grid of square cells. Each cell can have a value representing its state.
 * The class provides methods for initializing the board, setting and getting the values of cells, and performing various operations on the board.
 */
class Board {
private:
    vector< vector<SquareVal> > board;

public:
    /**
   * \brief Constructs a Board object with the specified dimensions.
   *
   * Initializes the game board with the specified number of rows and columns.
   * By default, each cell is set to SquareVal::Empty, except for predefined positions that are set to Players.
   *
   * \param y The number of rows in the board. Default is 17.
   * \param x The number of columns in the board. Default is 9.
   */
    explicit Board(int y = 17, int x=9): board(y, vector<SquareVal>(x, SquareVal::Empty)) {
        this->setSquare(0,4,SquareVal::Ruby);
        this->setSquare(4,8,SquareVal::Pearl);
        this->setSquare(12,8,SquareVal::Ruby);
        this->setSquare(16,4,SquareVal::Pearl);
        this->setSquare(12,0,SquareVal::Ruby);
        this->setSquare(4,0,SquareVal::Pearl);
        for (int r = 0; r < y; r++) {
            for (int c = 0; c < x; c++) {
                if ((r % 2 != 0 && c % 2 == 0) || (r % 2 == 0 && c % 2 != 0)
                    || (r + c <= 3)
                    || (r + c >= 21)
                    || (c - r >= 5)
                    || (r - c >= 13)) {
                    this->setSquare(r,c,SquareVal::Invaluable);
                }
            }
        }
        this->setSquare(6,4,SquareVal::Invaluable);
        this->setSquare(9,3,SquareVal::Invaluable);
        this->setSquare(9,5,SquareVal::Invaluable);
    }
    /**
        * \brief Overloaded output stream operator for printing the board.
        *
        * Prints the board to the output stream in a formatted manner.
        * Each cell value is represented by a specific character:
        * - SquareVal::Invaluable: Represented by empty space ("   ").
        * - SquareVal::Empty: Represented by " O ".
        * - SquareVal::Ruby: Represented by " R ".
        * - SquareVal::Pearl: Represented by " P ".
        *
        * \param out The output stream to write the board to.
        * \param b The Board object to be printed.
        * \return The output stream after printing the board.
        */
    friend ostream& operator<<(ostream& out, const Board& b) {
        const int numRows = b.board.size();
        const int numCols = b.board[0].size();

        // Output column numbers in the first row
        out << "   ";
        for (int c = 0; c < numCols; c++) {
            out << setw(1) << c << "  ";
        }
        out << endl;

        // Output board rows
        for (int r = 0; r < numRows; r++) {
            // Output row number on the left
            if (numCols < 10) {
                out << setw(2) << r << "";
            } else out << setw(0) << r << "";

            // Output cells
            for (int c = 0; c < numCols; c++) {
                if (b.board[r][c] == SquareVal::Invaluable) {
                    out << "   ";
                } else {
                    if (b.board[r][c] == SquareVal::Empty) {
                        out << " O ";
                    } else if (b.board[r][c] == SquareVal::Ruby) {
                        out << " R ";
                    } else if (b.board[r][c] == SquareVal::Pearl) {
                        out << " P ";
                    }
                }
            }
            out << endl;
        }

        return out;
    }
/**
    * \brief Gets the value of the cell at the specified position.
   */
   SquareVal getSquare(int y, int x) const {
       return board[y][x];
   }
/**
    * \brief Sets the value of the cell at the specified position.
    */
  void setSquare(int y, int x, SquareVal val) {
      board[y][x] = val;
  }
    /**
     * \brief Returns the number of columns in the board.
     */
  int sizeX() const {
      return board[0].size();
  }
    /**
   * \brief Returns the number of rows in the board.
   *
   */
  int sizeY() const {
      return board.size();
  }
/**
 * \brief Returns a copy of the game board.
 */
vector<vector<SquareVal>> getBoard() const {
  return board;
}
/**
     * \brief Checks if the specified position is within the valid board boundaries.
     *
     * Determines if the given row and column values are within the valid boundaries of the game board.
     * It also checks if the position is a valid move position based on the game rules.
     *
     * \return True if the position is within the valid board boundaries and a valid move position, false otherwise.
     */
static bool isWithinBoard(int y, int x) {
  if ((y%2==0 && x%2==0)) {
      if (y == 0 && x == 4
          ||  y == 2 && x >= 2 && x <= 6
          ||  y == 14 && x >=2 && x <= 6
          ||  y == 16 && x == 4
          ||  y >= 4 && y <= 12) { return true; }
  }
  if ((y%2!=0 && x%2!=0)){
      if (y == 1 && x >= 3 && x <= 5
          || y == 3 && x >= 1 && x <= 7
          || y == 13 && x >= 1 && x <= 7
          || y == 15 && x >= 3 && x <= 5
          || y >= 4 && y <= 12) { return true; }
  }
  return false;
}
    /**
       * \brief Gets the valid move positions for the specified cell.
       *
       * Determines the valid move positions (neighboring cells) for the given row and column.
       * The valid move positions must be within the valid board boundaries and should be empty cells.
       *
       * \return A vector of tuples representing the valid move positions.
       */
vector<tuple<int, int>> getValidMoves(int y, int x) const{
  vector<tuple<int,int>> moves;
  tuple<int, int> positions[] = {
          make_tuple(y-2, x),
          make_tuple(y-1, x+1),
          make_tuple(y+1, x+1),
          make_tuple(y+2, x),
          make_tuple(y+1, x-1),
          make_tuple(y-1, x-1),
          make_tuple(y-4, x),
          make_tuple(y-3, x-1),
          make_tuple(y-2, x-2),
          make_tuple(y, x-2),
          make_tuple(y+2, x-2),
          make_tuple(y+3, x-1),
          make_tuple(y+4, x),
          make_tuple(y+3, x+1),
          make_tuple(y+2, x+2),
          make_tuple(y, x+2),
          make_tuple(y-2, x+2),
          make_tuple(y-3, x+1),
  };
  for (auto pos: positions) {
      auto [r, c] = pos; // C++17 structured binding
      if (isWithinBoard(r, c) && getSquare(r, c) == SquareVal::Empty) {
          moves.push_back(pos);
      }
  }
  return moves;
}
/**
     * \brief Gets the neighboring positions for the specified cell.
     *
     * Determines the neighboring positions (adjacent cells) for the given row and column.
     * The neighboring positions must be within the valid board boundaries and should not be SquareVal::Invaluable cells.
     *
     * \return A vector of tuples representing the neighboring positions.
     */
vector< tuple<int, int> > getNeighbors(int y, int x) const {
  tuple<int, int> positions[] = {
          make_tuple(y-2, x),
          make_tuple(y-1, x+1),
          make_tuple(y+1, x+1),
          make_tuple(y+2, x),
          make_tuple(y+1, x-1),
          make_tuple(y-1, x-1),
  };
  vector< tuple<int, int> > neighbors;
  for (auto pos: positions) {
      auto [r, c] = pos; // C++17 structured binding
      if (isWithinBoard(r, c) && getSquare(r, c) != SquareVal::Invaluable) {
          neighbors.push_back(pos);
      }
  }
  return neighbors;
}
/**
     * \brief Counts the number of figures with the specified value on the board.
     *
     *
     * \param val The value of the figures to count.
     * \return The number of figures with the specified value on the board.
     */
int countFigures(SquareVal val){
  int counter = 0;
  for (int y = 0; y < board.size(); y++) {
      for (int x = 0; x < board[y].size(); x++) {
          if (getSquare(y,x) == val) counter++;
      }
  }
  return counter;
}
    /**
        * \brief Checks if the specified cell is empty.
        *
        *
        * \return True if the cell is empty, false otherwise.
        */
bool isEmpty(int y, int x){
  return getSquare(y,x) == SquareVal::Empty;
}

~Board()= default;
};
#endif //UNTITLED1_BOARD_H

