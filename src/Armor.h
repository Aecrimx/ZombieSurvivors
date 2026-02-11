#ifndef OOP_ARMOR_H
#define OOP_ARMOR_H

#include "Item.h"
#include "Player.h"

class Armor : public Item {
public:
    Armor() : Item("Armor", 1) {
    }

    void applyEffect(Player &player) override {
        // 10% damage reduction pt fiecare level up
        const float reduction = static_cast<float>(level) * 0.1f;
        player.setDamageReduction(reduction);
    }

    Item *clone() const override { return new Armor(*this); }

private:
    void print(std::ostream &os) const override {
        os << "[Armor] Level: " << level << " (Damage Reduction: " << (level * 10)
                << "%)";
    }
};

#endif // OOP_ARMOR_H
