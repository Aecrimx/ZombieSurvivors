#include "DemonicBook.h"

DemonicBook::DemonicBook(ResourceManager &res)
    : Weapon("Demonic Book", 0.f, res), auraRadius(0.f), auraRadiusScale(4.0f),
      tickTimer(0.f), tickInterval(1.5f), damage(25.f),
      auraSprite(res.getTexture("damage_aura")) {
    sf::Vector2u size = res.getTexture("damage_aura").getSize();
    auraSprite.setOrigin({size.x / 2.f, size.y / 2.f});
    auraSprite.setScale({auraRadiusScale, auraRadiusScale});
    const sf::FloatRect gb = auraSprite.getGlobalBounds();
    auraRadius = std::min(gb.size.x, gb.size.y) / 2.f; // dependenta de size
}

Weapon *DemonicBook::clone() const { return new DemonicBook(*this); }

void DemonicBook::update(float dt, sf::Vector2f playerPos,
                         const std::vector<std::unique_ptr<Enemy> > &enemies,
                         std::vector<Projectile> &projectiles) {
    tickTimer += dt;

    auraSprite.setPosition(playerPos);

    const sf::FloatRect gb = auraSprite.getGlobalBounds();
    auraRadius = std::min(gb.size.x, gb.size.y) / 2.f;

    if (tickTimer >= tickInterval) {
        for (const auto &enemy: enemies) {
            sf::Vector2f diff = enemy->getPos() - playerPos;
            float distSq = diff.x * diff.x + diff.y * diff.y;
            float radiusSq = auraRadius * auraRadius;

            if (distSq <= radiusSq) {
                enemy->damageTaken(damage);
            }
        }
        tickTimer -= tickInterval;
    }
}

void DemonicBook::draw(sf::RenderWindow &window) const {
    window.draw(auraSprite);
}

void DemonicBook::print(std::ostream &os) const {
    os << "[DemonicBook]\n";
    os << "Name: " << name << " Cooldown: " << cooldown
            << " AuraRadius: " << auraRadius << " Damage: " << damage;
}
