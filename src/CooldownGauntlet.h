#ifndef OOP_COOLDOWNGAUNTLET_H
#define OOP_COOLDOWNGAUNTLET_H

#include "Item.h"
#include "Player.h"

class CooldownGauntlet : public Item {
public:
    CooldownGauntlet() : Item("Cooldown Gauntlet", 1) {
    }

    void applyEffect(Player &player) override {
        // Level 1: 16.67%, Level 2: 33.34%, Level 3: 50% cooldown reduction
        // lvl1 : 16.67%, apoi 33.35% apoi 50%
        const float cdMultiplier = 1.0f - (static_cast<float>(level) * 0.1667f);
        player.setCooldownMultiplier(cdMultiplier);
    }

    Item *clone() const override { return new CooldownGauntlet(*this); }

private:
    void print(std::ostream &os) const override {
        const float cdr = level * 16.67f;
        os << "[Cooldown Gauntlet] Level: " << level << " (CDR: " << cdr << "%)";
    }
};

#endif // OOP_COOLDOWNGAUNTLET_H
