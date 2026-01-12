#include "PauseState.h"
#include "Game.h"
#include <iostream>

PauseState::PauseState(Game &gameRef)
    : State(gameRef), continueHovered(false), exitHovered(false) {
    if (!font.openFromFile("fonts/game_over.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    titleText = std::make_unique<sf::Text>(font, "PAUSED", 60);
    titleText->setFillColor(sf::Color::White);
    titleText->setStyle(sf::Text::Bold);

    continueText = std::make_unique<sf::Text>(font, "Continue", 40);
    continueText->setFillColor(sf::Color::White);

    exitText = std::make_unique<sf::Text>(font, "Exit to Menu", 40);
    exitText->setFillColor(sf::Color::White);

    sf::Vector2f size = game.getWindowSize();
    updateLayout(size.x, size.y);
}

void PauseState::updateLayout(int windowWidth, int windowHeight) {
    if (titleText) {
        sf::FloatRect titleBounds = titleText->getGlobalBounds();
        titleText->setPosition(
            {(windowWidth - titleBounds.size.x) / 2.f, windowHeight / 4.f});
    }

    if (continueText) {
        sf::FloatRect contBounds = continueText->getGlobalBounds();
        continueText->setPosition(
            {(windowWidth - contBounds.size.x) / 2.f, windowHeight / 2.f});
        continueBounds = continueText->getGlobalBounds();
    }

    if (exitText) {
        sf::FloatRect exBounds = exitText->getGlobalBounds();
        exitText->setPosition(
            {(windowWidth - exBounds.size.x) / 2.f, windowHeight / 2.f + 80.f});
        exitBounds = exitText->getGlobalBounds();
    }
}

void PauseState::handleInput() {
}

void PauseState::update(float dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(game.getWindow());
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x),
                           static_cast<float>(mousePos.y));

    continueHovered = continueBounds.contains(mousePosF);
    exitHovered = exitBounds.contains(mousePosF);

    if (continueText) {
        continueText->setFillColor(continueHovered
                                       ? sf::Color::Green
                                       : sf::Color::White);
    }
    if (exitText) {
        exitText->setFillColor(exitHovered ? sf::Color::Red : sf::Color::White);
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (continueHovered) {
            game.popState();
        } else if (exitHovered) {
            game.popState();
            game.popState();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        game.popState();
    }
}

void PauseState::draw() {
    sf::RectangleShape overlay({game.getWindowSize().x, game.getWindowSize().y});
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    game.getWindow().draw(overlay);

    if (titleText)
        game.getWindow().draw(*titleText);
    if (continueText)
        game.getWindow().draw(*continueText);
    if (exitText)
        game.getWindow().draw(*exitText);
}

void PauseState::Resize(int w, int h) { updateLayout(w, h); }
