#ifndef OOP_BAT_H
#define OOP_BAT_H

#include "Enemy.h"
#include <cmath>
#include <ostream>

class Bat : public Enemy {
public:
    Bat(sf::Texture &texture, sf::Vector2f startPos, int playerLevel = 0)
        : Enemy(texture, 10.f + (5.f * playerLevel), 5.f, 135.f) {
        sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
        sprite.setPosition(startPos);
        sprite.setScale({1.f, 1.f});
    }

    void update(float dt, sf::Vector2f playerPos) override {
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

    friend std::ostream &operator<<(std::ostream &os, const Bat &obj) {
        return os << static_cast<const Enemy &>(obj);
    }
};

#endif // OOP_BAT_H
