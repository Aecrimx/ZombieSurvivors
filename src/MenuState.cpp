#include "MenuState.h"
#include "Animation.h"
#include "Game.h"
#include "GameState.h"
 #include "ResourceLoadException.h"
#include <iostream>

MenuState::MenuState(Game &gameRef)
    : State(gameRef), hoveredIndex(-1), selectedIndex(-1) {
    if (!font.openFromFile("fonts/game_over.ttf")) {
        throw ResourceLoadException("Failed to load font: fonts/game_over.ttf");
    }

    //bakcground image
    ResourceManager &res = game.getResourceManager();
    res.loadTexture("menu_background", "assets/background1.png");
    backgroundSprite.emplace(res.getTexture("menu_background"));

    // scaling stuff
    sf::Vector2f windowSize = game.getWindowSize();
    sf::Vector2u textureSize = res.getTexture("menu_background").getSize();
    float scaleX = windowSize.x / static_cast<float>(textureSize.x);
    float scaleY = windowSize.y / static_cast<float>(textureSize.y);
    backgroundSprite->setScale({scaleX, scaleY});

    // title stuff
    titleText = std::make_unique<sf::Text>(font, "ZOMBIE SURVIVORS", 60);
    titleText->setFillColor(sf::Color::White);
    titleText->setStyle(sf::Text::Bold);

    setupCharacters();

    sf::Vector2f size = game.getWindowSize();
    updateLayout(size.x, size.y);
}

void MenuState::setupCharacters() {
    SaveManager &saveManager = SaveManager::getInstance();
    ResourceManager &res = game.getResourceManager();

    res.loadTexture("wizard", "assets/wizard.png");
    res.loadTexture("demon", "assets/demon.png");
    res.loadTexture("spinning_skull", "assets/spinning_skull_sheet.png");

    CharacterCard wizard;
    wizard.name = "Hocus ( Amazing Wizard )";
    wizard.textureName = "wizard";
    wizard.data =
            CharacterData("Hocus", "wizard", 125.f, 125.f, "Fire wand", "fire_wand");
    wizard.unlocked = saveManager.isCharacterUnlocked("wizard");
    wizard.highScore = saveManager.getHighScore("wizard");
    wizard.sprite = std::make_unique<sf::Sprite>(res.getTexture("wizard"));
    wizard.sprite->setScale({0.3f, 0.3f});

    CharacterCard demon;
    demon.name = "Azoth ( Dapper Demon )";
    demon.textureName = "demon";
    demon.data = CharacterData("Azoth", "demon", 150.f, 100.f, "Demonic Book",
                               "demonic_book");
    demon.unlocked = saveManager.isCharacterUnlocked("demon");
    demon.highScore = saveManager.getHighScore("demon");
    demon.sprite = std::make_unique<sf::Sprite>(res.getTexture("demon"));
    demon.sprite->setScale({0.3f, 0.3f});

    CharacterCard skull;
    skull.name = "Flying Skull ( Rattling... )";
    skull.textureName = "spinning_skull";
    skull.data = CharacterData("Flying Skull", "spinning_skull", 110.f, 100.f,
                               "Bone Shooter", "bone");
    skull.unlocked = saveManager.isCharacterUnlocked("flying_skull");
    skull.highScore = saveManager.getHighScore("flying_skull");
    skull.sprite = std::make_unique<sf::Sprite>(res.getTexture("spinning_skull"));
    skull.sprite->setScale({0.5f, 0.5f});

    skull.animation = std::make_unique<Animation>(*skull.sprite);
    const int frameW = 149;
    const int frameH = 128;
    const sf::Vector2u tsize = res.getTexture("spinning_skull").getSize();
    const int frames = std::max(1u, tsize.y / frameH);
    for (int i = 0; i < frames; ++i) {
        skull.animation->addFrame(
            Frame{sf::IntRect({0, i * frameH}, {frameW, frameH}), 0.05});
    }
    skull.sprite->setOrigin({frameW / 2.f, frameH / 2.f});
    skull.sprite->setTextureRect(sf::IntRect({0, 0}, {frameW, frameH}));

    //setup character cards
    for (auto *card: {&wizard, &demon, &skull}) {
        card->nameText = std::make_unique<sf::Text>(font, card->name, 48);
        card->nameText->setFillColor(sf::Color::White);

        std::string stats =
                "HP: " + std::to_string((int) card->data.maxHealth) +
                " | Speed: " + std::to_string((int) card->data.moveSpeed);
        card->statsText = std::make_unique<sf::Text>(font, stats, 36);
        card->statsText->setFillColor(sf::Color(200, 200, 200));

        card->scoreText = std::make_unique<sf::Text>(
            font, "High Score: " + std::to_string(card->highScore), 36);
        card->scoreText->setFillColor(sf::Color::Yellow);

        card->lockedText = std::make_unique<sf::Text>(font, "\nLOCKED", 30);
        card->lockedText->setFillColor(sf::Color::Red);
        card->lockedText->setStyle(sf::Text::Bold);

        card->cardBg.setFillColor(sf::Color(50, 50, 50, 200));
        card->cardBg.setOutlineThickness(3.f);
        card->cardBg.setOutlineColor(sf::Color(100, 100, 100));
    }

    characters.push_back(std::move(wizard));
    characters.push_back(std::move(demon));
    characters.push_back(std::move(skull));
}

