//
// Created by andra on 19.05.2023.
//



#ifndef UNTITLED1_GRAPHICALBOARD_H
#define UNTITLED1_GRAPHICALBOARD_H

#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Game.h"
#include "AI.h"


/**

\class GraphicalBoard
\brief The GraphicalBoard class represents the graphical representation of the game board.
The class handles the rendering of the game board using the SFML library and provides methods for interacting
with the game board through user input. It also manages the animation of moves and keeps track of the selected figure.
*/
class GraphicalBoard {
    Game game; /**< The Game object representing the game logic. */
    std::vector<sf::CircleShape> hexagones; /**< The vector of hexagone shapes representing the game board. */
    vector<int> selectedFigure; /**< The selected figure on the game board. */

public:
/**
* \brief Constructor for the GraphicalBoard class.
*
* The constructor initializes the graphical window, buttons, and text elements using the SFML library.
* It allows the user to choose the game mode (PVP or PVE) and initializes the game accordingly.
* The constructor then fills the hexagones vector with CircleShape objects representing the game board
* and paints the initial board on the window.
*
* It enters a game loop where it continuously checks for user input and updates the game state accordingly.
* It also handles the AI move if the game mode is PvAI and the current player is Pearl.
*
* Once the game is won, it displays the winner on the window, waits for 5 seconds, and exits the game loop.
*/
    GraphicalBoard() {
        sf::RenderWindow window(sf::VideoMode({640, 920}), "Hexxagon");
        sf::Event event{};
        sf::Vector2f buttonSize(150.0f, 50.0f);
        sf::Vector2f buttonPosition(100 , 410);
        sf::Vector2f buttonTextPosition(120,420);
        sf::Vector2f labelPosition(150,200);

        sf::RectangleShape buttonPVP(buttonSize);
        buttonPVP.setPosition(buttonPosition);
        buttonPVP.setFillColor(sf::Color::Green);

        sf::RectangleShape buttonPVE(buttonSize);
        buttonPosition.x += buttonSize.x * 2;
        buttonPVE.setPosition(buttonPosition);
        buttonPVE.setFillColor(sf::Color::Red);

        sf::Font font;
        if (!font.loadFromFile("Fonts/Franklin Gothic Heavy Regular.ttf")) {
            cout<<"O_O";
        }
        sf::Text buttonPVPText = sf::Text(font);
        buttonPVPText.setString("PVP");
        buttonPVPText.setCharacterSize(20);
        buttonPVPText.setFillColor(sf::Color::White);
        buttonPVPText.setStyle(sf::Text::Bold);
        buttonPVPText.setPosition(buttonTextPosition);

        sf::Text buttonPVEText = sf::Text(font);
        buttonPVEText.setString("PVE");
        buttonPVEText.setCharacterSize(20);
        buttonPVEText.setFillColor(sf::Color::White);
        buttonPVEText.setStyle(sf::Text::Bold);
        buttonTextPosition.x += buttonSize.x * 2;
        buttonPVEText.setPosition(buttonTextPosition);

        sf::Text text = sf::Text(font);
        text.setString("Choose game mode");
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::Cyan);
        text.setOutlineColor(sf::Color::Red);
        text.setOutlineThickness(3);
        text.setStyle(sf::Text::Bold);
        text.setPosition(labelPosition);


