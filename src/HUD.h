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
    sf::Font font;

    std::vector<sf::Sprite> weapon_slot_sprites;
    std::vector<sf::Sprite> item_slot_sprites;

    sf::RectangleShape xpBarRect;
    sf::RectangleShape xpBarFillRect;

    std::unique_ptr<sf::Text> timerText;
    std::unique_ptr<sf::Text> levelText;

    ResourceManager &resourceManager;

public:
    HUD(ResourceManager &res, int width, int height) : resourceManager(res) {
        uiView.setSize(
            sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
        uiView.setCenter(sf::Vector2f(static_cast<float>(width) / 2.f,
                                      static_cast<float>(height) / 2.f));

        if (!font.openFromFile("fonts/game_over.ttf")) {
            std::cerr << "Failed to load HUD font!" << std::endl;
        }

        timerText = std::make_unique<sf::Text>(font, "00:00", 32);
        timerText->setFillColor(sf::Color::White);
        timerText->setPosition({static_cast<float>(width) - 120.f, 40.f});

        levelText = std::make_unique<sf::Text>(font, "Lv 1", 36);
        levelText->setFillColor(sf::Color::Yellow);
        levelText->setStyle(sf::Text::Bold);
        levelText->setPosition({static_cast<float>(width) / 2.f - 40.f, 35.f});

        sf::Texture &slotTexture = res.getTexture("slot");

        const float slotScale = 1.f / 3.f;
        float startX = 20.f;
        float startY = 50.f;
        float space = 10.f;
        sf::Vector2u texSize = slotTexture.getSize();
        float slotWidth = static_cast<float>(texSize.x) * slotScale;

        for (int i = 0; i < 3; i++) {
            sf::Sprite s{slotTexture};
            s.setScale(sf::Vector2f(slotScale, slotScale));
            s.setPosition({startX + i * (slotWidth + space), startY});
            weapon_slot_sprites.push_back(s);
        }

        for (int i = 0; i < 3; i++) {
            sf::Sprite s{slotTexture};
            s.setScale(sf::Vector2f(slotScale, slotScale));
            s.setPosition(
                {startX + i * (slotWidth + space), startY + slotWidth + space});
            item_slot_sprites.push_back(s);
        }

        xpBarRect.setSize({static_cast<float>(width) - 40.f, 20.f});
        xpBarRect.setPosition({20.f, 10.f});
        xpBarRect.setFillColor(sf::Color(45, 45, 45));

        xpBarFillRect.setSize({0.f, 20.f});
        xpBarFillRect.setPosition({20.f, 10.f});
        xpBarFillRect.setFillColor(sf::Color::Blue);

        res.loadTexture("demonic_book_icon", "assets/demonic_book128x128.png");
        res.loadTexture("magic_gun_icon", "assets/magic_gun128x128.png");
        res.loadTexture("soul_lantern_icon", "assets/soul_lantern128x128.png");
        res.loadTexture("fire_wand_icon", "assets/fire_wand.png");
        res.loadTexture("armor_icon", "assets/armor_item128x128.png");
        res.loadTexture("boots_icon", "assets/boots128x128.png");
        res.loadTexture("cooldown_gauntlet_icon",
                        "assets/cooldown_gauntlet128x128.png");
        res.loadTexture("heart_crystal_icon", "assets/heart_crystal128x128.png");
    }

    void onResize(int width, int height) {
        uiView.setSize({static_cast<float>(width), static_cast<float>(height)});
        uiView.setCenter(
            {static_cast<float>(width) / 2.f, static_cast<float>(height) / 2.f});

        xpBarRect.setSize({static_cast<float>(width) - 40.f, 20.f});
        xpBarRect.setPosition({20.f, 10.f});
        xpBarFillRect.setPosition({20.f, 10.f});

        float invY = 50.f;
        float gap = 10.f;

        const float slot_size = 128.f / 3.f;

        for (int i = 0; i < 3; i++) {
            weapon_slot_sprites[i].setPosition({20.f + i * (slot_size + gap), invY});
            item_slot_sprites[i].setPosition(
                {20.f + i * (slot_size + gap), invY + slot_size + gap});
        }

        if (timerText)
            timerText->setPosition({static_cast<float>(width) - 120.f, 40.f});
        if (levelText)
            levelText->setPosition({static_cast<float>(width) / 2.f - 40.f, 35.f});
    }

    void update(const Player &player, float gameTimer) {
        float currentXP = player.getCurrentXP();
        float maxXP = player.getXPToNextLevel();
        float pct = maxXP > 0 ? (currentXP / maxXP) : 0.f;
        xpBarFillRect.setSize({xpBarRect.getSize().x * pct, xpBarRect.getSize().y});

        int minutes = static_cast<int>(gameTimer) / 60;
        int seconds = static_cast<int>(gameTimer) % 60;
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << minutes << ":"
                << std::setfill('0') << std::setw(2) << seconds;
        if (timerText)
            timerText->setString(oss.str());

        if (levelText)
            levelText->setString("Lv " + std::to_string(player.getLevel()));
    }

    void draw(sf::RenderWindow &window, const Player &player) {
        sf::View originalView = window.getView();
        const sf::FloatRect vp = originalView.getViewport();
        const sf::Vector2u ws = window.getSize();

        uiView.setViewport(vp);
        uiView.setSize({
            static_cast<float>(ws.x) * vp.size.x,
            static_cast<float>(ws.y) * vp.size.y
        });
        uiView.setCenter({uiView.getSize().x / 2.f, uiView.getSize().y / 2.f});

        window.setView(uiView);

        window.draw(xpBarRect);
        window.draw(xpBarFillRect);

        for (const auto &s: weapon_slot_sprites)
            window.draw(s);
        for (const auto &s: item_slot_sprites)
            window.draw(s);

        const auto &weapons = player.getWeapons();
        const float iconScale = 1.f / 3.f;
        for (size_t i = 0; i < weapons.size() && i < 3; ++i) {
            std::string weaponName = weapons[i]->getName();
            std::string textureName = "";

            if (weaponName == "Demonic Book")
                textureName = "demonic_book_icon";
            else if (weaponName == "Magic Gun")
                textureName = "magic_gun_icon";
            else if (weaponName == "Soul Lantern")
                textureName = "soul_lantern_icon";
            else if (weaponName == "Fire Wand")
                textureName = "fire_wand_icon";

            if (!textureName.empty()) {
                sf::Sprite icon(resourceManager.getTexture(textureName));
                icon.setScale({iconScale, iconScale});
                icon.setPosition(weapon_slot_sprites[i].getPosition());
                window.draw(icon);
            }
        }

        const auto &items = player.getItems();
        for (size_t i = 0; i < items.size() && i < 3; ++i) {
            std::string itemName = items[i]->getName();
            std::string textureName = "";

            if (itemName == "Armor")
                textureName = "armor_icon";
            else if (itemName == "Boots")
                textureName = "boots_icon";
            else if (itemName == "Cooldown Gauntlet")
                textureName = "cooldown_gauntlet_icon";
            else if (itemName == "Heart Crystal")
                textureName = "heart_crystal_icon";

            if (!textureName.empty()) {
                sf::Sprite icon(resourceManager.getTexture(textureName));
                icon.setScale({iconScale, iconScale});
                icon.setPosition(item_slot_sprites[i].getPosition());
                window.draw(icon);
            }
        }

        if (timerText)
            window.draw(*timerText);
        if (levelText)
            window.draw(*levelText);

        window.setView(originalView);
    }
};

#endif // OOP_HUD_H
