#include "GameOverState.h"
#include "Game.h"
#include "MenuState.h"
#include "ResourceLoadException.h"

GameOverState::GameOverState(Game &gameRef, const int score,
                             const std::string &charName, const bool isVictory,
                             State *underlyingState)
    : State(gameRef), retryHovered(false), menuHovered(false),
      victory(isVictory), finalScore(score), characterName(charName),
      gameState(underlyingState) {

    const auto& font = game.getResourceManager().getFont("game_over");

    std::string titleStr = victory ? "YOU WON" : "GAME OVER";
    const sf::Color titleColor = victory ? sf::Color::Yellow : sf::Color::Red;

    titleText = std::make_unique<sf::Text>(font, titleStr, 70);
    titleText->setFillColor(titleColor);
    titleText->setStyle(sf::Text::Bold);

    scoreText = std::make_unique<sf::Text>(
        font, "Score: " + std::to_string(finalScore), 40);
    scoreText->setFillColor(sf::Color::White);

    SaveManager &saveManager = SaveManager::getInstance();
    int highScore = saveManager.getHighScore(characterName);

    if (finalScore > highScore) {
        saveManager.updateHighScore(characterName, finalScore);
        highScore = finalScore;
    }
    if (victory) {
        saveManager.incrementWins();
    }

    highScoreText = std::make_unique<sf::Text>(
        font, "High Score: " + std::to_string(highScore), 35);
    highScoreText->setFillColor(
        sf::Color(255, 215, 0)); // rgb value pt culoarea aurului

    retryText = std::make_unique<sf::Text>(font, "Retry", 40);
    retryText->setFillColor(sf::Color::White);

    menuText = std::make_unique<sf::Text>(font, "Main Menu", 40);
    menuText->setFillColor(sf::Color::White);

    const sf::Vector2f size = game.getWindowSize();
    updateLayout(size.x, size.y);
}

void GameOverState::updateLayout(const int windowWidth, const int windowHeight) {
    float startY = windowHeight / 5.f;

    if (titleText) {
        const sf::FloatRect titleBounds = titleText->getGlobalBounds();
        titleText->setPosition({(windowWidth - titleBounds.size.x) / 2.f, startY});
    }

    if (scoreText) {
        const sf::FloatRect scoreBounds = scoreText->getGlobalBounds();
        scoreText->setPosition(
            {(windowWidth - scoreBounds.size.x) / 2.f, startY + 100.f});
    }

    if (highScoreText) {
        const sf::FloatRect hsBounds = highScoreText->getGlobalBounds();
        highScoreText->setPosition(
            {(windowWidth - hsBounds.size.x) / 2.f, startY + 160.f});
    }

    if (retryText) {
        const sf::FloatRect retryB = retryText->getGlobalBounds();
        retryText->setPosition(
            {(windowWidth - retryB.size.x) / 2.f, startY + 250.f});
        retryBounds = retryText->getGlobalBounds();
    }

    if (menuText) {
        const sf::FloatRect menuB = menuText->getGlobalBounds();
        menuText->setPosition({(windowWidth - menuB.size.x) / 2.f, startY + 320.f});
        menuBounds = menuText->getGlobalBounds();
    }
}

void GameOverState::handleInput() {
}

void GameOverState::update(float /*dt*/) {
    const sf::Vector2i mousePos = sf::Mouse::getPosition(game.getWindow());
    const sf::Vector2f mousePosF(static_cast<float>(mousePos.x),
                           static_cast<float>(mousePos.y));

    // pt mouse hovering
    retryHovered = retryBounds.contains(mousePosF);
    menuHovered = menuBounds.contains(mousePosF);

    if (retryText) {
        retryText->setFillColor(retryHovered ? sf::Color::Green : sf::Color::White);
    }
    if (menuText) {
        menuText->setFillColor(menuHovered ? sf::Color::Yellow : sf::Color::White);
    }

    const bool mouseCurrentlyPressed =
            sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    if (mouseWasPressed && !mouseCurrentlyPressed) {
        if (retryHovered) {
            game.popState();
            game.scheduleReplace(std::make_unique<MenuState>(game));
            return;
        } else if (menuHovered) {
            game.popState();
            game.scheduleReplace(std::make_unique<MenuState>(game));
            return;
        }
    }

    mouseWasPressed = mouseCurrentlyPressed;
}

void GameOverState::draw() {
    if (gameState) {
        gameState->draw();
    }

    game.getWindow().setView(game.getWindow().getDefaultView());

    sf::RectangleShape overlay({game.getWindowSize().x, game.getWindowSize().y});
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    game.getWindow().draw(overlay);

    if (titleText)
        game.getWindow().draw(*titleText);
    if (scoreText)
        game.getWindow().draw(*scoreText);
    if (highScoreText)
        game.getWindow().draw(*highScoreText);
    if (retryText)
        game.getWindow().draw(*retryText);
    if (menuText)
        game.getWindow().draw(*menuText);
}

void GameOverState::Resize(const int w, const int h) { updateLayout(w, h); }
