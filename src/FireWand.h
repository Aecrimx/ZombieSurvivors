#ifndef OOP_FIREWAND_H
#define OOP_FIREWAND_H

#include "Weapon.h"
#include <ostream>

class FireWand : public Weapon {
public:
    explicit FireWand(ResourceManager &res);

    Weapon *clone() const override;

    void update(float dt, sf::Vector2f playerPos,
                const std::vector<std::unique_ptr<Enemy> > &enemies,
                std::vector<Projectile> &projectiles) override;

private:
    void print(std::ostream &os) const override {
        os << "[FireWand]\n";
        os << "Name: " << name << " Cooldown: " << cooldown << " Timer: " << timer;
    }
};

#endif // OOP_FIREWAND_H
