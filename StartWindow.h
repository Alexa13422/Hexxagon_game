//
// Created by andra on 19.05.2023.
//

#ifndef UNTITLED_STARTWINDOW_H
#define UNTITLED_STARTWINDOW_H


#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include "GraphicalBoard.h"

/**

@class StartWindow
@brief The StartWindow class represents the starting window of the game.
The StartWindow class is responsible for displaying the initial window of the game,
which contains buttons to start the game or exit the program.
*/
class StartWindow{
private:
    float windowWidth = 800.0f;
    float windowHeight = 600.0f;

public:
    /**
    @brief Default constructor for the StartWindow class.
     */
    StartWindow() {

        sf::RenderWindow window(sf::VideoMode({800, 600}), "Board with Buttons");
        sf::Event event{};

        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("Images/Background.jpg")) {
            std::cout<<"O OU";
        }
        sf::Sprite background(backgroundTexture);

        sf::Vector2f buttonSize(200.0f, 100.0f);
        sf::Vector2f buttonPosition(windowWidth / 2 - buttonSize.x * 1.5 , windowHeight / 2 - buttonSize.y / 2);
        bool isStartButtonHighlighted = false;
        bool isExitButtonHighlighted = false;

        sf::RectangleShape buttonStart(buttonSize);
        buttonStart.setPosition(buttonPosition);
        buttonStart.setFillColor(sf::Color::Green);

        sf::RectangleShape buttonExit(buttonSize);
        buttonPosition.x += buttonSize.x * 2;
        buttonExit.setPosition(buttonPosition);
        buttonExit.setFillColor(sf::Color::Red);

        sf::Color originalStartButtonColor = buttonStart.getFillColor();
        sf::Color originalExitButtonColor = buttonExit.getFillColor();
        sf::Color highlightedStartColor(100, 255, 100);
        sf::Color highlightedExitColor(255, 100, 100);

        while (window.isOpen()) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                        auto mousePos = static_cast<sf::Vector2f>(mousePosition);

                        if (buttonStart.getGlobalBounds().contains(mousePos)) {

                            GraphicalBoard graphicalBoard;
                        } else if (buttonExit.getGlobalBounds().contains(mousePos)) {

                            window.close();
                        }
                    }
                } else if(event.type == sf::Event::MouseMoved){
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    auto mousePos = static_cast<sf::Vector2f>(mousePosition);
                    if (buttonStart.getGlobalBounds().contains(mousePos)) {

                        isStartButtonHighlighted = true;
                    } else {

                        isStartButtonHighlighted = false;
                    }
                    if (buttonExit.getGlobalBounds().contains(mousePos)) {

                        isExitButtonHighlighted = true;
                    } else {

                        isExitButtonHighlighted = false;
                    }
                }
            }

            if (isStartButtonHighlighted) {
                buttonStart.setFillColor(highlightedStartColor);
            } else {
                buttonStart.setFillColor(originalStartButtonColor);
            }
            if (isExitButtonHighlighted) {
                buttonExit.setFillColor(highlightedExitColor);
            } else {
                buttonExit.setFillColor(originalExitButtonColor);
            }
            sf::Texture buttonStartTexture, buttonExitTexture;
            if (!buttonStartTexture.loadFromFile("Images/Start.png")) {
                std::cout << "File not loaded";
            }
            if (!buttonExitTexture.loadFromFile("Images/Exit.png")) {
                std::cout << "File not loaded";
            }

            buttonStart.setTexture(&buttonStartTexture);
            buttonExit.setTexture(&buttonExitTexture);

            window.clear(sf::Color::White);

            window.draw(background);
            window.draw(buttonStart);
            window.draw(buttonExit);


            window.display();
        }
    }
};

#endif //UNTITLED_STARTWINDOW_H