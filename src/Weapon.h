#ifndef OOP_WEAPON_H
#define OOP_WEAPON_H

#include "Enemy.h"
#include "Projectile.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <ostream>
#include <vector>

class Weapon {
protected:
    std::string name;
    float cooldown;
    float timer;
    ResourceManager &resources;

public:
    Weapon(const std::string &name, float cooldown, ResourceManager &res);

    virtual ~Weapon();

    virtual Weapon *clone() const = 0;

    virtual Weapon *copy() const;

    virtual void update(float dt, sf::Vector2f playerPos,
                        const std::vector<std::unique_ptr<Enemy> > &enemies,
                        std::vector<Projectile> &projectiles) = 0;

    virtual void draw(sf::RenderWindow &window) const;

    std::string getName() const;

    friend std::ostream &operator<<(std::ostream &os, const Weapon &obj);

private:
    virtual void print(std::ostream &os) const = 0;
};

#endif // OOP_WEAPON_H
