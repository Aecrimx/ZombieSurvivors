#ifndef OOP_BIGZOMBIEBOSS_H
#define OOP_BIGZOMBIEBOSS_H

#include "Enemy.h"
#include "Projectile.h"
#include "ResourceManager.h"
#include <cmath>
#include <ostream>
#include <vector>

class BigZombieBoss : public Enemy {
private:
    float fireTimer;
    std::vector<Projectile> &projectiles;
    ResourceManager &resources;

public:
    BigZombieBoss(const sf::Texture &texture, sf::Vector2f startPos,
                  std::vector<Projectile> &projs, ResourceManager &res,
                  int playerLevel = 0);

    Enemy *clone() const override;

    void update(float dt, sf::Vector2f playerPos) override;

    friend std::ostream &operator<<(std::ostream &os, const BigZombieBoss &obj);
};

#endif // OOP_BIGZOMBIEBOSS_H
