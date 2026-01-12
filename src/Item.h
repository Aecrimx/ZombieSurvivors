#ifndef OOP_ITEM_H
#define OOP_ITEM_H

#include <SFML/Graphics.hpp>
#include <string>


class Player;

class Item {
protected:
    std::string name;
    int level; // 3 levele max
    sf::Sprite iconSprite;

public:
    Item(const std::string &itemName, int itemLevel = 1)
        : name(itemName), level(itemLevel) {
    }

    virtual ~Item() = default;

    virtual void applyEffect(Player &player) = 0;

    void levelUp() {
        if (level < 3) {
            level++;
        }
    }

    bool canLevelUp() const { return level < 3; }
    int getLevel() const { return level; }
    std::string getName() const { return name; }

    //ui stuff
    sf::Sprite getIcon() const { return iconSprite; }
    void setIcon(const sf::Texture &texture) { iconSprite.setTexture(texture); }
};

#endif // OOP_ITEM_H
