#ifndef OOP_DEMONICBOOK_H
#define OOP_DEMONICBOOK_H

#include "Weapon.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <ostream>

class DemonicBook : public Weapon {
private:
    float auraRadius;
    float auraRadiusScale;
    float tickTimer;
    float tickInterval;
    float damage;
    sf::Sprite auraSprite;

public:
    explicit DemonicBook(ResourceManager &res);

    Weapon *clone() const override;

    void update(float dt, sf::Vector2f playerPos,
                const std::vector<std::unique_ptr<Enemy> > &enemies,
                std::vector<Projectile> &projectiles) override;

    void draw(sf::RenderWindow &window) const override;

private:
    void print(std::ostream &os) const override;
};

#endif // OOP_DEMONICBOOK_H
