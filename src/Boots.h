#ifndef OOP_BOOTS_H
#define OOP_BOOTS_H

#include "Item.h"
#include "Player.h"

class Boots : public Item {
public:
    Boots() : Item("Boots", 1) {
    }

    void applyEffect(Player &player) override {
        // la fiecare level 10% speed increase
        const float speedMultiplier = 1.0f + (level * 0.1f);
        player.increaseSpeed(speedMultiplier);
    }

    Item *clone() const override { return new Boots(*this); }

private:
    void print(std::ostream &os) const override {
        os << "[Boots] Level: " << level << " (Speed Boost: +" << (level * 10)
                << "%)";
    }
};

#endif // OOP_BOOTS_H
