
#ifndef OOP_GAMESTATE_H
#define OOP_GAMESTATE_H

#include "Enemy.h"
#include "Game.h"
#include "Player.h"
#include "Projectile.h"
#include "State.h"

#include <memory>
#include <ostream>
#include <vector>

class GameState : public State {

  std::unique_ptr<Player> player;

  std::unique_ptr<sf::Sprite> background;
  sf::View view;
  // maybe view de GUI si aici?
  float spawnTimer{0.f};
  std::vector<std::unique_ptr<Enemy>> enemies;
  std::vector<Projectile> projectiles;

public:
  explicit GameState(Game &game);

  void handleInput() override;
  void update(float dt) override;
  void draw() override;
  void Resize(int w, int h) override;

  friend std::ostream & operator<<(std::ostream &os, const GameState & /*obj*/) {
    return os << "[GameState]" << '\n';
  }

};

#endif // OOP_GAMESTATE_H