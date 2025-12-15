#ifndef OOP_FIREWAND_H
#define OOP_FIREWAND_H

#include "Weapon.h"
#include <cmath>
#include <iostream>
#include <ostream>

class FireWand : public Weapon {
public:
  explicit FireWand(ResourceManager &res) : Weapon("Fire Wand", 5.0f, res) {}

  Weapon *clone() const override { return new FireWand(*this); }

  void update(float dt, sf::Vector2f playerPos,
              const std::vector<std::unique_ptr<Enemy>> &enemies,
              std::vector<Projectile> &projectiles) override {
    if (timer > 0)
      timer -= dt;
    if (timer <= 0 && !enemies.empty()) {
      const Enemy *nearest = nullptr;
      float minDistSq = 999999999.f;

      for (const auto &enemy : enemies) {
        sf::Vector2f diff = enemy->getPos() - playerPos;
        float distSq = diff.x * diff.x + diff.y * diff.y;
        if (distSq < minDistSq) {
          minDistSq = distSq;
          nearest = enemy.get();
        }
      }

      if (nearest) {
        sf::Vector2f dir = nearest->getPos() - playerPos;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0)
          dir /= len;

        sf::Texture &tex = resources.getTexture("fireball");
        projectiles.emplace_back(tex, playerPos, dir, 150.f, 10.f);
        //mutat dmg in proprietatile clasei pls
        timer = cooldown;
      }
    }
  }

  friend std::ostream & operator<<(std::ostream &os, const FireWand &obj) {
    os << "[FireWand]\n";
    os << "Cooldown: " << obj.cooldown << '\n' << "Name: " << obj.name << '\n';
    return os;
  }
};

#endif // OOP_FIREWAND_H