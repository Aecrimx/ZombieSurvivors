//
// Created by Mircea on 13.12.2025.
//

#include "Weapon.h"

Weapon::Weapon(const std::string &name, float cooldown, ResourceManager &res,
               float baseDmg)
    : name(name), cooldown(cooldown), timer(0.f), resources(res), level(1),
      damage(baseDmg), baseDamage(baseDmg) {}

Weapon::~Weapon() = default;

Weapon *Weapon::copy() const { return clone(); }

void Weapon::draw(sf::RenderWindow &window) const {}

const std::string& Weapon::getName() const { return name; }

void Weapon::levelUp() {
  if (level < 3) {
    level++;
    damage = baseDamage * (1.0f + (level - 1) * 0.2f); // 20% mai mult damage
    cooldown = cooldown * 0.9f; // 10% reducere cooldown
  }
}

bool Weapon::canLevelUp() const { return level < 3; }

int Weapon::getLevel() const { return level; }

float Weapon::getDamage() const { return damage; }

std::ostream &operator<<(std::ostream &os, const Weapon &obj) {
  obj.print(os);
  return os;
}
