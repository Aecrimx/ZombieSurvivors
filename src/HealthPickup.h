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
               float heal = 0.25f)
      : Pickup(texture, spawnPos, 150.f), healAmount(heal) {}

  void onCollect(Player &player) override {
    float maxHealth = player.getMaxHealth();
    player.heal(maxHealth * healAmount); // Heal 25% of max health
  }

  float getHealAmount() const { return healAmount; }
};

#endif // OOP_HEALTHPICKUP_H
