#ifndef OOP_BONESHOOTER_H
#define OOP_BONESHOOTER_H

#include "Weapon.h"
#include <cmath>
#include <iostream>

class BoneShooter : public Weapon {
public:
    explicit BoneShooter(ResourceManager &res);

    Weapon *clone() const override;

    void update(float dt, sf::Vector2f playerPos,
                const std::vector<std::unique_ptr<Enemy> > &enemies,
                std::vector<Projectile> &projectiles) override;

private:
    void print(std::ostream &os) const override {
        os << "[BoneShooter]\n";
        os << "Name: " << name << " Cooldown: " << cooldown;
    }
};

#endif // OOP_BONESHOOTER_H
