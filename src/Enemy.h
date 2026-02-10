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
    Enemy(const Enemy &other)
        : sprite(other.sprite), health(other.health), damage(other.damage),
          speed(other.speed) {
    }

    Enemy &operator=(const Enemy &other) {
        if (this != &other) {
            sprite = other.sprite;
            health = other.health;
            damage = other.damage;
            speed = other.speed;
        }
        return *this;
    }

    Enemy(Enemy &&other) noexcept = default;
    Enemy &operator=(Enemy &&other) noexcept = default;
    friend void swap(Enemy &first, Enemy &second) noexcept {
        using std::swap;
        swap(first.sprite, second.sprite);
        swap(first.health, second.health);
        swap(first.damage, second.damage);
        swap(first.speed, second.speed);
    }

    virtual ~Enemy() = default;
    virtual Enemy *clone() const = 0;

    virtual void update(float dt, sf::Vector2f playerPos) = 0;

    virtual void draw(sf::RenderWindow &window) { window.draw(sprite); }

    void damageTaken(float amount) { health -= amount; }
    bool isDead() const { return health <= 0; }
    float getDamage() const { return damage; }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::Vector2f getPos() const { return sprite.getPosition(); }

    friend std::ostream &operator<<(std::ostream &os, const Enemy &obj);

private:
    virtual void print(std::ostream &os) const {
        os << "health: " << health << " damage: " << damage << " speed: " << speed;
    }
};

#endif // OOP_ENEMY_H
