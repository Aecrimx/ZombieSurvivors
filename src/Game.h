//
// Created by Mircea on 13.12.2025.
//

#ifndef OOP_GAME_H
#define OOP_GAME_H

#include "ResourceManager.h"
#include "UpgradeManagerFactory.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <ostream>
#include <stack>

class State;

class Game {
    sf::RenderWindow window;
    ResourceManager resourceManager;
    std::stack<std::unique_ptr<State> > states;

    std::string title;

    int width, height;

    int fullscreen_toggle;
    bool shouldExit = false;
    /*
     * Cand MenuState da pop la el insusi si da push la Gamestate folosim
     * intermediarul pending pt a nu da segfault
     */
    std::unique_ptr<State> pendingState;
    bool hasPendingReplace = false;
    int pendingPops = 0;

    UpgradeManagerFactory upgradeManagerFactory;

public:
    Game(int width, int height, const std::string &title);

    void run();

    void pushState(std::unique_ptr<State> state);

    void popState();

    void scheduleReplace(std::unique_ptr<State> state);

    sf::RenderWindow &getWindow();

    ResourceManager &getResourceManager();

    static sf::View LetterboxView(sf::View view, int winWidth, int winHeight);

    sf::Vector2f getWindowSize() const;

    UpgradeManagerFactory& getUpgradeManagerFactory() { return upgradeManagerFactory; }

    friend std::ostream &operator<<(std::ostream &os, const Game &obj) {
        return os << "[Game]"
               << "title: " << obj.title << " width: " << obj.width
               << " height: " << obj.height
               << " fullscreen_toggle: " << obj.fullscreen_toggle
               << " shouldExit: " << obj.shouldExit;
    }
};

#endif // OOP_GAME_H
