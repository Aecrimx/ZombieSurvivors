//
// Created by Mircea on 15.12.2025.
//

#include "Zombie.h"

Zombie::Zombie(sf::Texture &texture, sf::Vector2f startPos)
    : Enemy(texture, 20.f, 10.f, 125.f) {
  sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
  sprite.setPosition(startPos);
  sprite.setScale({0.2f, 0.2f});
}

Zombie::Zombie(const Zombie &other) : Enemy(other) {}

Zombie &Zombie::operator=(const Zombie &other) {
  if (this != &other) {
    Enemy::operator=(other);
  }
  return *this;
}

Enemy *Zombie::clone() const { return new Zombie(*this); }

void Zombie::update(float dt, sf::Vector2f playerPos) {
  sf::Vector2f dir = playerPos - sprite.getPosition();
  float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

  if (length > 0) {
    dir /= length;
    sprite.move(dir * speed * dt);

    if (dir.x < 0)
      sprite.setScale({-0.2f, 0.2f});
    else
      sprite.setScale({0.2f, 0.2f});
  }
}

std::ostream &operator<<(std::ostream &os, const Zombie &obj) {
  return os << static_cast<const Enemy &>(obj);
}
