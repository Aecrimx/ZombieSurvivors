#ifndef OOP_LEVELUPSTATE_H
#define OOP_LEVELUPSTATE_H

#include "Player.h"
#include "State.h"
#include <SFML/Graphics.hpp>
#include <functional>
//Pt std::function ca template ca sa pot folosi functii lambda
#include <memory>
#include <string>
#include <vector>


class LevelUpState : public State {
private:
    sf::Font font;
    std::unique_ptr<sf::Text> titleText;

    struct UpgradeOption {
        std::string name;
        std::string description;
        std::function<void(Player &)> applyUpgrade;

        sf::RectangleShape bg;
        std::unique_ptr<sf::Text> nameText;
        std::unique_ptr<sf::Text> descText;
        sf::FloatRect bounds;
        bool hovered;
    };

    std::vector<UpgradeOption> options;
    int hoveredIndex;
    Player *player;

    void generateOptions();

    void updateLayout(int windowWidth, int windowHeight);

public:
    LevelUpState(Game &gameRef, Player *playerRef);

    void handleInput() override;

    void update(float dt) override;

    void draw() override;

    void Resize(int w, int h) override;
};

#endif // OOP_LEVELUPSTATE_H
