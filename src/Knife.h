#ifndef OOP_KNIFE_H
#define OOP_KNIFE_H

#include "Weapon.h"

class Player;

class Knife : public Weapon {
private:
    Player *player;

public:
    explicit Knife(ResourceManager &res, Player *playerPtr);

    Weapon *clone() const override;

    void setOwner(Player &p) override;

    void update(float dt, sf::Vector2f playerPos,
                const std::vector<std::unique_ptr<Enemy> > &enemies,
                std::vector<Projectile> &projectiles) override;

private:
    void print(std::ostream &os) const override {
        os << "[Knife]\n";
        os << "Name: " << name << " Cooldown: " << cooldown
                << " Damage: " << damage;
    }
};

#endif // OOP_KNIFE_H
