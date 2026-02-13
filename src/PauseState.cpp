#include "PauseState.h"
#include "Game.h"
#include "MenuState.h"
#include "ResourceLoadException.h"
#include <iostream>

PauseState::PauseState(Game &gameRef, State *underlyingState)
    : State(gameRef), continueHovered(false), exitHovered(false),
      gameState(underlyingState) {

    const auto& font = game.getResourceManager().getFont("game_over");

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

void PauseState::handleEvent(const sf::Event &event) {
    if (const auto *keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            game.popState();
        }
    }
}

void PauseState::update(float /*dt*/) {
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

    bool mouseCurrentlyPressed =
            sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    if (mouseWasPressed && !mouseCurrentlyPressed) {
        if (continueHovered) {
            game.popState();
            return;
        } else if (exitHovered) {
            game.popState();
            game.scheduleReplace(std::make_unique<MenuState>(game));
            return;
        }
    }

    mouseWasPressed = mouseCurrentlyPressed;
}

void PauseState::draw() {
    if (gameState) {
        gameState->draw();
    }

    game.getWindow().setView(game.getWindow().getDefaultView());

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
