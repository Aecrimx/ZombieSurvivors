#ifndef OOP_HEALTHPICKUP_H
#define OOP_HEALTHPICKUP_H

#include "Pickup.h"
#include "Player.h"
#include <cmath>

class HealthPickup : public Pickup {
private:
    float healAmount;

public:
    HealthPickup(const sf::Texture &texture, sf::Vector2f spawnPos,
                 float heal = 0.25f);

    void onCollect(Player &player) override;

    float getHealAmount() const;
};

#endif // OOP_HEALTHPICKUP_H
