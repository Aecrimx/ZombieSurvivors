#ifndef OOP_ITEM_H
#define OOP_ITEM_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class Player;

class Item {
protected:
    std::string name;
    int level; // 3 levele max
    std::unique_ptr<sf::Sprite> iconSprite;

public:
    Item(const std::string &itemName, int itemLevel = 1);

    virtual ~Item();

    Item(const Item &other);

    Item &operator=(const Item &other);

    virtual void applyEffect(Player &player) = 0;

    void levelUp();

    bool canLevelUp() const;

    int getLevel() const;

    const std::string& getName() const;

    // ui stuff
    const sf::Sprite *getIcon() const;

    void setIcon(const sf::Texture &texture);

    virtual Item *clone() const = 0;

    friend std::ostream &operator<<(std::ostream &os, const Item &obj);

private:
    virtual void print(std::ostream &os) const = 0;
};

#endif // OOP_ITEM_H
