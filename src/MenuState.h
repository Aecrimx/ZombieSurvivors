#ifndef OOP_MENUSTATE_H
#define OOP_MENUSTATE_H

#include "Player.h"
#include "SaveManager.h"
#include "State.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class MenuState : public State {
private:
    sf::Font font;
    std::unique_ptr<sf::Text> titleText;

    struct CharacterCard {
        std::string name;
        std::string textureName;
        CharacterData data{"","",0.f,0.f,"",""};
        bool unlocked{false};
        int highScore{0};

        std::unique_ptr<sf::Sprite> sprite;
        sf::RectangleShape cardBg;
        std::unique_ptr<sf::Text> nameText;
        std::unique_ptr<sf::Text> statsText;
        std::unique_ptr<sf::Text> scoreText;
        std::unique_ptr<sf::Text> lockedText;

        sf::FloatRect mouse_bounds;
    };

    std::vector<CharacterCard> characters;
    int hoveredIndex;
    int selectedIndex;

    void setupCharacters();

    void updateLayout(int windowWidth, int windowHeight);

public:
    explicit MenuState(Game &gameRef);

    void handleInput() override;

    void update(float dt) override;

    void draw() override;

    void Resize(int w, int h) override;
};

#endif // OOP_MENUSTATE_H
