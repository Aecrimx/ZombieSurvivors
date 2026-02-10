#include "Bat.h"

Bat::Bat(sf::Texture &texture, sf::Vector2f startPos, int playerLevel)
    : Enemy(texture, 10.f + (5.f * static_cast<float>(playerLevel)), 5.f,
            135.f) {
    sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
    sprite.setPosition(startPos);
    sprite.setScale({1.f, 1.f});
}

Bat::Bat(const Bat &other) : Enemy(other) {
}

Bat &Bat::operator=(const Bat &other) {
    if (this != &other) {
        Enemy::operator=(other);
    }
    return *this;
}

Bat::Bat(Bat &&other) noexcept = default;

Bat &Bat::operator=(Bat &&other) noexcept = default;

void swap(Bat &first, Bat &second) noexcept {
    using std::swap;
    swap(static_cast<Enemy &>(first), static_cast<Enemy &>(second));
}

Enemy *Bat::clone() const { return new Bat(*this); }

void Bat::update(float dt, sf::Vector2f playerPos) {
    sf::Vector2f dir = playerPos - sprite.getPosition();
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (length > 0) {
        dir /= length;
        sprite.move(dir * speed * dt);
        if (dir.x < 0)
            sprite.setScale({-1.f, 1.f});
        else
            sprite.setScale({1.f, 1.f});
    }
}

std::ostream &operator<<(std::ostream &os, const Bat &obj) {
    return os << static_cast<const Enemy &>(obj);
}
