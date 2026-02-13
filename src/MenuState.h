#ifndef OOP_MENUSTATE_H
#define OOP_MENUSTATE_H

#include "Animation.h"
#include "Player.h"
#include "SaveManager.h"
#include "State.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>
#include <vector>

class MenuState : public State {
private:
    // sf::Font font;
    std::unique_ptr<sf::Text> titleText;
    std::optional<sf::Sprite>
    backgroundSprite; // test de calmare warning pt sfml3

    struct CharacterCard {
        std::string name;
        std::string textureName;
        CharacterData data{"", "", 0.f, 0.f, "", ""};
        bool unlocked{false};
        int highScore{0};

        std::unique_ptr<sf::Sprite> sprite;
        std::unique_ptr<Animation> animation;
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

    void handleEvent(const sf::Event &event) override;

    void update(float dt) override;

    void draw() override;

    void Resize(int w, int h) override;
};

#endif // OOP_MENUSTATE_H
