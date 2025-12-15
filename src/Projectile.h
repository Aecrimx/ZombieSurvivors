#ifndef OOP_PROJECTILE_H
#define OOP_PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <ostream>

class Projectile {
  sf::Sprite sprite;
  sf::Vector2f velocity;
  float damage;
  float life_time;

public:
  Projectile(const sf::Texture &texture, sf::Vector2f pos, sf::Vector2f dir,
             float speed, float dmg)
      : sprite(texture), velocity(0, 0), damage(dmg), life_time(3.0f) {

    // ori scale manual de sprite ori scale dat de img size momentan drc
    sprite.setScale({3.5f, 3.5f});

    sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});

    sprite.setPosition(pos);
    velocity = dir * speed;

    float angleDeg = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
    sprite.setRotation(sf::degrees(angleDeg));
  }

  void update(float dt) {
    sprite.move(velocity * dt);
    life_time -= dt;
  }

  void draw(sf::RenderWindow &window) const { window.draw(sprite); }

  bool isDead() const { return life_time <= 0.f; }
  sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
  float getDamage() const { return damage; }
  // void Destroy() {life_time = -1.f;}

friend std::ostream & operator<<(std::ostream &os, const Projectile &obj) {
    return os
           << "velocity: " << obj.velocity.x << ", " << obj.velocity.y
           << " damage: " << obj.damage
           << " life_time: " << obj.life_time;
  }
};

#endif // OOP_PROJECTILE_H