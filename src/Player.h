
#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Weapon.h"
#include "ResourceManager.h"


struct CharacterData {
    std::string name;
    std::string texture_name;
    float moveSpeed;
    float maxHealth;
    std::string startingWeapon;
    std::string Weapon_sprite;
};

class Player {
    sf::Sprite sprite;
    std::vector<Weapon*> weapons;

    float speed;
    float maxHealth;
    float currentHealth;


public:
    Player(const CharacterData& data, ResourceManager& resources);

    ~Player();

    Player(const Player& other);
    Player& operator=(const Player& other);

    //void handleInput();
    void update(float dt, const std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles);
    void draw(sf::RenderWindow& window) const;

    void addWeapon(const Weapon& weapon);

    sf::Vector2f getPos() const;


};


#endif //OOP_PLAYER_H