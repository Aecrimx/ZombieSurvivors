#ifndef OOP_ZOMBIE_H
#define OOP_ZOMBIE_H

#include "Enemy.h"
#include <cmath>
#include <ostream>

class Zombie : public Enemy {
public:
    Zombie(sf::Texture &texture, sf::Vector2f startPos);

    Zombie(const Zombie &other);
    Zombie &operator=(const Zombie &other);

    Zombie(Zombie &&other) noexcept = default;
    Zombie &operator=(Zombie &&other) noexcept = default;

    friend void swap(Zombie &first, Zombie &second) noexcept {
        using std::swap;
        swap(static_cast<Enemy &>(first), static_cast<Enemy &>(second));
    }

    Enemy *clone() const override;

    void update(float dt, sf::Vector2f playerPos) override;

    friend std::ostream &operator<<(std::ostream &os, const Zombie &obj);
};

#endif // OOP_ZOMBIE_H
