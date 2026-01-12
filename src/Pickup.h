#ifndef OOP_PICKUP_H
#define OOP_PICKUP_H

#include <SFML/Graphics.hpp>
#include <cmath>

class Player;

class Pickup {
protected:
    sf::Sprite sprite;
    sf::Vector2f position;
    float magnetRange;

public:
    Pickup(const sf::Texture &texture, sf::Vector2f spawnPos,
           float magRange = 150.f)
        : sprite(texture), position(spawnPos), magnetRange(magRange) {
        sf::Vector2u size = texture.getSize();
        sprite.setOrigin({size.x / 2.f, size.y / 2.f});
        sprite.setPosition(spawnPos);
        sprite.setScale({0.3f, 0.3f});
    }

    virtual ~Pickup() = default;

    virtual void update(float dt, sf::Vector2f playerPos) {
        sf::Vector2f dir = playerPos - position;
        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (dist < magnetRange && dist > 0) {
            dir /= dist;
            float pullSpeed = 300.f;
            position += dir * pullSpeed * dt;
            sprite.setPosition(position);
        }
    }

    virtual void draw(sf::RenderWindow &window) const { window.draw(sprite); }

    virtual void onCollect(Player &player) = 0;

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::Vector2f getPosition() const { return position; }
};

#endif // OOP_PICKUP_H
