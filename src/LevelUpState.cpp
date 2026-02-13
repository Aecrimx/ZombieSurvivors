#include "LevelUpState.h"
#include "Game.h"
#include <algorithm>
#include <random>

LevelUpState::LevelUpState(Game &gameRef, Player *playerRef,
                           State *underlyingState)
    : State(gameRef), gameState(underlyingState), hoveredIndex(-1),
      player(playerRef) {

    const auto& font = game.getResourceManager().getFont("game_over");

    titleText = std::make_unique<sf::Text>(font, "LEVEL UP!", 50);
    titleText->setFillColor(sf::Color::Yellow);
    titleText->setStyle(sf::Text::Bold);

    generateOptions();

    sf::Vector2f size = game.getWindowSize();
    updateLayout(size.x, size.y);
}

void LevelUpState::generateOptions() {
    options.clear();

    if (!player) return;

    auto availableUpgradeData = game.getUpgradeManagerFactory().getUpgradeOptions(*player);

    /*
     * Am folosit un random generator din std in loc de un clasic rand().
     * note to self: rd da seed, std::mt19937 este un mersenne twister RNG, apoi
     * doar dau shuffle la vector.
     */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(availableUpgradeData.begin(), availableUpgradeData.end(), gen);

    const int numOptions = std::min(3, static_cast<int>(availableUpgradeData.size()));
    for (int i = 0; i < numOptions; ++i) {
        UpgradeOption opt;
        opt.name = availableUpgradeData[i].name;
        opt.description = availableUpgradeData[i].description;
        opt.applyUpgrade = availableUpgradeData[i].apply;
        opt.hovered = false;

        const auto& font = game.getResourceManager().getFont("game_over");

        opt.nameText = std::make_unique<sf::Text>(font, opt.name, 28);
        opt.nameText->setFillColor(sf::Color::White);
        opt.nameText->setStyle(sf::Text::Bold);

        opt.descText = std::make_unique<sf::Text>(font, opt.description, 20);
        opt.descText->setFillColor(sf::Color(200, 200, 200));

        opt.bg.setFillColor(sf::Color(40, 40, 40, 220));
        opt.bg.setOutlineThickness(3.f);
        opt.bg.setOutlineColor(sf::Color(80, 80, 80));

        options.push_back(std::move(opt));
    }
}

void LevelUpState::updateLayout(const int windowWidth, int /*windowHeight*/) {
    if (titleText) {
        const sf::FloatRect titleBounds = titleText->getGlobalBounds();
        titleText->setPosition({(windowWidth - titleBounds.size.x) / 2.f, 50.f});
    }

    float boxWidth = 500.f;
    float boxHeight = 100.f;
    float spacing = 20.f;
    float startY = 150.f;
    float startX = (windowWidth - boxWidth) / 2.f;

    for (size_t i = 0; i < options.size(); ++i) {
        auto &opt = options[i];
        float y = startY + i * (boxHeight + spacing);

        opt.bg.setSize({boxWidth, boxHeight});
        opt.bg.setPosition({startX, y});
        opt.bounds = opt.bg.getGlobalBounds();

        if (opt.nameText) {
            opt.nameText->setPosition({startX + 20.f, y + 15.f});
        }
        if (opt.descText) {
            opt.descText->setPosition({startX + 20.f, y + 55.f});
        }
    }
}

void LevelUpState::handleInput() {
}

void LevelUpState::update(float /*dt*/) {
    const sf::Vector2i mousePos = sf::Mouse::getPosition(game.getWindow());
    const sf::Vector2f mousePosF(static_cast<float>(mousePos.x),
                                 static_cast<float>(mousePos.y));

    hoveredIndex = -1;
    for (size_t i = 0; i < options.size(); ++i) {
        if (options[i].bounds.contains(mousePosF)) {
            hoveredIndex = static_cast<int>(i);
            options[i].bg.setOutlineColor(sf::Color::Green);
            options[i].hovered = true;
        } else {
            options[i].bg.setOutlineColor(sf::Color(80, 80, 80));
            options[i].hovered = false;
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (hoveredIndex != -1 && player) {
            options[hoveredIndex].applyUpgrade(*player);
            game.popState();
        }
    }
}

void LevelUpState::draw() {
    if (gameState) {
        gameState->draw();
    }
    game.getWindow().setView(game.getWindow().getDefaultView());

    sf::RectangleShape overlay({game.getWindowSize().x, game.getWindowSize().y});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));

    game.getWindow().draw(overlay);
    if (titleText) {
        game.getWindow().draw(*titleText);
    }

    for (const auto &opt: options) {
        game.getWindow().draw(opt.bg);
        if (opt.nameText)
            game.getWindow().draw(*opt.nameText);
        if (opt.descText)
            game.getWindow().draw(*opt.descText);
    }
}

void LevelUpState::Resize(const int w, const int h) { updateLayout(w, h); }