void MenuState::updateLayout(int windowWidth, int windowHeight) {
    if (backgroundSprite) {
        ResourceManager &res = game.getResourceManager();
        sf::Vector2u textureSize = res.getTexture("menu_background").getSize();
        float scaleX = windowWidth / static_cast<float>(textureSize.x);
        float scaleY = windowHeight / static_cast<float>(textureSize.y);
        backgroundSprite->setScale({scaleX, scaleY});
    }

    sf::FloatRect titleBounds = titleText->getGlobalBounds();
    titleText->setPosition({(windowWidth - titleBounds.size.x) / 2.f, 50.f});

    float cardWidth = 250.f;
    float cardHeight = 350.f;
    float spacing = 30.f;
    float totalWidth = (cardWidth * 3) + (spacing * 2);
    float startX = (windowWidth - totalWidth) / 2.f;
    float startY = 200.f;

    for (size_t i = 0; i < characters.size(); ++i) {
        auto &card = characters[i];
        float x = startX + i * (cardWidth + spacing);

        card.cardBg.setSize({cardWidth, cardHeight});
        card.cardBg.setPosition({x, startY});
        card.mouse_bounds = card.cardBg.getGlobalBounds();

        // pozitie sprite pt cards
        if (card.sprite) {
            sf::FloatRect spriteBounds = card.sprite->getGlobalBounds();
            if (card.animation) {
                card.sprite->setPosition({x + cardWidth / 2.f, startY + 85.f});
            } else {
                card.sprite->setPosition(
                    {x + (cardWidth - spriteBounds.size.x) / 2.f, startY + 20.f});
            }
        }

        if (card.nameText)
            card.nameText->setPosition({x + 10.f, startY + 150.f});
        if (card.statsText)
            card.statsText->setPosition({x + 10.f, startY + 190.f});

        if (card.scoreText)
            card.scoreText->setPosition({x + 10.f, startY + 220.f});

        if (card.lockedText) {
            sf::FloatRect lockedBounds = card.lockedText->getGlobalBounds();
            card.lockedText->setPosition({
                x + (cardWidth - lockedBounds.size.x) / 2.f,
                startY + (cardHeight / 2.f)
            });
        }
    }
}

void MenuState::handleInput() {
}

void MenuState::update(float dt) {
    for (auto &card: characters) {
        if (card.animation) {
            card.animation->update(dt);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        game.getWindow().close();
        return;
    }

    sf::Vector2i mousePos = sf::Mouse::getPosition(game.getWindow());
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x),
                           static_cast<float>(mousePos.y));
    // hover pe cards
    hoveredIndex = -1;
    for (size_t i = 0; i < characters.size(); ++i) {
        if (characters[i].unlocked &&
            characters[i].mouse_bounds.contains(mousePosF)) {
            hoveredIndex = static_cast<int>(i);
            characters[i].cardBg.setOutlineColor(sf::Color::Green);
        } else {
            characters[i].cardBg.setOutlineColor(sf::Color(100, 100, 100));
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (hoveredIndex != -1 && selectedIndex == -1) {
            selectedIndex = hoveredIndex;

            game.scheduleReplace(
                std::make_unique<GameState>(game, characters[selectedIndex].data));
            return;
        }
    }
}

void MenuState::draw() {
    game.getWindow().clear();
    if (backgroundSprite) {
        game.getWindow().draw(*backgroundSprite);
    }

    if (titleText)
        game.getWindow().draw(*titleText);

    for (const auto &card: characters) {
        game.getWindow().draw(card.cardBg);
        if (card.sprite)
            game.getWindow().draw(*card.sprite);
        if (card.nameText)
            game.getWindow().draw(*card.nameText);

        if (card.unlocked) {
            if (card.statsText)
                game.getWindow().draw(*card.statsText);
            if (card.scoreText)
                game.getWindow().draw(*card.scoreText);
        } else {
            if (card.lockedText)
                game.getWindow().draw(*card.lockedText);
        }
    }
}

void MenuState::Resize(int w, int h) { updateLayout(w, h); }
