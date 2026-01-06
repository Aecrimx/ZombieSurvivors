#ifndef OOP_DEMONICBOOK_H
#define OOP_DEMONICBOOK_H

#include "Weapon.h"
#include <cmath>
#include <iostream>
#include <ostream>
#include <algorithm>

class DemonicBook : public Weapon {
private:
    float auraRadius;
    float auraRadiusScale;
    float tickTimer;
    float tickInterval;
    float damage;
    sf::Sprite auraSprite;

public:
    explicit DemonicBook(ResourceManager &res)
        : Weapon("Demonic Book", 0.f, res), auraRadius(0.f), auraRadiusScale(1.f),tickTimer(0.f),
          tickInterval(1.5f), damage(25.f),
          auraSprite(res.getTexture("damage_aura")) {
        sf::Vector2u size = res.getTexture("damage_aura").getSize();
        auraSprite.setOrigin({size.x / 2.f, size.y / 2.f});
        auraSprite.setScale({auraRadiusScale, auraRadiusScale});
        const sf::FloatRect gb = auraSprite.getGlobalBounds();
        auraRadius = std::min(gb.size.x, gb.size.y) / 2.f; // dependenta de size
    }

    Weapon *clone() const override { return new DemonicBook(*this); }

    void update(float dt, sf::Vector2f playerPos,
                const std::vector<std::unique_ptr<Enemy> > &enemies,
                std::vector<Projectile> &projectiles) override {
        tickTimer += dt;

        auraSprite.setPosition(playerPos);


        const sf::FloatRect gb = auraSprite.getGlobalBounds();
        auraRadius = std::min(gb.size.x, gb.size.y) / 2.f;

        if (tickTimer >= tickInterval) {
            for (const auto &enemy: enemies) {
                sf::Vector2f diff = enemy->getPos() - playerPos;
                float distSq = diff.x * diff.x + diff.y * diff.y;
                float radiusSq = auraRadius * auraRadius;

                if (distSq <= radiusSq) {
                    enemy->damageTaken(damage);
                }
            }
            tickTimer -= tickInterval;
        }
    }

    void draw(sf::RenderWindow &window) const override {
        window.draw(auraSprite);
    }

    friend std::ostream &operator<<(std::ostream &os, const DemonicBook &obj) {
        os << "[DemonicBook]\n";
        os << "Aura Radius: " << obj.auraRadius << '\n'
                << "Tick Interval: " << obj.tickInterval << '\n'
                << "Damage: " << obj.damage << '\n';
        return os;
    }
};

#endif // OOP_DEMONICBOOK_H
