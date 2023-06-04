/**
 * \brief Main method
 *
 * In main method firstly start gui part of the project with start menu and then game board in the console.
 * Game continues until all positions will be placed by figures or there will be no place for one of the player.
 * Wins player who has more points.
 * If we choose pvai game mode main method call calculate method from AI class and
 * the move method to make bot move its pieces.
 * @see AI
 * main.cpp file also contains overladed cout operator for Player enum class
 * and read int method for clearly out massages to console.
 *
*/

#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <limits>
#include "SFML/System/Vector2.hpp"
#include "StartWindow.h"
#include "Game.h"
#include "AI.h"

using namespace std;


ostream& operator<<(ostream& out, const Player& p) {
    char label = p == Player::Ruby ? 'R' : 'P';
    out << label;

    return out;
}

void readint(const string& title, int &i) {
    cout << title << endl;
    while(!(cin >> i)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid integer, please try again: ";
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    StartWindow startWindow;

    cout << "Select game format: 1)PVP 2)PVAI ";
    int gameFormat;
    cin >> gameFormat;

    Game g = Game();
    AI bot;
    g.setGameFormat(gameFormat == 1 ? GameFormat::pvp : GameFormat::pvai);

    cout << "Game start - Ruby turn, " << endl;

    while (!g.isWon()) {
        g.printBoard();
        g.setRubyCounter();
        g.setPearlCounter();
        cout << "Ruby number: " << g.getRubyCounter() << endl
             << "Pearl number: " << g.getPearlCounter() << endl;
        cout << "Player " << g.getPlayer() << "'s turn, select figure: " << endl;
        int yFrom, xFrom, yTo, xTo;
        //bot turn moves
        if (g.getGameFormat() == GameFormat::pvai && g.getPlayer() == Player::Pearl){
            bot.setBoard(g.board);
            auto possibleMoves = bot.computeMove(g.getValidFigures(g.getPlayer()));
            srand(time(nullptr));
            int random_index = rand() % possibleMoves.size();
            tuple<int, int, int, int> random_move = possibleMoves[random_index];
            yFrom = get<0>(random_move);
            xFrom = get<1>(random_move);
            yTo = get<2>(random_move);
            xTo = get<3>(random_move);
            g.move(yFrom,xFrom,yTo,xTo);
            continue;
        } else {
            readint("Enter row: ", yFrom);
            readint("Enter col: ", xFrom);
        }
        if (!g.yourFigure(yFrom,xFrom)){
            cout<<"Its not your figure, select another"<<endl;
            continue;
        }
        if (g.noMoves(yFrom,xFrom)) continue;

        g.showValidMoves(yFrom,xFrom);
        vector<tuple<int, int>> validMoves = g.getValidMoves(yFrom,xFrom);
        bool contains = false;
        while (!contains) {
            cout << "Select where to move: " << endl;
            readint("Enter row: ", yTo);
            readint("Enter col: ", xTo);
            tuple<int,int> selectedMove = make_tuple(yTo,xTo);
            auto it = find(validMoves.begin(), validMoves.end(), selectedMove);
            if (it != validMoves.end()) {
                contains = true;
            } else {
                contains = false;
                g.printBoard();
                cout<<"this move is not valid" << endl;
                g.showValidMoves(yFrom,xFrom);
            }
        }
        g.move(yFrom, xFrom,yTo, xTo);
    }

    return 1;
}
