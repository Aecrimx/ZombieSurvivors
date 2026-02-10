#ifndef OOP_RANGEDENEMY_H
#define OOP_RANGEDENEMY_H

#include "Enemy.h"
#include "Projectile.h"
#include "ResourceManager.h"

class RangedEnemy : public Enemy {
protected:
    std::vector<Projectile> &projectiles;
    ResourceManager &resources;

public:
    RangedEnemy(const sf::Texture &texture, float hp, float dmg, float spd,
                std::vector<Projectile> &projectilesRef, ResourceManager &res);

    Enemy *clone() const override = 0;
};

#endif // OOP_RANGEDENEMY_H
