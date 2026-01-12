#ifndef OOP_MENUSTATE_H
#define OOP_MENUSTATE_H

#include "Player.h"
#include "SaveManager.h"
#include "State.h"
#include <SFML/Graphics.hpp>
#include <vector>

class MenuState : public State {
private:
    sf::Font font;
    sf::Text titleText;

    struct CharacterCard {
        std::string name;
        std::string textureName;
        CharacterData data;
        bool unlocked;
        int highScore;

        sf::Sprite sprite;
        sf::RectangleShape cardBg;
        sf::Text nameText;
        sf::Text statsText;
        sf::Text scoreText;
        sf::Text lockedText;

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
