#include "BigZombieBoss.h"

BigZombieBoss::BigZombieBoss(const sf::Texture &texture, const sf::Vector2f startPos,
                             std::vector<Projectile> &projs,
                             ResourceManager &res, const int playerLevel)
    : Enemy(texture, 500.f + (5.f * playerLevel), 25.f, 100.f), fireTimer(0.f),
      projectiles(projs), resources(res) {
    sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
    sprite.setPosition(startPos);
    sprite.setScale({1.0f, 1.0f});
}

Enemy *BigZombieBoss::clone() const { return new BigZombieBoss(*this); }

void BigZombieBoss::update(const float dt, const sf::Vector2f playerPos) {
    sf::Vector2f dir = playerPos - sprite.getPosition();
    const float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (length > 0) {
        dir /= length;
        sprite.move(dir * speed * dt);
        if (dir.x < 0)
            sprite.setScale({-1.0f, 1.0f});
        else
            sprite.setScale({1.0f, 1.0f});
    }

    fireTimer += dt;
    if (fireTimer >= 2.0f) {
        fireTimer = 0.f;

        if (length > 0) {
            sf::Vector2f projectileDir = dir;
            sf::Texture &tex = resources.getTexture("dumbbell");
            const float projSpeed = 200.f;
            const float projDmg = 15.f;

            /*
             * Atacul special al bossului e sa spawneze doua proiectle, unu la
             * stanga lui si unu la dreapta
             */
            const sf::FloatRect bounds = sprite.getGlobalBounds();
            const float offsetX = bounds.size.x * 0.3f;

            sf::Vector2f leftPos = sprite.getPosition() + sf::Vector2f(-offsetX, 0);
            projectiles.emplace_back(tex, leftPos, projectileDir, projSpeed, projDmg,
                                     true);
            sf::Vector2f rightPos = sprite.getPosition() + sf::Vector2f(offsetX, 0);
            projectiles.emplace_back(tex, rightPos, projectileDir, projSpeed, projDmg,
                                     true);
        }
    }
}

std::ostream &operator<<(std::ostream &os, const BigZombieBoss &obj) {
    return os << static_cast<const Enemy &>(obj);
}
