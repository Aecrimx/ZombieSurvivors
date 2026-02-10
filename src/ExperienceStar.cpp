#include "ExperienceStar.h"

ExperienceStar::ExperienceStar(const sf::Texture &texture, sf::Vector2f spawnPos, float xp): Pickup(texture, spawnPos, 150.f), xpValue(xp) {
}

void ExperienceStar::onCollect(Player &player) { player.addXP(xpValue); }

float ExperienceStar::getXPValue() const { return xpValue; }
