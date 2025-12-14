
#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Weapon.h"
#include "ResourceManager.h"


struct CharacterData {
    std::string name;
    std::string texture_name; // this is a texture key used with ResourceManager
    float moveSpeed;
    float maxHealth;
    std::string startingWeapon;
    std::string Weapon_sprite; // store by value to allow default construction
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
    void update(float dt);
    void draw(sf::RenderWindow& window);

    void addWeapon(const Weapon& weapon);

    sf::Vector2f getPos() const; // maybe needed idk


};


#endif //OOP_PLAYER_H