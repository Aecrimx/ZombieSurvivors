#ifndef OOP_RANGEDENEMY_H
#define OOP_RANGEDENEMY_H

#include "Enemy.h"
#include "Projectile.h"
#include "ResourceManager.h"

class RangedEnemy : public Enemy{
protected:
    std::vector<Projectile>* projectiles{nullptr};
    ResourceManager* resources{nullptr};
public:
    RangedEnemy(const sf::Texture &texture, float hp, float dmg, float spd,
                std::vector<Projectile>& projectilesRef, ResourceManager& res)
        : Enemy(texture, hp, dmg, spd), projectiles(&projectilesRef), resources(&res) {}

};


#endif //OOP_RANGEDENEMY_H