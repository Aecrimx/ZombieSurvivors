#include "GameState.h"
#include "Bat.h"
#include "BigZombieBoss.h"
#include "Game.h"
#include "GameOverState.h"
#include "KnightZombie.h"
#include "LevelUpState.h"
#include "PauseState.h"
#include "SaveManager.h"
#include "Zombie.h"
#include <cmath>
#include <iostream>

#include "Flying_Skull.h"

GameState::GameState(Game &ref) : State(ref) {
    // this is kinda deprecated code
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
    res.loadTexture("knife", "assets/knife128x128.png");
    res.loadTexture("knife_projectile", "assets/knife_projectile32x16.png");
    res.loadTexture("background", "assets/grass.png");
    res.getTexture("background").setRepeated(true);

    CharacterData Azoth = CharacterData("Azoth", "demon", 150.f, 100.f,
                                        "Demonic Book", "demonic_book");

    player = std::make_unique<Player>(Azoth, res);
    characterName = "demon";

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

GameState::GameState(Game &gameRef, const CharacterData &charData)
    : State(gameRef), gameTimer(0.f), characterName(charData.name),
      bossSpawned(false), contactDamageCooldown(0.f), escWasPressed(false) {
    ResourceManager &res = game.getResourceManager();

    res.loadTexture("fire_wand", "assets/fire_wand.png");
    res.loadTexture("zombie", "assets/zombie.png");
    res.loadTexture("bat", "assets/bat64x64.png");
    res.loadTexture("knight_zombie", "assets/knight_zombie512x567.png");
    res.loadTexture("boss", "assets/le_big_boss.png");
    res.loadTexture("fireball", "assets/fireball.png");
    res.loadTexture("wizard", "assets/wizard.png");
    res.loadTexture("spinning_skull", "assets/spinning_skull_sheet.png");
    res.loadTexture("slot", "assets/inventory_slot.png");
    res.loadTexture("demon", "assets/demon.png");
    res.loadTexture("damage_aura", "assets/damage_aura_demonic_book64x64.png");
    res.loadTexture("demonic_book", "assets/demonic_book32x32.png");
    res.loadTexture("laser_projectile", "assets/laser_projectile512x16.png");
    res.loadTexture("skull", "assets/skull_projectile.png");
    res.loadTexture("dumbbell", "assets/dumbbell.png");
    res.loadTexture("bone", "assets/bone128x128.png");
    res.loadTexture("knife", "assets/knife128x128.png");
    res.loadTexture("knife_projectile", "assets/knife_projectile32x16.png");
    res.loadTexture("background", "assets/grass.png");
    res.loadTexture("soul_Scream", "assets/soul_Scream32x64.png");

    res.getTexture("background").setRepeated(true);

    res.loadTexture("experience", "assets/experience_star128x128.png");
    res.loadTexture("health", "assets/health_pickup128x128.png");

    player = std::make_unique<Player>(charData, res);
    characterName = charData.texture_name;

    background = std::make_unique<sf::Sprite>(
        game.getResourceManager().getTexture("background"));
    background->setTextureRect(sf::IntRect({0, 0}, {10000, 10000}));
    view.setSize({game.getWindowSize().x, game.getWindowSize().y});
    view.setCenter(player->getPos());

    const sf::Vector2f winSize = game.getWindowSize();
    GameState::Resize(static_cast<int>(winSize.x), static_cast<int>(winSize.y));

    sf::Vector2u size = game.getWindow().getSize();
    hud = std::make_unique<HUD>(res, size.x, size.y);

    GameState::Resize(static_cast<int>(size.x), static_cast<int>(size.y));
}

void GameState::Resize(const int w, const int h) {
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

// std::vector<Projectile> &GameState::getProjectiles() { return projectiles; }

std::ostream &operator<<(std::ostream &os, const GameState &) {
    return os << "[GameState]" << '\n';
}

void GameState::handleInput() {
}

void GameState::handleEvent(const sf::Event &event) {
    if (const auto *keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            std::cout << "Esc pressed" << std::endl;
            game.pushState(std::make_unique<PauseState>(game, this));
        }
    }
}

void GameState::update(const float dt) {
    if (!player) {
        return;
    }

    gameTimer += dt;

    if (player->shouldLevelUp()) {
        player->levelUp();
        game.pushState(std::make_unique<LevelUpState>(game, player.get(), this));
        return;
    }

    if (player->isDead()) {
        int score = player->getLevel() * 100;
        game.scheduleReplace(std::make_unique<GameOverState>(
            game, score, characterName, false,
            nullptr)); // inainte nu pasam nullptr which fucked everything up
        return;
    }

    if (!bossSpawned && gameTimer >= 900.f) {
        // 900s = 15 min
        bossSpawned = true;
        const float angle = (rand() % 360) * 3.14f / 180.f;
        const float dist = 500.f;
        sf::Vector2f spawnPos =
                player->getPos() +
                sf::Vector2f(std::cos(angle) * dist, std::sin(angle) * dist);

        enemies.push_back(std::make_unique<BigZombieBoss>(
            game.getResourceManager().getTexture("boss"), spawnPos, projectiles,
            game.getResourceManager(), player->getLevel()));
    }

    spawnTimer += dt;
    float spawnInterval = 3.5f;

    if (spawnTimer > spawnInterval) {
        spawnTimer = 0;
        const float angle = (rand() % 360) * 3.14f / 180.f;
        const float dist = 400.f;
        sf::Vector2f spawnPos =
                player->getPos() +
                sf::Vector2f(std::cos(angle) * dist, std::sin(angle) * dist);

        if (gameTimer < 180.f) {
            // mix de zombie si bats
            if (rand() % 2 == 0) {
                enemies.push_back(
                    std::make_unique<Bat>(game.getResourceManager().getTexture("bat"),
                                          spawnPos, player->getLevel()));
            } else {
                enemies.push_back(std::make_unique<Zombie>(
                    game.getResourceManager().getTexture("zombie"), spawnPos));
            }
        } else if (gameTimer < 420.f) {
            // doar zombii
            enemies.push_back(std::make_unique<Zombie>(
                game.getResourceManager().getTexture("zombie"), spawnPos));
        } else if (gameTimer < 600.f) {
            // si knight zombies
            int enemyType = rand() % 3;
            if (enemyType == 0) {
                enemies.push_back(std::make_unique<KnightZombie>(
                    game.getResourceManager().getTexture("knight_zombie"), spawnPos,
                    player->getLevel()));
            } else {
                enemies.push_back(std::make_unique<Zombie>(
                    game.getResourceManager().getTexture("zombie"), spawnPos));
            }
        } else {
            // acm si flying skulls
            int enemyType = rand() % 4;
            if (enemyType == 0) {
                enemies.push_back(std::make_unique<Flying_Skull>(
                    game.getResourceManager().getTexture("spinning_skull"), spawnPos,
                    projectiles, game.getResourceManager()));
            } else if (enemyType == 1) {
                enemies.push_back(std::make_unique<KnightZombie>(
                    game.getResourceManager().getTexture("knight_zombie"), spawnPos,
                    player->getLevel()));
            } else {
                enemies.push_back(std::make_unique<Zombie>(
                    game.getResourceManager().getTexture("zombie"), spawnPos));
            }
        }
    }

    player->update(dt, game.getWindow(), enemies, projectiles);
    view.setCenter(player->getPos());

    for (const auto &enemy: enemies) {
        enemy->update(dt, player->getPos());
    }

    for (const auto &pickup: pickups) {
        pickup->update(dt, player->getPos());
    }

    const sf::FloatRect playerBounds = player->getBounds();
    for (auto it = pickups.begin(); it != pickups.end();) {
        const sf::FloatRect pb = (*it)->getBounds();
        const bool collides =
        (playerBounds.position.x < pb.position.x + pb.size.x &&
         playerBounds.position.x + playerBounds.size.x > pb.position.x &&
         playerBounds.position.y < pb.position.y + pb.size.y &&
         playerBounds.position.y + playerBounds.size.y > pb.position.y);
        if (collides) {
            (*it)->onCollect(*player);
            it = pickups.erase(it);
        } else {
            ++it;
        }
    }

    if (contactDamageCooldown > 0.f) {
        contactDamageCooldown -= dt;
    }

    for (const auto &enemy: enemies) {
        if (contactDamageCooldown <= 0.f) {
            const sf::FloatRect enemyBounds = enemy->getBounds();
            const bool collides = (playerBounds.position.x <
                                   enemyBounds.position.x + enemyBounds.size.x &&
                                   playerBounds.position.x + playerBounds.size.x >
                                   enemyBounds.position.x &&
                                   playerBounds.position.y <
                                   enemyBounds.position.y + enemyBounds.size.y &&
                                   playerBounds.position.y + playerBounds.size.y >
                                   enemyBounds.position.y);
            if (collides) {
                player->takeDamage(enemy->getDamage());
                contactDamageCooldown = 1.0f;
                break;
            }
        }
    }

    for (auto it = projectiles.begin(); it != projectiles.end();) {
        it->update(dt);

        bool hit = false;
        const sf::FloatRect a = it->getBounds();

        if (it->isHostile() && player) {
            const sf::FloatRect pb = player->getBounds();
            const bool overlapP = (a.position.x < pb.position.x + pb.size.x &&
                                   a.position.x + a.size.x > pb.position.x &&
                                   a.position.y < pb.position.y + pb.size.y &&
                                   a.position.y + a.size.y > pb.position.y);
            if (overlapP) {
                player->takeDamage(it->getDamage());
                hit = true;
            }
        }

        if (!hit && !it->isHostile()) {
            for (const auto &enemy: enemies) {
                const sf::FloatRect b = enemy->getBounds();
                const bool overlap = (a.position.x < b.position.x + b.size.x &&
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

    SaveManager &saveManager = SaveManager::getInstance();
    for (auto it = enemies.begin(); it != enemies.end();) {
        if ((*it)->isDead()) {
            sf::Vector2f deathPos = (*it)->getPos();

            if (dynamic_cast<Flying_Skull *>(it->get())) {
                saveManager.incrementFlyingSkullKills();
            }

            if (dynamic_cast<BigZombieBoss *>(it->get())) {
                int score = player->getLevel() * 100;
                game.scheduleReplace(std::make_unique<GameOverState>(
                    game, score, characterName, true, nullptr)); // la fel ca mai sus
                return;
            }

            if (const int dropChance = rand() % 100; dropChance < 90) {
                pickups.push_back(std::make_unique<ExperienceStar>(
                    game.getResourceManager().getTexture("experience"), deathPos));
            } else {
                pickups.push_back(std::make_unique<HealthPickup>(
                    game.getResourceManager().getTexture("health"), deathPos));
            }
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }

    if (hud && player) {
        hud->update(*player, gameTimer);
    }

}

void GameState::draw() {
    game.getWindow().setView(view);
    game.getWindow().draw(*background);

    for (const auto &enemy: enemies)
        enemy->draw(game.getWindow());
    for (const auto &proj: projectiles)
        proj.draw(game.getWindow());

    for (const auto &pickup: pickups)
        pickup->draw(game.getWindow());

    if (player)
        player->draw(game.getWindow());

    if (hud && player)
        hud->draw(game.getWindow(), *player);
}
