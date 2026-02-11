#include "KnightZombie.h"

KnightZombie::KnightZombie(const sf::Texture &texture, const sf::Vector2f startPos,
                           const int playerLevel)
    : Enemy(texture, 35.f + (5.f * playerLevel), 25.f, 80.f) {
    sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
    sprite.setPosition(startPos);
    sprite.setScale({0.25f, 0.25f});
}

Enemy *KnightZombie::clone() const { return new KnightZombie(*this); }

void KnightZombie::update(const float dt, const sf::Vector2f playerPos) {
    sf::Vector2f dir = playerPos - sprite.getPosition();
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (length > 0) {
        dir /= length;
        sprite.move(dir * speed * dt);

        if (dir.x < 0)
            sprite.setScale({-0.25f, 0.25f});
        else
            sprite.setScale({0.25f, 0.25f});
    }
}

std::ostream &operator<<(std::ostream &os, const KnightZombie &obj) {
    return os << static_cast<const Enemy &>(obj);
}
