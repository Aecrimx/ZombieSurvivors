#ifndef OOP_BAT_H
#define OOP_BAT_H

#include "Enemy.h"
#include <cmath>
#include <ostream>

class Bat : public Enemy {
public:
    Bat(sf::Texture &texture, sf::Vector2f startPos, int playerLevel = 0);
    Bat(const Bat &other);
    Bat &operator=(const Bat &other);
    Bat(Bat &&other) noexcept;
    Bat &operator=(Bat &&other) noexcept;

    friend void swap(Bat &first, Bat &second) noexcept;

    Enemy *clone() const override;

    void update(float dt, sf::Vector2f playerPos) override;

    friend std::ostream &operator<<(std::ostream &os, const Bat &obj);
};

#endif // OOP_BAT_H
