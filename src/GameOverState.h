#ifndef OOP_GAMEOVERSTATE_H
#define OOP_GAMEOVERSTATE_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class GameOverState : public State {
private:
    // sf::Font font;
    std::unique_ptr<sf::Text> titleText;
    std::unique_ptr<sf::Text> scoreText;
    std::unique_ptr<sf::Text> highScoreText;
    std::unique_ptr<sf::Text> retryText;
    std::unique_ptr<sf::Text> menuText;

    sf::FloatRect retryBounds;
    sf::FloatRect menuBounds;

    bool retryHovered;
    bool menuHovered;
    bool victory;

    int finalScore;
    std::string characterName;

    State *gameState;
    bool mouseWasPressed = false;

    void updateLayout(int windowWidth, int windowHeight);

public:
    GameOverState(Game &gameRef, int score, const std::string &charName,
                  bool isVictory, State *underlyingState = nullptr);

    void handleInput() override;

    void update(float dt) override;

    void draw() override;

    void Resize(int w, int h) override;
};

#endif // OOP_GAMEOVERSTATE_H
