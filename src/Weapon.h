#ifndef OOP_WEAPON_H
#define OOP_WEAPON_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemy.h"
#include "Projectile.h"
#include "ResourceManager.h"

class Weapon {
protected:
    std::string name;
    float cooldown;
    float timer;
    ResourceManager& resources;

public:
    Weapon(const std::string& name, float cooldown, ResourceManager& res)
        : name(name), cooldown(cooldown), timer(0.f), resources(res) {}

    virtual ~Weapon() = default;
    virtual Weapon* clone() const = 0;
    virtual Weapon* copy() const { return clone(); }

    virtual void update(float dt, sf::Vector2f playerPos,
                        const std::vector<std::unique_ptr<Enemy>>& enemies,
                        std::vector<Projectile>& projectiles) = 0;
};


#endif //OOP_WEAPON_H