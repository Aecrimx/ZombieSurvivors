#ifndef OOP_GAMESTATE_H
#define OOP_GAMESTATE_H

#include "Enemy.h"
#include "ExperienceStar.h"
#include "Game.h"
#include "HUD.h"
#include "HealthPickup.h"
#include "PauseState.h"
#include "Pickup.h"
#include "Player.h"
#include "Projectile.h"
#include "State.h"

#include <memory>
#include <ostream>
#include <vector>

class GameState : public State {
    std::unique_ptr<Player> player;
    std::unique_ptr<HUD> hud;

    std::unique_ptr<sf::Sprite> background;
    sf::View view;
    float spawnTimer{0.f};
    float gameTimer{0.f}; // total game time
    std::vector<std::unique_ptr<Enemy> > enemies;
    std::vector<Projectile> projectiles;
    std::vector<std::unique_ptr<Pickup> > pickups;

    std::string characterName;
    bool bossSpawned{false};

    float contactDamageCooldown{0.f};
    bool escWasPressed{false};

public:
    explicit GameState(Game &game);

    GameState(Game &game, const CharacterData &charData);

    void handleInput() override;

    void handleEvent(const sf::Event &event) override;

    void update(float dt) override;

    void draw() override;

    void Resize(int w, int h) override;

    //std::vector<Projectile> &getProjectiles();

    friend std::ostream &operator<<(std::ostream &os, const GameState & /*obj*/);

};

#endif // OOP_GAMESTATE_H
