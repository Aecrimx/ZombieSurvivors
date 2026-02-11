#ifndef OOP_EXPERIENCESTAR_H
#define OOP_EXPERIENCESTAR_H

#include "Pickup.h"
#include "Player.h"

class ExperienceStar : public Pickup {
private:
    float xpValue;

public:
    ExperienceStar(const sf::Texture &texture, sf::Vector2f spawnPos,
                   float xp = 5.f);

    void onCollect(Player &player) override;

    //float getXPValue() const;
};

#endif // OOP_EXPERIENCESTAR_H
