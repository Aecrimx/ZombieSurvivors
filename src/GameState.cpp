#include "GameState.h"
#include "BigZombieBoss.h"
#include "Game.h"
#include "GameOverState.h"
#include "LevelUpState.h"
#include "PauseState.h"
#include "SaveManager.h"
#include <cmath>
#include <iostream>

#include "Flying_Skull.h"
#include "PlayerException.h"

GameState::GameState(Game &gameRef, const CharacterData &charData)
    : State(gameRef), gameTimer(0.f), characterName(charData.name),
      bossSpawned(false), contactDamageCooldown(0.f) {
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

    enemyFactory = std::make_unique<EnemyFactory>(res, *player, projectiles);

    EnemySpawnManager::getInstance().reset();

    // Wave 1 : 0 - 3 min bats si zombies 50/50
    EnemySpawnManager::getInstance().addRule(EnemyType::Bat, 1, 0.f, 180.f);
    EnemySpawnManager::getInstance().addRule(EnemyType::Zombie, 1, 0.f, 180.f);

    // Wave 2 : 3 - 7 min Doar zombies
    EnemySpawnManager::getInstance().addRule(EnemyType::Zombie, 1, 180.f, 420.f);

    // Wave 3 : 7 - 10 min Zombies + Knight Zmbies + 1/0 sa fie flying skull
    EnemySpawnManager::getInstance().addRule(EnemyType::FlyingSkull, 1, 420.f, 600.f);
    EnemySpawnManager::getInstance().addRule(EnemyType::KnightZombie, 4, 420.f, 600.f);
    EnemySpawnManager::getInstance().addRule(EnemyType::Zombie, 5, 420.f, 600.f);

    // Wave 10 - 15 min Flying Skull Knight Zombies
    EnemySpawnManager::getInstance().addRule(EnemyType::FlyingSkull, 1, 600.f);
    EnemySpawnManager::getInstance().addRule(EnemyType::KnightZombie, 2, 600.f);
    // EnemySpawnManager::getInstance().addRule(EnemyType::Zombie, 1, 600.f);

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

    if (!bossSpawned && EnemySpawnManager::shouldSpawnBoss(gameTimer)) {
        bossSpawned = true;
        const float angle = (rand() % 360) * 3.14f / 180.f;
        const float dist = 500.f;
        sf::Vector2f spawnPos =
                player->getPos() +
                sf::Vector2f(std::cos(angle) * dist, std::sin(angle) * dist);

        auto boss = enemyFactory->createEnemy(EnemyType::BigZombieBoss, spawnPos);
        if (boss) {
            enemies.push_back(std::move(boss));
        }
    }

    spawnTimer += dt;
    float spawnInterval = 2.5f;

    if (spawnTimer > spawnInterval) {
        spawnTimer = 0;
        const float angle = (rand() % 360) * 3.14f / 180.f;
        const float dist = 425.f;
        sf::Vector2f spawnPos =
                player->getPos() +
                sf::Vector2f(std::cos(angle) * dist, std::sin(angle) * dist);

        EnemyType type = EnemySpawnManager::getInstance().pickEnemy(gameTimer);
        if (type != EnemyType::None) {
            auto enemy = enemyFactory->createEnemy(type, spawnPos);
            if (enemy) {
                enemies.push_back(std::move(enemy));
            }
        }
    }

    player->update(dt, game.getWindow(), enemies, projectiles);

    //Map Border
    sf::Vector2f pos = player->getPos();
    float clampedX = std::max(0.f, std::min(pos.x, 10000.f));
    float clampedY = std::max(0.f, std::min(pos.y, 10000.f));
    player->setPos({clampedX, clampedY});

    //Validare coords
    if (player->getPos().x < 0 || player->getPos().x > 10000 ||
        player->getPos().y < 0 || player->getPos().y > 10000) {
        throw PlayerException("Player coordinates outside world limits.");
    }

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
