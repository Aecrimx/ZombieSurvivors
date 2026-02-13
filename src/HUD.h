#ifndef OOP_HUD_H
#define OOP_HUD_H

#include "Item.h"
#include "Player.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

class HUD {
    sf::View uiView;
    // sf::Font font;

    std::vector<sf::Sprite> weapon_slot_sprites;
    std::vector<sf::Sprite> item_slot_sprites;

    sf::RectangleShape xpBarRect;
    sf::RectangleShape xpBarFillRect;

    std::unique_ptr<sf::Text> timerText;
    std::unique_ptr<sf::Text> levelText;

    ResourceManager &resourceManager;

public:
    HUD(ResourceManager &res, int width, int height);

    void onResize(int width, int height);

    void update(const Player &player, float gameTimer);

    void draw(sf::RenderWindow &window, const Player &player);
};

#endif // OOP_HUD_H
