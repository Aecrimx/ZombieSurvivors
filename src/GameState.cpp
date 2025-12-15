#include "GameState.h"
#include "Game.h"
#include <iostream>
#include <cmath>
#include "Zombie.h"
#include "FireWand.h"

GameState::GameState(Game& ref) : State(ref) {
    ResourceManager& res = game.getResourceManager();
    CharacterData Hocus; // later dat din Meniu

    res.loadTexture("fire_wand", "assets/fire_wand.png");
    res.loadTexture("zombie", "assets/zombie.png");
    res.loadTexture("fireball", "assets/fireball.png");
    res.loadTexture("wizard", "assets/wizard.png");

    Hocus.name = "Hocus";
    Hocus.texture_name = "wizard";
    Hocus.moveSpeed = 125.f;
    Hocus.maxHealth = 100.f;
    Hocus.startingWeapon = "Fire wand";
    Hocus.Weapon_sprite = "fire_wand";

    player = std::make_unique<Player>(Hocus, res);



    //background stuff
    game.getResourceManager().loadTexture("background", "assets/grass.png");
    game.getResourceManager().getTexture("background").setRepeated(true);
    background = std::make_unique<sf::Sprite>(game.getResourceManager().getTexture("background"));
    background->setTextureRect(sf::IntRect({0, 0}, {10000, 10000}));
    view.setSize({game.getWindowSize().x, game.getWindowSize().y});
    view.setCenter(player->getPos());

    sf::Vector2f winSize = game.getWindowSize();
    GameState::Resize((int)winSize.x, (int)winSize.y);

}

void GameState::Resize(int w, int h) {
    view = game.LetterboxView(view, w, h);
}


void GameState::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) { // idk yet unde sa o tin
        game.getWindow().close();
    }
}

void GameState::update(float dt) {
    spawnTimer += dt;
    if (spawnTimer > 1.5f) {
        spawnTimer = 0;
        float angle = (rand() % 360) * 3.14f / 180.f; // random angle
        float dist = 400.f;
        sf::Vector2f spawnPos = player->getPos() + sf::Vector2f(std::cos(angle)*dist, std::sin(angle)*dist);

        enemies.push_back(std::make_unique<Zombie>(game.getResourceManager().getTexture("zombie"), spawnPos));
    }
    if (player) {
        player->update(dt, enemies, projectiles);
        view.setCenter(player->getPos());
    }
    for (auto& enemy : enemies) {
        enemy->update(dt, player->getPos());
    }

    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        it->update(dt);

        bool hit = false;
        for (auto& enemy : enemies) {
            const sf::FloatRect a = it->getBounds();
            const sf::FloatRect b = enemy->getBounds();
            bool overlap = (a.position.x < b.position.x + b.size.x && a.position.x + a.size.x > b.position.x &&
                            a.position.y < b.position.y + b.size.y && a.position.y + a.size.y > b.position.y);
            if (overlap) {
                enemy->damageTaken(it->getDamage());
                hit = true;
                break;
            }
        }
        if (hit || it->isDead()) {
            it = projectiles.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = enemies.begin(); it != enemies.end(); ) {
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

    for (const auto& enemy : enemies) enemy->draw(game.getWindow());
    for (auto& proj : projectiles) proj.draw(game.getWindow());

    if (player) player->draw(game.getWindow());
}



