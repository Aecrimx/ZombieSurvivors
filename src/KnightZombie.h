#ifndef OOP_KNIGHTZOMBIE_H
#define OOP_KNIGHTZOMBIE_H

#include "Enemy.h"
#include <cmath>
#include <ostream>

class KnightZombie : public Enemy {
public:
    KnightZombie(const sf::Texture &texture, sf::Vector2f startPos,
                 int playerLevel = 0);

    Enemy *clone() const override;

    void update(float dt, sf::Vector2f playerPos) override;

    friend std::ostream &operator<<(std::ostream &os, const KnightZombie &obj);
};

#endif // OOP_KNIGHTZOMBIE_H