        window.draw(text);
        window.draw(buttonPVP);
        window.draw(buttonPVE);
        window.draw(buttonPVPText);
        window.draw(buttonPVEText);
        window.display();
        bool gameModeIsChosen=false;
        AI bot;
        while (!gameModeIsChosen) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                        auto mousePos = static_cast<sf::Vector2f>(mousePosition);
                        if (buttonPVP.getGlobalBounds().contains(mousePos)) {
                            game.setGameFormat(GameFormat::pvp);
                            gameModeIsChosen = true;
                        } else if (buttonPVE.getGlobalBounds().contains(mousePos)) {
                            game.setGameFormat(GameFormat::pvai);
                            gameModeIsChosen = true;
                        }
                    }
                }
            }
        }
        window.clear();

        fillHexagones();
        paintBoard(window);
        window.display();


        while (window.isOpen()) {
            if (game.isWon()){
                window.clear();
                if (game.getRubyCounter() > game.getPearlCounter()){
                    text.setString("RUBY WIN!");
                }
                else if (game.getPearlCounter() > game.getRubyCounter()){
                    text.setString("PEARL WIN!");
                }
                else  text.setString("!DRAW!");
                text.setCharacterSize(54);
                labelPosition.y += 200;
                labelPosition.x += 25;
                text.setPosition(labelPosition);
                window.draw(text);
                window.display();
                sf::sleep(sf::seconds(5.0f));
                break;
            }
            if (game.getGameFormat() == GameFormat::pvai && game.getPlayer() == Player::Pearl){
                bot.setBoard(game.board);
                auto possibleMoves = bot.computeMove(game.getValidFigures(game.getPlayer()));
                srand(time(nullptr));
                int random_index = rand() % possibleMoves.size();
                tuple<int, int, int, int> random_move = possibleMoves[random_index];
                int yTo, xTo;
                selectedFigure = Game::chooseFigure(get<0>(random_move), get<1>(random_move));
                yTo = get<2>(random_move)*54;
                xTo = get<3>(random_move)*54;
                sf::sleep(sf::seconds(1.0f));
                for ( auto & hex: hexagones){
                    if (hex.getPosition().y == yTo && hex.getPosition().x == xTo){
                        move(window,hex);
                        break;
                    }
                }
            }
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                        auto mousePos = static_cast<sf::Vector2f>(mousePosition);
                        for ( auto & hex: hexagones) {
                            if (hex.getGlobalBounds().contains(mousePos)) {
                                if (!selectedFigure.empty()) {
                                    move(window, hex);
                                } else {
                                    selectFigure(window, hex);
                                }
                                break;
                            }
                        }
                    }
                }
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
        }
    };

    /**
 * \brief Paints the game board on the given SFML window.
 *
 * This method clears the window and draws the hexagon shapes representing the game board.
 * It also displays the counters for the number of Ruby and Pearl figures on the board.
 *
 * \param window The SFML window on which to paint the game board.
 */
    void paintBoard(sf::RenderWindow& window){
        window.clear();
        for (const auto& hex : hexagones){
            window.draw(hex);
        }
        sf::Font font;
        if (!font.loadFromFile("Fonts/Franklin Gothic Heavy Regular.ttf")) {
            cout<<"O_O";
        }
        int rubyCount, pearlCount;
        game.setRubyCounter();
        game.setPearlCounter();
        rubyCount = game.getRubyCounter();
        pearlCount = game.getPearlCounter();
        sf::Vector2f labelPosition(500,200);
        sf::Text rubyCounter = sf::Text(font);
        rubyCounter.setString("Ruby: " + std::to_string(rubyCount));
        rubyCounter.setCharacterSize(30);
        rubyCounter.setFillColor(sf::Color::Red);
        rubyCounter.setOutlineColor(sf::Color::Cyan);
        rubyCounter.setOutlineThickness(3);
        rubyCounter.setStyle(sf::Text::Bold);
        rubyCounter.setPosition(labelPosition);

        sf::Text pearlCounter = sf::Text(font);
        pearlCounter.setString("Pearl: " + std::to_string(pearlCount));
        pearlCounter.setCharacterSize(30);
        pearlCounter.setFillColor(sf::Color::Cyan);
        pearlCounter.setOutlineColor(sf::Color::Red);
        pearlCounter.setOutlineThickness(3);
        pearlCounter.setStyle(sf::Text::Bold);
        labelPosition.y += 200;
        pearlCounter.setPosition(labelPosition);
        window.draw(rubyCounter);
        window.draw(pearlCounter);
    }

    /**
 * \brief Fills the hexagones vector with CircleShape objects representing the game board.
 *
 * This method iterates over the game board and creates CircleShape objects for each position.
 * The created shapes are added to the hexagones vector.
 */
    void fillHexagones(){
        hexagones.clear();
        for (int y = 0; y < game.board.sizeY(); y++) {
            for (int x = 0; x < game.board.sizeX(); x++) {
                auto figure = writeHexagon(y,x);
                if (!isShapePositionZero(figure)) {
                    hexagones.push_back(figure);
                }
            }
        }
    }
