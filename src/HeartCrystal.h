#ifndef OOP_HEARTCRYSTAL_H
#define OOP_HEARTCRYSTAL_H

#include "Item.h"
#include "Player.h"

class HeartCrystal : public Item {
public:
    HeartCrystal() : Item("Heart Crystal", 1) {
    }

    void applyEffect(Player &player) override {
        //La fiecare lvl up +10max hp si +0.5HP/sec regenerare
        float maxHPIncrease = level * 10.f;
        float regenIncrease = level * 0.5f;

        player.increaseMaxHealth(maxHPIncrease);
        player.setHealthRegen(regenIncrease);
    }
};

#endif // OOP_HEARTCRYSTAL_H
