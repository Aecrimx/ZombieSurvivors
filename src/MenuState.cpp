#include "MenuState.h"
#include "Game.h"
#include "GameState.h"
#include <iostream>

MenuState::MenuState(Game &gameRef)
    : State(gameRef), hoveredIndex(-1), selectedIndex(-1) {
    if (!font.openFromFile("fonts/game_over.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    //title stuff
    titleText.setFont(font);
    titleText.setString("ZOMBIE SURVIVORS");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

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
    wizard.name = "Hocus (Wizard)";
    wizard.textureName = "wizard";
    wizard.data =
            CharacterData("Hocus", "wizard", 125.f, 125.f, "Fire wand", "fire_wand");
    wizard.unlocked = saveManager.isCharacterUnlocked("wizard");
    wizard.highScore = saveManager.getHighScore("wizard");
    wizard.sprite.setTexture(res.getTexture("wizard"));
    wizard.sprite.setScale({0.3f, 0.3f});

    CharacterCard demon;
    demon.name = "Azoth (Demon)";
    demon.textureName = "demon";
    demon.data = CharacterData("Azoth", "demon", 150.f, 100.f, "Demonic Book",
                               "demonic_book");
    demon.unlocked = saveManager.isCharacterUnlocked("demon");
    demon.highScore = saveManager.getHighScore("demon");
    demon.sprite.setTexture(res.getTexture("demon"));
    demon.sprite.setScale({0.3f, 0.3f});

    CharacterCard skull;
    skull.name = "Skull (Undead)";
    skull.textureName = "spinning_skull";
    skull.data = CharacterData("Flying Skull", "spinning_skull", 110.f, 100.f,
                               "Skull Shooter", "skull");
    skull.unlocked = saveManager.isCharacterUnlocked("flying_skull");
    skull.highScore = saveManager.getHighScore("flying_skull");
    skull.sprite.setTexture(res.getTexture("spinning_skull"));
    skull.sprite.setScale({0.2f, 0.2f});

    // setup character cards
    for (auto *card: {&wizard, &demon, &skull}) {
        card->nameText.setFont(font);
        card->nameText.setCharacterSize(24);
        card->nameText.setFillColor(sf::Color::White);
        card->nameText.setString(card->name);

        card->statsText.setFont(font);
        card->statsText.setCharacterSize(18);
        card->statsText.setFillColor(sf::Color(200, 200, 200));
        std::string stats =
                "HP: " + std::to_string((int) card->data.maxHealth) +
                " | Speed: " + std::to_string((int) card->data.moveSpeed);
        card->statsText.setString(stats);

        card->scoreText.setFont(font);
        card->scoreText.setCharacterSize(18);
        card->scoreText.setFillColor(sf::Color::Yellow);
        card->scoreText.setString("High Score: " + std::to_string(card->highScore));

        card->lockedText.setFont(font);
        card->lockedText.setCharacterSize(30);
        card->lockedText.setFillColor(sf::Color::Red);
        card->lockedText.setString("LOCKED");
        card->lockedText.setStyle(sf::Text::Bold);

        card->cardBg.setFillColor(sf::Color(50, 50, 50, 200));
        card->cardBg.setOutlineThickness(3.f);
        card->cardBg.setOutlineColor(sf::Color(100, 100, 100));
    }

    characters.push_back(wizard);
    characters.push_back(demon);
    characters.push_back(skull);
}

void MenuState::updateLayout(int windowWidth, int windowHeight) {
    sf::FloatRect titleBounds = titleText.getGlobalBounds();
    titleText.setPosition({(windowWidth - titleBounds.size.x) / 2.f, 50.f});

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

        //pozitie sprite pt cards
        sf::FloatRect spriteBounds = card.sprite.getGlobalBounds();
        card.sprite.setPosition({
            x + (cardWidth - spriteBounds.size.x) / 2.f,
            startY + 20.f
        });

        card.nameText.setPosition({x + 10.f, startY + 150.f});
        card.statsText.setPosition({x + 10.f, startY + 190.f});

        card.scoreText.setPosition({x + 10.f, startY + 220.f});

        sf::FloatRect lockedBounds = card.lockedText.getGlobalBounds();
        card.lockedText.setPosition({
            x + (cardWidth - lockedBounds.size.x) / 2.f,
            startY + (cardHeight / 2.f)
        });
    }
}

void MenuState::handleInput() {
}

void MenuState::update(float dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(game.getWindow());
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x),
                           static_cast<float>(mousePos.y));
    //hover pe cards
    hoveredIndex = -1;
    for (size_t i = 0; i < characters.size(); ++i) {
        if (characters[i].unlocked && characters[i].mouse_bounds.contains(mousePosF)) {
            hoveredIndex = static_cast<int>(i);
            characters[i].cardBg.setOutlineColor(sf::Color::Green);
        } else {
            characters[i].cardBg.setOutlineColor(sf::Color(100, 100, 100));
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (hoveredIndex != -1 && selectedIndex == -1) {
            selectedIndex = hoveredIndex;

            game.popState();
            game.pushState(
                std::make_unique<GameState>(game, characters[selectedIndex].data));
        }
    }
}

void MenuState::draw() {
    game.getWindow().clear(sf::Color(255, 255, 255));

    game.getWindow().draw(titleText);


    for (const auto &card: characters) {
        game.getWindow().draw(card.cardBg);
        game.getWindow().draw(card.sprite);
        game.getWindow().draw(card.nameText);

        if (card.unlocked) {
            game.getWindow().draw(card.statsText);
            game.getWindow().draw(card.scoreText);
        } else {
            game.getWindow().draw(card.lockedText);
        }
    }
}

void MenuState::Resize(int w, int h) { updateLayout(w, h); }
