#ifndef OOP_MAGICGUN_H
#define OOP_MAGICGUN_H

#include "Weapon.h"

class MagicGun : public Weapon {
private:
    /*
     * Laserul trage in sens ceasornic la fiecare pozitie de ceas adica 360 / 12
     * => incepe sa traga la 0 grade, apoi la 30, 60 etc...
     */
    int currentAngle;
    //const float angleIncrement = 30.f;

public:
    explicit MagicGun(ResourceManager &res);

    Weapon *clone() const override;

    void update(float dt, sf::Vector2f playerPos,
                const std::vector<std::unique_ptr<Enemy> > &enemies,
                std::vector<Projectile> &projectiles) override;

private:
    void print(std::ostream &os) const override {
        os << "[MagicGun]\n";
        os << "Name: " << name << " Cooldown: " << cooldown
                << " CurrentAngle: " << currentAngle;
    }
};

#endif // OOP_MAGICGUN_H
