#include "GameState.h"
#include "Game.h"
#include <iostream>

GameState::GameState(Game& ref) : State(ref) {
    ResourceManager& res = game.getResourceManager();

    // //game.getResourceManager().loadTexture("player", "assets/wizard.png");
    // player = std::make_unique<sf::Sprite>(game.getResourceManager().getTexture("player"));
    // player->setScale({0.2f, 0.2f}); // la fel si aici?? probabil tot in Player class
    // sf::Vector2u size = game.getResourceManager().getTexture("player").getSize();
    // player->setOrigin({size.x/2.f, size.y/2.f});
    //
    // playerPos = {5000.f, 5000.f};
    // player->setPosition(playerPos);

    CharacterData Hocus; // later dat din Meniu

    res.loadTexture("player", "assets/wizard.png");

    Hocus.name = "Hocus";
    Hocus.texture_name = "player";
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
    view.setCenter(playerPos);

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
    if (player) {
        player->update(dt);
        view.setCenter(player->getPos());
    }
    //dddfdsfdssssfawaffffffdfffffwaffstd::cout << "dt: " << dt << "Pozitie: " << playerPos.x  << ' ' << playerPos.y<< '\n';
}

void GameState::draw() {
    game.getWindow().setView(view);
    game.getWindow().draw(*background);

    if (player) player->draw(game.getWindow());
}



