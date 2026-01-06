#ifndef OOP_HUD_H
#define OOP_HUD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "ResourceManager.h"
#include "Player.h"

/*
 * note to self: inv slot e scaled down de la 128x128 la 32x32
 * Keep it la fel pt iteme
 */

class HUD {
    sf::View uiView;
    sf::Font font;

    std::vector<sf::Sprite> weapon_slot_sprites;
    std::vector<sf::Sprite> item_slot_sprites;

    sf::RectangleShape xpBarRect;
    sf::RectangleShape xpBarFillRect;

public:
    HUD(ResourceManager& res, int width, int height) {
        uiView.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
        uiView.setCenter(sf::Vector2f(static_cast<float>(width) / 2.f, static_cast<float>(height) / 2.f));

        sf::Texture& slotTexture = res.getTexture("slot");

        const float slotScale = 1.f / 3.f; // pt treimea de scale
        float startX = 20.f;
        float startY = 20.f;
        float space = 10.f;
        sf::Vector2u texSize = slotTexture.getSize();
        float slotWidth = static_cast<float>(texSize.x) * slotScale;

        for (int i = 0; i < 3 ; i++) {
            sf::Sprite s{slotTexture};
            s.setScale(sf::Vector2f(slotScale, slotScale));
            s.setPosition({startX + i * (slotWidth + space), startY});
            weapon_slot_sprites.push_back(s);
        }

        for (int i = 0; i < 3 ; i++) {
            sf::Sprite s{slotTexture};
            s.setScale(sf::Vector2f(slotScale, slotScale));
            s.setPosition({startX + i * (slotWidth + space), startY + slotWidth + space});
            item_slot_sprites.push_back(s);
        }

        xpBarRect.setSize({static_cast<float>(width) - 40.f, 20.f});
        xpBarRect.setPosition({20.f, 20.f});
        xpBarRect.setFillColor(sf::Color(45, 45, 45));

        xpBarFillRect.setSize({0.f, 20.f});
        xpBarFillRect.setPosition({20.f, 20.f});
        xpBarFillRect.setFillColor(sf::Color::Blue);
    }


    void onResize(int width, int height) {
        uiView.setSize({static_cast<float>(width), static_cast<float>(height)});
        uiView.setCenter({static_cast<float>(width) / 2.f, static_cast<float>(height) / 2.f});

        xpBarRect.setSize({static_cast<float>(width) - 40.f, 20.f});

        float y_gap = 10.f;
        xpBarRect.setPosition({20.f, y_gap});
        xpBarFillRect.setPosition({20.f, y_gap});

        float invY = 40.f;
        float gap = 10.f;

        const float slot_size = 128.f / 3.f;

        for (int i = 0; i < 3; i++) {
            weapon_slot_sprites[i].setPosition({20.f + i * (slot_size + gap), invY});
            item_slot_sprites[i].setPosition({20.f + i * (slot_size + gap), invY + slot_size + gap});
        }
    }

    void update(float currentXP, float maxXP) {
        float pct = maxXP > 0 ? (currentXP / maxXP) : 0.f;
        xpBarFillRect.setSize({xpBarRect.getSize().x * pct, xpBarRect.getSize().y});
        //std::cout << pct << std::endl;
    }

    void draw(sf::RenderWindow& window) {
        sf::View originalView = window.getView();
        const sf::FloatRect vp = originalView.getViewport(); // pt aliniere cu viewportul letterbox
        const sf::Vector2u ws = window.getSize();

        //mappare la letterbox
        uiView.setViewport(vp);
        uiView.setSize({static_cast<float>(ws.x) * vp.size.x, static_cast<float>(ws.y) * vp.size.y});
        uiView.setCenter({uiView.getSize().x / 2.f, uiView.getSize().y / 2.f});

        window.setView(uiView);

        window.draw(xpBarRect);
        window.draw(xpBarFillRect);

        for (const auto& s : weapon_slot_sprites) window.draw(s);
        for (const auto& s : item_slot_sprites) window.draw(s);

        window.setView(originalView);
    }
};


#endif //OOP_HUD_H