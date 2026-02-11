//
// Created by Mircea on 20.12.2025.
//

#include "Flying_Skull.h"

Flying_Skull::Flying_Skull(const sf::Texture &texture, const sf::Vector2f startPos,
                           std::vector<Projectile> &projs, ResourceManager &res)
    : RangedEnemy(texture, 25.f, 10.f, 60.f, projs, res), fireTimer(0.f),
      anim(sprite) {
    const int frameW = 149;
    const int frameH = 128;

    const sf::Vector2u tsize = texture.getSize();

    const int frames = std::max(1u, tsize.y / frameH);
    for (int i = 0; i < frames; ++i) {
        anim.addFrame(Frame{sf::IntRect({0, i * frameH}, {frameW, frameH}), 0.05});
    }
    sprite.setOrigin({frameW / 2.f, frameH / 2.f});
    sprite.setTextureRect(sf::IntRect({0, 0}, {frameW, frameH}));
    sprite.setPosition(startPos);
    sprite.setScale({0.7f, 0.7f});
}

Enemy *Flying_Skull::clone() const { return new Flying_Skull(*this); }

void Flying_Skull::update(const float dt, const sf::Vector2f playerPos) {
    anim.update(dt);
    const sf::Vector2f dir = playerPos - sprite.getPosition();
    const float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (length > 300.f) {
        sf::Vector2f ndir = dir / length;
        sprite.move(ndir * speed * dt);

        if (ndir.x < 0)
            sprite.setScale({-0.7f, 0.7f});
        else
            sprite.setScale({0.7f, 0.7f});
    }

    fireTimer += dt;
    if (fireTimer >= 3.5f) {
        fireTimer = 0.f;

        if (length > 0) {
            sf::Vector2f projectileDir = dir / length;
            sf::Texture &tex = resources.getTexture("skull");
            const float projSpeed = 300.f;
            const float projDmg = 5.f;
            projectiles.emplace_back(tex, sprite.getPosition(), projectileDir,
                                     projSpeed, projDmg, true);
        }
    }
}
