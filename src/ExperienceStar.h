#ifndef OOP_EXPERIENCESTAR_H
#define OOP_EXPERIENCESTAR_H

#include <SFML/Graphics.hpp>
#include <cmath>

class ExperienceStar {
private:
    sf::Sprite sprite;
    float xpValue;
    sf::Vector2f position;
    float magnetRange;

public:
    ExperienceStar(const sf::Texture &texture, sf::Vector2f spawnPos,
                   float xp = 5.f)
        : sprite(texture), xpValue(xp), position(spawnPos), magnetRange(150.f) {
        sf::Vector2u size = texture.getSize();
        sprite.setOrigin({size.x / 2.f, size.y / 2.f});
        sprite.setPosition(spawnPos);
        sprite.setScale({0.3f, 0.3f}); //aici sa verifici la testing
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

    float getXPValue() const { return xpValue; }
};

#endif // OOP_EXPERIENCESTAR_H
