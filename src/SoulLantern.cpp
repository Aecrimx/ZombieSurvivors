#include "SoulLantern.h"

SoulLantern::SoulLantern(ResourceManager &res)
    : Weapon("Soul Lantern", 2.f, res) {
    damage = 7.5f;
}

Weapon *SoulLantern::clone() const { return new SoulLantern(*this); }

void SoulLantern::update(float dt, sf::Vector2f playerPos,
                         const std::vector<std::unique_ptr<Enemy> > &/*enemies*/,
                         std::vector<Projectile> &projectiles) {
    if (timer > 0)
        timer -= dt;

    if (timer <= 0) {
        sf::Texture &tex = resources.getTexture("soul_Scream");

        sf::Vector2f leftDirection(-1.f, 0.f);
        sf::Vector2f rightDirection(1.f, 0.f);

        projectiles.emplace_back(tex, playerPos, leftDirection, 150.f, damage,
                                 false);
        projectiles.emplace_back(tex, playerPos, rightDirection, 150.f, damage,
                                 false);

        timer = cooldown;
    }
}
