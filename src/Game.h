//
// Created by Mircea on 13.12.2025.
//

#ifndef OOP_GAME_H
#define OOP_GAME_H

#include <SFML/Graphics.hpp>
#include <stack>
#include <memory>
#include "ResourceManager.h"

class State;

class Game {
    sf::RenderWindow window;
    ResourceManager resourceManager;
    std::stack<std::unique_ptr<State>> states;

    std::string title;

    int width, height;

    int fullscreen_toggle;
    bool shouldExit = false;

public:
    Game(int width, int height, const std::string& title);

    void run();

    void pushState(std::unique_ptr<State> state);
    void popState();

    sf::RenderWindow& getWindow();
    ResourceManager& getResourceManager();

    sf::Vector2f getWindowSize() const;
};


#endif //OOP_GAME_H