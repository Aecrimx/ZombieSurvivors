#include "LevelUpState.h"
#include "Armor.h"
#include "Boots.h"
#include "CooldownGauntlet.h"
#include "DemonicBook.h"
#include "FireWand.h"
#include "Game.h"
#include "HeartCrystal.h"
#include "MagicGun.h"
#include "ResourceLoadException.h"
#include "SoulLantern.h"
#include <algorithm>
#include <iostream>
#include <random>

LevelUpState::LevelUpState(Game &gameRef, Player *playerRef,
                           State *underlyingState)
    : State(gameRef), hoveredIndex(-1), player(playerRef),
      gameState(underlyingState) {
    if (!font.openFromFile("fonts/game_over.ttf")) {
        throw ResourceLoadException("Failed to load font: fonts/game_over.ttf");
    }

    titleText = std::make_unique<sf::Text>(font, "LEVEL UP!", 50);
    titleText->setFillColor(sf::Color::Yellow);
    titleText->setStyle(sf::Text::Bold);

    generateOptions();

    sf::Vector2f size = game.getWindowSize();
    updateLayout(size.x, size.y);
}

void LevelUpState::generateOptions() {
    options.clear();

    struct PossibleUpgrade {
        std::string name;
        std::string desc;
        std::function<void(Player &)> apply;
        bool available;
    };

    std::vector<PossibleUpgrade> allUpgrades;

    const auto &items = player->getItems();
    bool hasArmor = false, hasBoots = false, hasCooldown = false,
            hasHeart = false;

    for (const auto &item: items) {
        if (item->getName() == "Armor")
            hasArmor = true;
        if (item->getName() == "Boots")
            hasBoots = true;
        if (item->getName() == "Cooldown Gauntlet")
            hasCooldown = true;
        if (item->getName() == "Heart Crystal")
            hasHeart = true;
    }

    if (!hasArmor && items.size() < 3) {
        allUpgrades.push_back(
            {
                "Armor", "+10% Damage Reduction",
                [](Player &p) { p.addItem(std::make_unique<Armor>()); }, true
            });
    }
    if (!hasBoots && items.size() < 3) {
        allUpgrades.push_back(
            {
                "Boots", "+10% Movement Speed",
                [](Player &p) { p.addItem(std::make_unique<Boots>()); }, true
            });
    }
    if (!hasCooldown && items.size() < 3) {
        allUpgrades.push_back(
            {
                "Cooldown Gauntlet", "+16.67% Cooldown Reduction",
                [](Player &p) { p.addItem(std::make_unique<CooldownGauntlet>()); },
                true
            });
    }
    if (!hasHeart && items.size() < 3) {
        allUpgrades.push_back(
            {
                "Heart Crystal", "+10 Max HP, +0.5 HP/sec Regen",
                [](Player &p) { p.addItem(std::make_unique<HeartCrystal>()); }, true
            });
    }

    for (auto &item: items) {
        if (item->canLevelUp()) {
            std::string itemName = item->getName();
            allUpgrades.push_back({
                itemName + " Level Up",
                "Upgrade " + itemName + " to Level " +
                std::to_string(item->getLevel() + 1),
                [itemName](Player &p) {
                    for (auto &i: p.getItems()) {
                        if (i->getName() == itemName &&
                            i->canLevelUp()) {
                            i->levelUp();
                            break;
                        }
                    }
                },
                true
            });
        }
    }

    const auto &weapons = player->getWeapons();
    if (weapons.size() < 3) {
        bool hasMagicGun = false, hasSoulLantern = false, hasFireWand = false,
                hasDemonicBook = false;
        for (const auto &weapon: weapons) {
            if (weapon->getName() == "Magic Gun")
                hasMagicGun = true;
            if (weapon->getName() == "Soul Lantern")
                hasSoulLantern = true;
            if (weapon->getName() == "Fire wand")
                hasFireWand = true;
            if (weapon->getName() == "Demonic Book")
                hasDemonicBook = true;
        }

        if (!hasMagicGun) {
            allUpgrades.push_back(
                {
                    "Magic Gun", "Shoots lasers in a clock-wise pattern.",
                    [this](Player &p) {
                        p.addWeapon(std::make_unique<MagicGun>(game.getResourceManager()));
                    },
                    true
                });
        }
        if (!hasSoulLantern) {
            allUpgrades.push_back(
                {
                    "Soul Lantern", "Shoots a soul scream in a direction.",
                    [this](Player &p) {
                        p.addWeapon(
                            std::make_unique<SoulLantern>(game.getResourceManager()));
                    },
                    true
                });
        }
        if (!hasFireWand) {
            allUpgrades.push_back(
                {
                    "Fire wand", "Shoots fireballs towards enemies.",
                    [this](Player &p) {
                        p.addWeapon(std::make_unique<FireWand>(game.getResourceManager()));
                    },
                    true
                });
        }
        if (!hasDemonicBook) {
            allUpgrades.push_back(
                {
                    "Demonic Book", "Damaging aura that periodically damages enemies.",
                    [this](Player &p) {
                        p.addWeapon(
                            std::make_unique<DemonicBook>(game.getResourceManager()));
                    },
                    true
                });
        }
    }

    allUpgrades.push_back({
        "Max HP Up", "+20 Maximum Health",
        [](Player &p) { p.increaseMaxHealth(20.f); }, true
    });
    allUpgrades.push_back(
        {
            "Health Regen", "+1.0 HP/sec Regeneration",
            [](Player &p) { p.setHealthRegen(p.getHealthRegen() + 1.0f); }, true
        });
    allUpgrades.push_back({
        "Speed Up", "+15% Movement Speed",
        [](Player &p) { p.increaseSpeed(1.15f); }, true
    });

    // filtrare available items
    std::vector<PossibleUpgrade> available;
    for (const auto &upgrade: allUpgrades) {
        if (upgrade.available) {
            available.push_back(upgrade);
        }
    }

    /*
     * Am folosit un random generator din std in loc de un clasic rand().
     * note to self: rd da seed, std::mt19937 este un mersenne twister RNG, apoi
     * doar dau shuffle la vector.
     */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(available.begin(), available.end(), gen);

    int numOptions = std::min(3, (int) available.size());
    for (int i = 0; i < numOptions; ++i) {
        UpgradeOption opt;
        opt.name = available[i].name;
        opt.description = available[i].desc;
        opt.applyUpgrade = available[i].apply;
        opt.hovered = false;

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

void LevelUpState::updateLayout(int windowWidth, int windowHeight) {
    if (titleText) {
        sf::FloatRect titleBounds = titleText->getGlobalBounds();
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

void LevelUpState::update(float dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(game.getWindow());
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x),
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

void LevelUpState::Resize(int w, int h) { updateLayout(w, h); }
