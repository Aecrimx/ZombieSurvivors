#ifndef OOP_ZOMBIE_H
#define OOP_ZOMBIE_H

#include "Enemy.h"
#include <cmath>
#include <ostream>

class Zombie : public Enemy {
public:
    Zombie(sf::Texture &texture, sf::Vector2f startPos)
        : Enemy(texture, 20.f, 100.f, 125.f) {
        sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
        sprite.setPosition(startPos);
        sprite.setScale({0.2f, 0.2f});
    }

    void update(float dt, sf::Vector2f playerPos) override {
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

    friend std::ostream &operator<<(std::ostream &os, const Zombie &obj) {
        return os << static_cast<const Enemy &>(obj);
    }
};

#endif // OOP_ZOMBIE_H
