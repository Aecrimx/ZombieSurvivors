#include "HealthPickup.h"

HealthPickup::HealthPickup(const sf::Texture &texture, sf::Vector2f spawnPos,
                           float heal)
    : Pickup(texture, spawnPos, 150.f), healAmount(heal) {
}

void HealthPickup::onCollect(Player &player) {
    float maxHealth = player.getMaxHealth();
    player.heal(maxHealth * healAmount);
}

// float HealthPickup::getHealAmount() const { return healAmount; }
