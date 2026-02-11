#ifndef OOP_SOULLANTERN_H
#define OOP_SOULLANTERN_H

#include "Weapon.h"

class SoulLantern : public Weapon {
public:
    explicit SoulLantern(ResourceManager &res);

    Weapon *clone() const override;

    void update(float dt, sf::Vector2f playerPos,
                const std::vector<std::unique_ptr<Enemy> > &enemies,
                std::vector<Projectile> &projectiles) override;

private:
    void print(std::ostream &os) const override {
        os << "[SoulLantern]\n";
        os << "Name: " << name << " Cooldown: " << cooldown
                << " Damage: " << damage;
    }
};

#endif // OOP_SOULLANTERN_H
