#ifndef OOP_EXPERIENCESTAR_H
#define OOP_EXPERIENCESTAR_H

#include "Pickup.h"
#include "Player.h"
#include <cmath>

class ExperienceStar : public Pickup {
private:
    float xpValue;

public:
    ExperienceStar(const sf::Texture &texture, sf::Vector2f spawnPos,
                   float xp = 5.f)
        : Pickup(texture, spawnPos, 150.f), xpValue(xp) {
    }

    void onCollect(Player &player) override { player.addXP(xpValue); }

    float getXPValue() const { return xpValue; }
};

#endif // OOP_EXPERIENCESTAR_H
