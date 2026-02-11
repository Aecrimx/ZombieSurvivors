#include "HealthPickup.h"

HealthPickup::HealthPickup(const sf::Texture &texture, const sf::Vector2f spawnPos,
                           const float heal)
    : Pickup(texture, spawnPos, 150.f), healAmount(heal) {
}

void HealthPickup::onCollect(Player &player) {
    const float maxHealth = player.getMaxHealth();
    player.heal(maxHealth * healAmount);
}

// float HealthPickup::getHealAmount() const { return healAmount; }
