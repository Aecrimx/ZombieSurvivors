#ifndef OOP_WEAPON_H
#define OOP_WEAPON_H

#include "Enemy.h"
#include "Projectile.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Player;

class Weapon {
protected:
    std::string name;
    float cooldown;
    float timer;
    ResourceManager &resources;
    int level;        // (1-3)
    float damage;     // current damage
    float baseDamage; // de la lvl 1

public:
    Weapon(const std::string &name, float cooldown, ResourceManager &res,
           float baseDmg = 10.f);

    virtual ~Weapon();

    virtual Weapon *clone() const = 0;

    virtual Weapon *copy() const;

    virtual void update(float dt, sf::Vector2f playerPos,
                        const std::vector<std::unique_ptr<Enemy> > &enemies,
                        std::vector<Projectile> &projectiles) = 0;

    virtual void draw(sf::RenderWindow &window) const;

    const std::string& getName() const;

    void levelUp();
    bool canLevelUp() const;
    int getLevel() const;
    float getDamage() const;

    virtual void setOwner(Player &/*p*/) {} // metoda care e folosita doar pt arme care ar avea nevoie de cv informatie din Player

    friend std::ostream &operator<<(std::ostream &os, const Weapon &obj);

private:
    virtual void print(std::ostream &os) const = 0;
};

#endif // OOP_WEAPON_H
