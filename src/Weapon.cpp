//
// Created by Mircea on 13.12.2025.
//

#include "Weapon.h"

Weapon::Weapon(const std::string &name, float cooldown, ResourceManager &res)
    : name(name), cooldown(cooldown), timer(0.f), resources(res) {
}

Weapon::~Weapon() = default;

Weapon *Weapon::copy() const { return clone(); }

void Weapon::draw(sf::RenderWindow &window) const {
}

std::string Weapon::getName() const { return name; }

std::ostream &operator<<(std::ostream &os, const Weapon &obj) {
    obj.print(os);
    return os;
}
