//
// Created by Mircea on 03.01.2026.
//

#include "RangedEnemy.h"

RangedEnemy::RangedEnemy(const sf::Texture &texture, float hp, float dmg,
                         float spd, std::vector<Projectile> &projectilesRef,
                         ResourceManager &res)
    : Enemy(texture, hp, dmg, spd), projectiles(projectilesRef),
      resources(res) {}
