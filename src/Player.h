
#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H

#include <ostream>

#include "ResourceManager.h"
#include "Weapon.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

struct CharacterData {
  std::string name;
  std::string texture_name;
  float moveSpeed;
  float maxHealth;
  std::string startingWeapon;
  std::string Weapon_sprite;

  CharacterData(const std::string &n, const std::string &tex, float speed,
                float hp, const std::string &weapon, const std::string &wSprite)
      : name(n), texture_name(tex), moveSpeed(speed), maxHealth(hp),
        startingWeapon(weapon), Weapon_sprite(wSprite) {}
};

class Player {
  sf::Sprite sprite;
  std::vector<Weapon *> weapons;

  float speed;
  float maxHealth;
  float currentHealth;

public:
  Player(const CharacterData &data, ResourceManager &resources);

  ~Player();

  Player(const Player &other);
  Player &operator=(const Player &other);

  // void handleInput();
  void update(float dt, const std::vector<std::unique_ptr<Enemy>> &enemies,
              std::vector<Projectile> &projectiles);
  void draw(sf::RenderWindow &window) const;

  void addWeapon(const Weapon &weapon);

  sf::Vector2f getPos() const;

friend std::ostream & operator<<(std::ostream &os, const Player &obj) {
    return os
           << "speed: " << obj.speed
           << " maxHealth: " << obj.maxHealth
           << " currentHealth: " << obj.currentHealth;
  }
};

#endif // OOP_PLAYER_H