#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H

#include <ostream>

#include "Animation.h"
#include "ResourceManager.h"
#include "Weapon.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

class Item;

struct CharacterData {
    std::string name;
    std::string texture_name;
    float moveSpeed;
    float maxHealth;
    std::string startingWeapon;
    std::string Weapon_sprite;

    CharacterData(const std::string &n, const std::string &tex, float speed,
                  float hp, const std::string &weapon, const std::string &wSprite)
        : name(n), texture_name(tex), moveSpeed(speed), maxHealth(hp),
          startingWeapon(weapon), Weapon_sprite(wSprite) {
    }
};

class Player {
    sf::Sprite sprite;
    std::unique_ptr<Animation> animation;
    std::vector<std::unique_ptr<Weapon> > weapons;
    std::vector<std::unique_ptr<Item> > items;

    float speed;
    float baseSpeed;
    float maxHealth;
    float currentHealth;
    float healthRegen; // hp per sec

    int level;
    float currentXP;
    float xpToNextLevel;
    // pt directie pt arme
    bool facingRight;
    // de la iteme
    float damageReduction; //=[0.0, 1.0] (0.3 = 30% dmgReduction)
    float cooldownMultiplier; // 0.5 = 50% CDR

public:
    Player(const CharacterData &data, ResourceManager &resources);

    ~Player();

    // E doar move-only
    Player(const Player &other) = delete;

    Player &operator=(const Player &other) = delete;

    Player(Player &&other) noexcept = default;

    Player &operator=(Player &&other) noexcept = default;

    // void handleInput();
    void update(float dt, const sf::RenderWindow &window,
                const std::vector<std::unique_ptr<Enemy> > &enemies,
                std::vector<Projectile> &projectiles);

    void draw(sf::RenderWindow &window) const;

    void addWeapon(std::unique_ptr<Weapon> weapon);

    void addItem(std::unique_ptr<Item> item);

    const std::vector<std::unique_ptr<Weapon> > &getWeapons() const;

    const std::vector<std::unique_ptr<Item> > &getItems() const;

    void addXP(float amount);

    bool shouldLevelUp() const;

    void levelUp();

    int getLevel() const;

    float getCurrentXP() const;

    float getXPToNextLevel() const;

    void heal(float amount);

    void takeDamage(float amount);

    bool isDead() const;

    float getMaxHealth() const;

    void increaseMaxHealth(float amount);

    void setHealthRegen(float regen);

    float getHealthRegen() const;

    void setDamageReduction(float reduction);

    void setCooldownMultiplier(float mult);

    float getDamageReduction() const;

    float getCooldownMultiplier() const;

    void increaseSpeed(float multiplier);

    float getSpeed() const;

    sf::Vector2f getPos() const;

    sf::FloatRect getBounds() const;

    bool isFacingRight() const;

    friend std::ostream &operator<<(std::ostream &os, const Player &obj);

    float getHealthPoints() const;

private:
    virtual void print(std::ostream &os) const {
        os << "[Player] " << sprite.getTexture().getSize().x << "x"
                << sprite.getTexture().getSize().y << " Level: " << level
                << " HP: " << currentHealth << "/" << maxHealth << " XP: " << currentXP
                << "/" << xpToNextLevel << " Weapons: " << weapons.size()
                << " Items: " << items.size();
    }
};

#endif // OOP_PLAYER_H
