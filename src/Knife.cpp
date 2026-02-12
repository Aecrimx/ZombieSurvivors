#include "Knife.h"
#include "Player.h"

Knife::Knife(ResourceManager &res, Player *playerPtr)
    : Weapon("Knife", 0.8f, res, 3.5f), player(playerPtr) {
}

Weapon *Knife::clone() const { return new Knife(*this); }

void Knife::update(float dt, sf::Vector2f playerPos,
                   const std::vector<std::unique_ptr<Enemy> > & /*enemies*/,
                   std::vector<Projectile> &projectiles) {
    if (timer > 0)
        timer -= dt;

    if (timer <= 0 && player) {
        sf::Texture &tex = resources.getTexture("knife_projectile");

        sf::Vector2f direction;
        if (player->isFacingRight()) {
            direction = sf::Vector2f(1.f, 0.f);
        } else {
            direction = sf::Vector2f(-1.f, 0.f);
        }

        projectiles.emplace_back(tex, playerPos, direction, 300.f, damage, false);

        timer = cooldown;
    }
}
