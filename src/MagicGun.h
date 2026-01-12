#ifndef OOP_MAGICGUN_H
#define OOP_MAGICGUN_H

#include "Weapon.h"
#include <cmath>
#include <iostream>

class MagicGun : public Weapon {
private:
    /*
     * Laserul trage in sens ceasornic la fiecare pozitie de ceas adica 360 / 12
     * => incepe sa traga la 0 grade, apoi la 30, 60 etc...
     */
    int currentAngle;
    const float angleIncrement = 30.f;

public:
    explicit MagicGun(ResourceManager &res)
        : Weapon("Magic Gun", 1.5f, res), currentAngle(0) {
    }

    Weapon *clone() const override { return new MagicGun(*this); }

    void update(float dt, sf::Vector2f playerPos,
                const std::vector<std::unique_ptr<Enemy> > &enemies,
                std::vector<Projectile> &projectiles) override {
        if (timer > 0)
            timer -= dt;

        if (timer <= 0) {
            float angleInDegrees = (currentAngle * 30.f) - 90.f; // -90 = -pi/2
            float angleInRadians = angleInDegrees * 3.14159f / 180.f;

            sf::Vector2f direction(std::cos(angleInRadians),
                                   std::sin(angleInRadians));

            sf::Texture &tex = resources.getTexture("laser_projectile");
            projectiles.emplace_back(tex, playerPos, direction, 300.f, 15.f, false);

            currentAngle = (currentAngle + 1) % 12;

            timer = cooldown;
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const MagicGun &obj) {
        os << "[MagicGun]\n";
        os << "Cooldown: " << obj.cooldown << '\n'
                << "Name: " << obj.name << '\n'
                << "Current Angle: " << obj.currentAngle << " o'clock\n";
        return os;
    }
};

#endif // OOP_MAGICGUN_H
