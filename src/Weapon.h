#ifndef OOP_WEAPON_H
#define OOP_WEAPON_H

#include <string>

class Weapon {
    std::string name;
    std::string sprite_name;
    float damage;
    float cooldown;
    float timer;
public:
    Weapon(const std::string& name, const std::string& sprite_name, float damage, float cooldown)
        : name(name), sprite_name(sprite_name), damage(damage), cooldown(cooldown), timer(0.f) {}

    Weapon* copy() const {
        return new Weapon(*this);
    }

    void update(float dt) {
        if (timer > 0) timer -= dt;
    }

    bool canFire() const {return timer <= 0;}
    void resetCooldown() {timer = cooldown;}


};


#endif //OOP_WEAPON_H