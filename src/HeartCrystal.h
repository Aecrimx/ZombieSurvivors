#ifndef OOP_HEARTCRYSTAL_H
#define OOP_HEARTCRYSTAL_H

#include "Item.h"
#include "Player.h"

class HeartCrystal : public Item {
public:
    HeartCrystal() : Item("Heart Crystal", 1) {
    }

    void applyEffect(Player &player) override {
        // La fiecare lvl up +10max hp si +0.5HP/sec regenerare
        const float maxHPIncrease = level * 10.f;
        const float regenIncrease = level * 0.5f;

        player.increaseMaxHealth(maxHPIncrease);
        player.setHealthRegen(regenIncrease);
    }

    Item *clone() const override { return new HeartCrystal(*this); }

private:
    void print(std::ostream &os) const override {
        os << "[Heart Crystal] Level: " << level << " (Max HP: +" << (level * 10)
                << ", Regen: +" << (level * 0.5f) << " HP/sec)";
    }
};

#endif // OOP_HEARTCRYSTAL_H
