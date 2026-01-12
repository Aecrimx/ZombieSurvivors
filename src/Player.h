#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H

#include <ostream>

#include "ResourceManager.h"
#include "Weapon.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// Forward declaration for Item class
class Item;

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
  std::vector<Item *> items;

  float speed;
  float baseSpeed; // Store base speed for item calculations
  float maxHealth;
  float currentHealth;
  float healthRegen; // HP per second

  // Leveling system
  int level;
  float currentXP;
  float xpToNextLevel;

  // Facing direction for directional weapons
  bool facingRight;

  // Item effects (applied from items)
  float damageReduction;    // 0.0 to 1.0 (e.g., 0.3 = 30% reduction)
  float cooldownMultiplier; // 0.5 = 50% CDR

public:
  Player(const CharacterData &data, ResourceManager &resources);

  ~Player();

  Player(const Player &other);

  Player &operator=(const Player &other);

  // void handleInput();
  void update(float dt, const sf::RenderWindow &window,
              const std::vector<std::unique_ptr<Enemy>> &enemies,
              std::vector<Projectile> &projectiles);

  void draw(sf::RenderWindow &window) const;

  // Weapon and item management
  void addWeapon(const Weapon &weapon);
  void addItem(Item *item);
  const std::vector<Weapon *> &getWeapons() const { return weapons; }
  const std::vector<Item *> &getItems() const { return items; }

  // Leveling
  void addXP(float amount);
  bool shouldLevelUp() const { return currentXP >= xpToNextLevel; }
  void levelUp();
  int getLevel() const { return level; }
  float getCurrentXP() const { return currentXP; }
  float getXPToNextLevel() const { return xpToNextLevel; }

  // Health
  void heal(float amount);
  void takeDamage(float amount);
  bool isDead() const { return currentHealth <= 0; }
  float getMaxHealth() const { return maxHealth; }
  void increaseMaxHealth(float amount) {
    maxHealth += amount;
    currentHealth += amount;
  }
  void setHealthRegen(float regen) { healthRegen = regen; }
  float getHealthRegen() const { return healthRegen; }

  // Item effects
  void setDamageReduction(float reduction) { damageReduction = reduction; }
  void setCooldownMultiplier(float mult) { cooldownMultiplier = mult; }
  float getDamageReduction() const { return damageReduction; }
  float getCooldownMultiplier() const { return cooldownMultiplier; }

  // Movement
  void increaseSpeed(float multiplier) { speed = baseSpeed * multiplier; }
  float getSpeed() const { return speed; }

  // Position and collision
  sf::Vector2f getPos() const;
  sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }

  // Direction
  bool isFacingRight() const { return facingRight; }

  friend std::ostream &operator<<(std::ostream &os, const Player &obj) {
    return os << "speed: " << obj.speed << " maxHealth: " << obj.maxHealth
              << " currentHealth: " << obj.currentHealth
              << " level: " << obj.level;
  }

  float getHealthPoints() const { return currentHealth / maxHealth; }
};

#endif // OOP_PLAYER_H
