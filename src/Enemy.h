#ifndef OOP_ENEMY_H
#define OOP_ENEMY_H

#include <SFML/Graphics.hpp>
#include <ostream>

class Enemy {
protected:
    sf::Sprite sprite;
    float health, damage, speed;

public:
    Enemy(const sf::Texture &texture, float hp, float dmg, float speed)
        : sprite(texture), health(hp), damage(dmg), speed(speed) {
    }

    virtual ~Enemy() = default;

    virtual void update(float dt, sf::Vector2f playerPos) = 0;

    virtual void draw(sf::RenderWindow &window) { window.draw(sprite); }

    void damageTaken(float amount) { health -= amount; }
    bool isDead() const { return health <= 0; }
    float getDamage() const { return damage; }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::Vector2f getPos() const { return sprite.getPosition(); }

    friend std::ostream &operator<<(std::ostream &os, const Enemy &obj) {
        return os << "health: " << obj.health << " damage: " << obj.damage
               << " speed: " << obj.speed;
    }
};

#endif // OOP_ENEMY_H
