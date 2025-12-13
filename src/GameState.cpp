#include "GameState.h"
#include "Game.h"
#include <iostream>

GameState::GameState(Game& ref) : State(ref) {
    game.getResourceManager().loadTexture("player", "assets/wizard.png");
    game.getResourceManager().loadTexture("background", "assets/grass.png");

    player = std::make_unique<sf::Sprite>(game.getResourceManager().getTexture("player"));
    player->setScale({0.2f, 0.2f}); // la fel si aici?? probabil tot in Player class
    sf::Vector2u size = game.getResourceManager().getTexture("player").getSize();
    player->setOrigin({size.x/2.f, size.y/2.f});

    playerPos = {100.f, 100.f};
    player->setPosition(playerPos);

    // Use the loaded background texture consistently
    game.getResourceManager().getTexture("background").setRepeated(true);
    background = std::make_unique<sf::Sprite>(game.getResourceManager().getTexture("background"));
    background->setTextureRect(sf::IntRect({0, 0}, {10000, 10000}));

    view.setSize({game.getWindowSize().x, game.getWindowSize().y});
    view.setCenter(playerPos);
}

void GameState::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) { // idk yet unde sa o tin
        game.getWindow().close();
    }
}

void GameState::update(float dt) {
    const float speed = 125.f;

    sf::Vector2f velocity(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) velocity.y -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) velocity.y += speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) velocity.x -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) velocity.x += speed * dt;

    playerPos += velocity;
    player->setPosition(playerPos);

    view.setCenter(playerPos);

    std::cout << "dt: " << dt << "Pozitie: " << playerPos.x  << ' ' << playerPos.y<< '\n';
}

void GameState::draw(float dt) {
    game.getWindow().setView(view);
    game.getWindow().draw(*background);
    game.getWindow().draw(*player);
}