/**
 * \brief Handles the selection of a figure on the game board.
 *
 * This method checks if the selected figure is a valid choice based on the current player and the figure's color.
 * If the selected figure is valid, it sets the outline color and thickness to indicate selection.
 * If a figure was already selected, it clears the selection and highlights the valid moves for the new selection.
 *
 * \param hex The CircleShape representing the selected figure.
 */
    void selectFigure(sf::RenderWindow& window, sf::CircleShape& hex) {
        auto Ruby = sf::Color(255, 50, 50);
        auto Pearl = sf::Color(200, 200, 250);
        if (hex.getFillColor() == Ruby && game.getPlayer() == Player::Ruby
            || hex.getFillColor() == Pearl && game.getPlayer() == Player::Pearl) {
            if (isChosed(hex)) {
                hex.setOutlineThickness(0);
                hex.setOutlineColor(sf::Color::Transparent);
                auto validMoves = game.getValidMoves(selectedFigure[0], selectedFigure[1]);
                selectedFigure.clear();
                pointMoves(validMoves);
                paintBoard(window);
                window.display();
            } else if (selectedFigure.empty()) {
                hex.setOutlineColor(sf::Color::Green);
                hex.setOutlineThickness(3);
                selectedFigure = Game::chooseFigure(hex.getPosition().y / 54, hex.getPosition().x / 54);
                auto validMoves = game.getValidMoves(selectedFigure[0], selectedFigure[1]);
                pointMoves(validMoves);
                paintBoard(window);
                window.display();
            }
        }
    }
/**
 * \brief Moves the selected figure on the game board.
 *
 * This method handles the movement of the selected figure based on the user's click on a valid move position.
 * It checks if the clicked position is a valid move for the selected figure and moves the figure accordingly.
 * After the move, it clears the selected figure and updates the hexagones vector and game state.
 *
 * \param hex The CircleShape representing the clicked position.
 */
    void move(sf::RenderWindow& window, sf::CircleShape& hex){
        if (checkFigureToChoosen(hex)) {
            selectFigure(window, hex);
            return;
        }
        auto validMoves = game.getValidMoves(selectedFigure[0],selectedFigure[1]);
        for (auto move : validMoves) {
            auto yMove = std::get<0>(move)*54;
            auto xMove = std::get<1>(move)*54;
            if (yMove == hex.getPosition().y
                && xMove == hex.getPosition().x){
                sf::Vector2f selectedHex(selectedFigure[1]*54+2,selectedFigure[0]*54+2);
                for ( auto hexagoneFrom : hexagones){
                    if (hexagoneFrom.getGlobalBounds().contains(selectedHex)){
                        animateMove(window,
                                    hexagoneFrom,
                                    sf::Vector2f(selectedFigure[1]*54,selectedFigure[0]*54),
                                    sf::Vector2f(xMove,yMove));
                        break;
                    }
                }

                game.move(selectedFigure[0],selectedFigure[1],yMove/54,xMove/54);
                selectedFigure.clear();
                fillHexagones();
                paintBoard(window);
                window.display();
                return;
            }
        }
    }
    /**
 * \brief Animates the movement of a figure from its initial position to the target position.
 *
 * This method calculates the velocity components based on the initial and target positions of the figure.
 * It then continuously updates the position of the figure using the calculated velocities until it reaches the target position.
 * This creates a smooth animation effect for the figure movement.
 *
 * \param window The SFML window in which the game board is displayed.
 * \param figure The CircleShape representing the moving figure.
 * \param initialPosition The initial position of the figure.
 * \param targetPosition The target position to which the figure should move.
 */
    static void animateMove(sf::RenderWindow& window, sf::CircleShape& figure, sf::Vector2f initialPosition, sf::Vector2f targetPosition){
        float dx = targetPosition.x - initialPosition.x;
        float dy = targetPosition.y - initialPosition.y;
        float angleRadians = std::atan2(dy, dx);

        float speed = 0.3f;
        float velocityX = speed * std::cos(angleRadians);
        float velocityY = speed * std::sin(angleRadians);

        while (window.isOpen())
        {
            figure.move(sf::Vector2f (velocityX,velocityY));
            sf::Vector2f currentPosition = figure.getPosition();
            float distance = std::sqrt(std::pow(targetPosition.x - currentPosition.x, 2) +
                                       std::pow(targetPosition.y - currentPosition.y, 2));

            if (distance <= speed)
            {
                velocityX = 0.0f;
                velocityY = 0.0f;
                break;
            }
            window.draw(figure);
            window.display();
        }
    }

    /**
 * \brief Creates a CircleShape object representing a hexagon for the given position on the game board.
 *
 * This method creates a CircleShape object with the appropriate color based on the value of the game board at the given position.
 * It sets the position of the shape based on the coordinates on the game board.
 *
 * \param y The y-coordinate of the position on the game board.
 * \param x The x-coordinate of the position on the game board.
 * \return The CircleShape representing the hexagon at the given position.
 */
    sf::CircleShape writeHexagon(float y, float x) const {
        sf::CircleShape circle(27, 6);

        if  (game.board.getSquare(y, x) == SquareVal::Ruby) {
            sf::Color cl(255,50,50);
            circle.setFillColor(cl);
        }
        else if (game.board.getSquare(y, x) == SquareVal::Pearl) {
            sf::Color cl(200,200,250);
            circle.setFillColor(cl);
        }
        else if (game.board.getSquare(y,x) == SquareVal::Empty){
            sf::Color cl(100, 55, 120);
            circle.setFillColor(cl);
        }
        if (game.board.getSquare(y, x) != SquareVal::Invaluable) {
            sf::Vector2f buttonPosition(x * (circle.getRadius()*2),
                                        y * (circle.getRadius()*2));
            circle.setPosition(buttonPosition);
        }
        return circle;
    }
