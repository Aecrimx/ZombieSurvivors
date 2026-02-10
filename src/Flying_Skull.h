//
// Created by Mircea on 20.12.2025.
//

#ifndef OOP_FLYING_SKULL_H
#define OOP_FLYING_SKULL_H
#include "Animation.h"
#include "GameState.h"
#include "RangedEnemy.h"
#include "ResourceManager.h"
#include <algorithm>
/*
 * pt sheet : 149 w 3072h, 24 de frame-uri
 * deci frame size 149x128
 */
class Flying_Skull : public RangedEnemy {
    float fireTimer;
    Animation anim;

public:
    Flying_Skull(sf::Texture &texture, sf::Vector2f startPos,
                 std::vector<Projectile> &projs, ResourceManager &res);

    Enemy *clone() const override;

    void update(float dt, sf::Vector2f playerPos) override;
};

#endif // OOP_FLYING_SKULL_H
