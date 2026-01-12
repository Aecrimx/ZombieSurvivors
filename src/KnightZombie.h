#ifndef OOP_KNIGHTZOMBIE_H
#define OOP_KNIGHTZOMBIE_H

#include "Enemy.h"
#include <cmath>
#include <ostream>

class KnightZombie : public Enemy {
public:
    KnightZombie(sf::Texture &texture, sf::Vector2f startPos, int playerLevel = 0)
        : Enemy(texture, 50.f + (5.f * playerLevel), 25.f, 80.f) {
        sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
        sprite.setPosition(startPos);
        sprite.setScale({0.25f, 0.25f});
    }

    void update(float dt, sf::Vector2f playerPos) override {
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

    friend std::ostream &operator<<(std::ostream &os, const KnightZombie &obj) {
        return os << static_cast<const Enemy &>(obj);
    }
};

#endif // OOP_KNIGHTZOMBIE_H
