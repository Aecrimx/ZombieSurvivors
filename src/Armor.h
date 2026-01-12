#ifndef OOP_ARMOR_H
#define OOP_ARMOR_H

#include "Item.h"
#include "Player.h"

class Armor : public Item {
public:
    Armor() : Item("Armor", 1) {
    }

    void applyEffect(Player &player) override {
        //10% damage reduction pt fiecare level up
        float reduction = level * 0.1f;
        player.setDamageReduction(reduction);
    }
};

#endif // OOP_ARMOR_H
