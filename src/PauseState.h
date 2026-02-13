#ifndef OOP_PAUSESTATE_H
#define OOP_PAUSESTATE_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <memory>

class PauseState : public State {
private:
    // sf::Font font;
    std::unique_ptr<sf::Text> titleText;
    std::unique_ptr<sf::Text> continueText;
    std::unique_ptr<sf::Text> exitText;

    sf::FloatRect continueBounds;
    sf::FloatRect exitBounds;

    bool continueHovered;
    bool exitHovered;

    State *gameState;
    bool mouseWasPressed = false;
    bool escWasPressed = false;

    void updateLayout(int windowWidth, int windowHeight);

public:
    explicit PauseState(Game &gameRef, State *underlyingState = nullptr);

    void handleInput() override;

    void handleEvent(const sf::Event &event) override;

    void update(float dt) override;

    void draw() override;

    void Resize(int w, int h) override;
};

#endif // OOP_PAUSESTATE_H