/**
 * \brief Checks if a hexagon shape is currently selected.
 *
 * This method checks if the outline color of a hexagon shape is set to green, indicating selection.
 *
 * \param hexagon The CircleShape representing the hexagon to check.
 * \return True if the hexagon is selected, false otherwise.
 */
    static bool isChosed(sf::CircleShape& hexagone){
        if (hexagone.getOutlineColor() == sf::Color::Green){
            return true;
        }
        return false;
    }
/**
 * \brief Point valid possible moves.
 *
 * This method marks possible moves for selected figure.
 *
 * \param shape The CircleShape to check.
 * \return True if the shape's position is null, false otherwise.
 */
    void pointMoves(const vector<tuple<int,int>>& validMoves){
        for (const auto& move : validMoves) {
            float moveX = std::get<1>(move) * 54; // Calculate x coordinate based on tuple element
            float moveY = std::get<0>(move) * 54; // Calculate y coordinate based on tuple element
            for (auto& hexagon : hexagones) {
                sf::Vector2f position = hexagon.getPosition();
                if (position.y == moveY && position.x == moveX){
                    if (hexagon.getOutlineColor() == sf::Color::Yellow ){
                        hexagon.setOutlineThickness(0);
                        hexagon.setOutlineColor(sf::Color::Transparent);
                    } else{
                        hexagon.setOutlineColor(sf::Color::Yellow);
                        hexagon.setOutlineThickness(3);
                    }
                    break;
                }
            }
        }
    }
    /**
 * \brief Checks if the position of a shape is null.
 *
 *
 * \param shape The CircleShape to check.
 * \return True if the shape's position is null, false otherwise.
 */
    static bool isShapePositionZero(const sf::CircleShape& shape) {
        sf::Vector2f position = shape.getPosition();
        return position.x == 0 && position.y == 0;
    }
    /**
 * \brief Checks if a clicked figure matches the selected figure.
 *
 * This method compares the position of a clicked figure with the position of the selected figure.
 *
 * \param figure The CircleShape representing the clicked figure.
 * \return True if the figure matches the selected figure, false otherwise.
 */
    bool checkFigureToChoosen(const sf::CircleShape& figure){
        return (figure.getPosition().y/27)/2 == selectedFigure[0] &&
               (figure.getPosition().x/27)/2 == selectedFigure[1];
    }
};

#endif //UNTITLED1_GRAPHICALBOARD_H