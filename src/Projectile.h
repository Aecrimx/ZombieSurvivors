#ifndef OOP_PROJECTILE_H
#define OOP_PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <ostream>

class Projectile {
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float damage;
    float life_time;
    bool hostile;

public:
    Projectile(const sf::Texture &texture, sf::Vector2f pos, sf::Vector2f dir,
               float speed, float dmg, bool isHostile = false);

    Projectile(const Projectile &other);
    Projectile &operator=(const Projectile &other);

    Projectile(Projectile &&other) noexcept = default;
    Projectile &operator=(Projectile &&other) noexcept = default;

    friend void swap(Projectile &first, Projectile &second) noexcept {
        using std::swap;
        swap(first.sprite, second.sprite);
        swap(first.velocity, second.velocity);
        swap(first.damage, second.damage);
        swap(first.life_time, second.life_time);
        swap(first.hostile, second.hostile);
    }

    void update(float dt);

    void draw(sf::RenderWindow &window) const;

    bool isDead() const;

    sf::FloatRect getBounds() const;

    float getDamage() const;

    // void Destroy() {life_time = -1.f;}

    bool isHostile() const;

    friend std::ostream &operator<<(std::ostream &os, const Projectile &obj);

private:
    virtual void print(std::ostream &os) const {
        os << "[Projectile] damage: " << damage << " lifetime: " << life_time
                << " hostile: " << (hostile ? "yes" : "no");
    }
};

#endif // OOP_PROJECTILE_H
