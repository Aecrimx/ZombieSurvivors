#ifndef OOP_SOULLANTERN_H
#define OOP_SOULLANTERN_H

#include "Projectile.h"
#include "Weapon.h"
#include <cmath>
#include <iostream>

class SoulScream : public Projectile {
private:
    float lifetime;
    float maxLifetime;
    float startScale;
    float endScale;

public:
    SoulScream(const sf::Texture &texture, sf::Vector2f startPos,
               sf::Vector2f dir, float spd, float dmg)
        : Projectile(texture, startPos, dir, spd, dmg, false), lifetime(0.f),
          maxLifetime(4.f), startScale(0.3f), endScale(1.2f) {
        sprite.setScale({startScale, startScale});
    }

    void update(float dt) override {
        lifetime += dt;

        float progress = std::min(lifetime / maxLifetime, 1.f);
        float currentScale = startScale + (endScale - startScale) * progress;
        sprite.setScale({currentScale, currentScale});

        if (lifetime >= maxLifetime) {
            dead = true;
        }

        sprite.move(direction * speed * dt);
    }
};

class SoulLantern : public Weapon {
public:
    explicit SoulLantern(ResourceManager &res)
        : Weapon("Soul Lantern", 3.0f, res) {
    }

    Weapon *clone() const override { return new SoulLantern(*this); }

    void update(float dt, sf::Vector2f playerPos,
                const std::vector<std::unique_ptr<Enemy> > &enemies,
                std::vector<Projectile> &projectiles) override {
        if (timer > 0)
            timer -= dt;

        if (timer <= 0) {
            sf::Vector2f direction(1.f, 0.f);

            sf::Texture &tex = resources.getTexture("soul_Scream32x64");

            projectiles.emplace_back(tex, playerPos, direction, 150.f, 35.f, false);

            timer = cooldown;
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const SoulLantern &obj) {
        os << "[SoulLantern]\n";
        os << "Cooldown: " << obj.cooldown << '\n' << "Name: " << obj.name << '\n';
        return os;
    }
};

#endif // OOP_SOULLANTERN_H
