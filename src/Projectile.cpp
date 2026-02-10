//
// Created by Mircea on 15.12.2025.
//

#include "Projectile.h"

Projectile::Projectile(const sf::Texture &texture, sf::Vector2f pos,
                       sf::Vector2f dir, float speed, float dmg, bool isHostile)
    : sprite(texture), velocity(0, 0), damage(dmg), life_time(3.0f),
      hostile(isHostile) {
    // ori scale manual de sprite ori scale dat de img size momentan drc
    sprite.setScale({3.5f, 3.5f});

    sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});

    sprite.setPosition(pos);
    velocity = dir * speed;

    float angleDeg = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
    sprite.setRotation(sf::degrees(angleDeg));
}

Projectile::Projectile(const Projectile &other)
    : sprite(other.sprite), velocity(other.velocity), damage(other.damage),
      life_time(other.life_time), hostile(other.hostile) {
}

Projectile &Projectile::operator=(const Projectile &other) {
    if (this != &other) {
        sprite = other.sprite;
        velocity = other.velocity;
        damage = other.damage;
        life_time = other.life_time;
        hostile = other.hostile;
    }
    return *this;
}

void Projectile::update(float dt) {
    sprite.move(velocity * dt);
    life_time -= dt;
}

void Projectile::draw(sf::RenderWindow &window) const { window.draw(sprite); }

bool Projectile::isDead() const { return life_time <= 0.f; }

sf::FloatRect Projectile::getBounds() const { return sprite.getGlobalBounds(); }

float Projectile::getDamage() const { return damage; }

bool Projectile::isHostile() const { return hostile; }

std::ostream &operator<<(std::ostream &os, const Projectile &obj) {
    obj.print(os);
    return os;
}
