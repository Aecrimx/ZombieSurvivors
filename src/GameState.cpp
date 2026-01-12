#include "GameState.h"
#include "DemonicBook.h"
#include "FireWand.h"
#include "Game.h"
#include "Zombie.h"
#include <cmath>
#include <iostream>

#include "Flying_Skull.h"

GameState::GameState(Game &ref) : State(ref) {
    ResourceManager &res = game.getResourceManager();

    res.loadTexture("fire_wand", "assets/fire_wand.png");
    res.loadTexture("zombie", "assets/zombie.png");
    res.loadTexture("fireball", "assets/fireball.png");
    res.loadTexture("wizard", "assets/wizard.png");
    res.loadTexture("spinning_skull", "assets/spinning_skull_sheet.png");
    res.loadTexture("slot", "assets/inventory_slot.png");
    res.loadTexture("demon", "assets/demon.png");
    res.loadTexture("damage_aura", "assets/damage_aura_demonic_book64x64.png");
    res.loadTexture("demonic_book", "assets/demonic_book32x32.png");
    res.loadTexture("laser_projectile", "assets/laser_projectile512x16.png");
    res.loadTexture("skull", "assets/skull_projectile.png");
    res.loadTexture("background", "assets/grass.png");
    res.getTexture("background").setRepeated(true);

    CharacterData Azoth = CharacterData("Azoth", "demon", 150.f, 100.f,
                                        "Demonic Book", "demonic_book");

    player = std::make_unique<Player>(Azoth, res);

    background = std::make_unique<sf::Sprite>(
        game.getResourceManager().getTexture("background"));
    background->setTextureRect(sf::IntRect({0, 0}, {10000, 10000}));
    view.setSize({game.getWindowSize().x, game.getWindowSize().y});
    view.setCenter(player->getPos());

    sf::Vector2f winSize = game.getWindowSize();
    GameState::Resize((int) winSize.x, (int) winSize.y);

    sf::Vector2u size = game.getWindow().getSize();
    hud = std::make_unique<HUD>(res, size.x, size.y);

    GameState::Resize(static_cast<int>(size.x), static_cast<int>(size.y));
}

GameState::GameState(Game &ref, const CharacterData &charData) : State(ref) {
    ResourceManager &res = game.getResourceManager();

    res.loadTexture("fire_wand", "assets/fire_wand.png");
    res.loadTexture("zombie", "assets/zombie.png");
    res.loadTexture("fireball", "assets/fireball.png");
    res.loadTexture("wizard", "assets/wizard.png");
    res.loadTexture("spinning_skull", "assets/spinning_skull_sheet.png");
    res.loadTexture("slot", "assets/inventory_slot.png");
    res.loadTexture("demon", "assets/demon.png");
    res.loadTexture("damage_aura", "assets/damage_aura_demonic_book64x64.png");
    res.loadTexture("demonic_book", "assets/demonic_book32x32.png");
    res.loadTexture("laser_projectile", "assets/laser_projectile512x16.png");
    res.loadTexture("skull", "assets/skull_projectile.png");
    res.loadTexture("background", "assets/grass.png");
    res.getTexture("background").setRepeated(true);


    player = std::make_unique<Player>(charData, res);

    background = std::make_unique<sf::Sprite>(
        game.getResourceManager().getTexture("background"));
    background->setTextureRect(sf::IntRect({0, 0}, {10000, 10000}));
    view.setSize({game.getWindowSize().x, game.getWindowSize().y});
    view.setCenter(player->getPos());

    sf::Vector2f winSize = game.getWindowSize();
    GameState::Resize((int) winSize.x, (int) winSize.y);

    sf::Vector2u size = game.getWindow().getSize();
    hud = std::make_unique<HUD>(res, size.x, size.y);

    GameState::Resize(static_cast<int>(size.x), static_cast<int>(size.y));
}

void GameState::Resize(int w, int h) {
    view = game.LetterboxView(view, w, h);
    if (hud) {
        const sf::FloatRect vp = view.getViewport();
        const int lw =
                static_cast<int>(std::round(static_cast<float>(w) * vp.size.x));
        const int lh =
                static_cast<int>(std::round(static_cast<float>(h) * vp.size.y));
        hud->onResize(lw, lh);
    }
}

void GameState::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        // idk yet unde sa o tin
        game.getWindow().close();
    }
}

void GameState::update(float dt) {
    if (!player) {
        return;
    }

    // const sf::Vector2f playerPos = player->getPos();
    spawnTimer += dt;
    if (spawnTimer > 1.5f) {
        spawnTimer = 0;
        float angle = (rand() % 360) * 3.14f / 180.f; // random angle
        float dist = 400.f;
        sf::Vector2f spawnPos =
                player->getPos() +
                sf::Vector2f(std::cos(angle) * dist, std::sin(angle) * dist);

        // schimbat la prefered spawn time dar rand momentan
        if (rand() % 2 == 0) {
            enemies.push_back(std::make_unique<Zombie>(
                game.getResourceManager().getTexture("zombie"), spawnPos));
        } else {
            enemies.push_back(std::make_unique<Flying_Skull>(
                game.getResourceManager().getTexture("spinning_skull"), spawnPos,
                projectiles, game.getResourceManager()));
        }
    }

    player->update(dt, game.getWindow(), enemies, projectiles);
    view.setCenter(player->getPos());

    for (auto &enemy: enemies) {
        enemy->update(dt, player->getPos());
    }

    for (auto it = projectiles.begin(); it != projectiles.end();) {
        it->update(dt);

        bool hit = false;
        const sf::FloatRect a = it->getBounds();

        if (it->isHostile() && player) {
            const sf::FloatRect pb = player->getBounds();
            bool overlapP = (a.position.x < pb.position.x + pb.size.x &&
                             a.position.x + a.size.x > pb.position.x &&
                             a.position.y < pb.position.y + pb.size.y &&
                             a.position.y + a.size.y > pb.position.y);
            if (overlapP) {
                player->takeDamage(it->getDamage());
                hit = true;
            }
        }

        if (!hit && !it->isHostile()) {
            for (auto &enemy: enemies) {
                const sf::FloatRect b = enemy->getBounds();
                bool overlap = (a.position.x < b.position.x + b.size.x &&
                                a.position.x + a.size.x > b.position.x &&
                                a.position.y < b.position.y + b.size.y &&
                                a.position.y + a.size.y > b.position.y);
                if (overlap) {
                    enemy->damageTaken(it->getDamage());
                    hit = true;
                    break;
                }
            }
        }

        if (hit || it->isDead()) {
            it = projectiles.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = enemies.begin(); it != enemies.end();) {
        if ((*it)->isDead()) {
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}

void GameState::draw() {
    game.getWindow().setView(view);
    game.getWindow().draw(*background);

    for (const auto &enemy: enemies)
        enemy->draw(game.getWindow());
    for (const auto &proj: projectiles)
        proj.draw(game.getWindow());

    if (player)
        player->draw(game.getWindow());

    if (hud)
        hud->draw(game.getWindow());
}
