//
// Created by Mircea on 20.12.2025.
//


#ifndef OOP_FLYING_SKULL_H
#define OOP_FLYING_SKULL_H
#include "GameState.h"
#include "RangedEnemy.h"
#include "ResourceManager.h"
#include "Animation.h"
#include <algorithm>
/*
 * pt sheet : 149 w 3072h, 24 de frame-uri
 * deci frame size 149x128
 */
class Flying_Skull : public RangedEnemy{
    float fireTimer;
    Animation anim;
public:
    Flying_Skull(sf::Texture &texture, sf::Vector2f startPos,
                 std::vector<Projectile>& projs, ResourceManager& res)
        : RangedEnemy(texture, 25.f, 10.f, 60.f, projs, res),
          fireTimer(0.f), anim(sprite) {
        
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

    void update(float dt, sf::Vector2f playerPos) override {
        anim.update(dt);
        sf::Vector2f dir = playerPos - sprite.getPosition();
        float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

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

            if (length > 0 && projectiles && resources) {
                sf::Vector2f projectileDir = dir / length;
                sf::Texture& tex = resources->getTexture("skull");
                const float projSpeed = 300.f;
                const float projDmg = 5.f;
                projectiles->emplace_back(tex, sprite.getPosition(), projectileDir, projSpeed, projDmg, true);
            }
        }
    }
};


#endif //OOP_FLYING_SKULL_H