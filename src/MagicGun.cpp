#include "MagicGun.h"

MagicGun::MagicGun(ResourceManager &res)
    : Weapon("Magic Gun", 2.5f, res), currentAngle(0) {
}

Weapon *MagicGun::clone() const { return new MagicGun(*this); }

void MagicGun::update(const float dt, sf::Vector2f playerPos,
                      const std::vector<std::unique_ptr<Enemy> > & /*enemies*/,
                      std::vector<Projectile> &projectiles) {
    if (timer > 0)
        timer -= dt;

    if (timer <= 0) {
        const float angleInDegrees = (currentAngle * 30.f) - 90.f; // -90 = -pi/2
        const float angleInRadians = angleInDegrees * 3.14159f / 180.f;

        sf::Vector2f direction(std::cos(angleInRadians), std::sin(angleInRadians));

        sf::Texture &tex = resources.getTexture("laser_projectile");
        projectiles.emplace_back(tex, playerPos, direction, 300.f, 15.f, false);

        currentAngle = (currentAngle + 1) % 12;

        timer = cooldown;
    }
}
