#ifndef OOP_HEALTHPICKUP_H
#define OOP_HEALTHPICKUP_H

#include <SFML/Graphics.hpp>
#include <cmath>

class HealthPickup {
private:
    sf::Sprite sprite;
    float healAmount; //0.25
    sf::Vector2f position;
    float magnetRange;

public:
    HealthPickup(const sf::Texture &texture, sf::Vector2f spawnPos,
                 float healPct = 0.25f)
        : sprite(texture), healAmount(healPct), position(spawnPos), magnetRange(150.f) {
        sf::Vector2u size = texture.getSize();
        sprite.setOrigin({size.x / 2.f, size.y / 2.f});
        sprite.setPosition(spawnPos);
        sprite.setScale({0.25f, 0.25f});
    }

    void update(float dt, sf::Vector2f playerPos) {
        sf::Vector2f dir = playerPos - position;
        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (dist < magnetRange && dist > 0) {
            dir /= dist;
            float pullSpeed = 300.f;
            position += dir * pullSpeed * dt;
            sprite.setPosition(position);
        }
    }

    void draw(sf::RenderWindow &window) const { window.draw(sprite); }

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }

    sf::Vector2f getPosition() const { return position; }

    float getHealAmount() const { return healAmount; }
};

#endif // OOP_HEALTHPICKUP_H
