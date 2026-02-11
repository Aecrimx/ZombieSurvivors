#include "Item.h"

Item::Item(const std::string &itemName, int itemLevel)
    : name(itemName), level(itemLevel) {
}

Item::~Item() = default;

Item::Item(const Item &other) : name(other.name), level(other.level) {
    if (other.iconSprite) {
        iconSprite = std::make_unique<sf::Sprite>(*other.iconSprite);
    }
}

Item &Item::operator=(const Item &other) {
    if (this != &other) {
        name = other.name;
        level = other.level;
        if (other.iconSprite) {
            iconSprite = std::make_unique<sf::Sprite>(*other.iconSprite);
        } else {
            iconSprite.reset();
        }
    }
    return *this;
}

void Item::levelUp() {
    if (level < 3) {
        level++;
    }
}

bool Item::canLevelUp() const { return level < 3; }

int Item::getLevel() const { return level; }

const std::string& Item::getName() const { return name; }

const sf::Sprite *Item::getIcon() const { return iconSprite.get(); }

void Item::setIcon(const sf::Texture &texture) {
    iconSprite = std::make_unique<sf::Sprite>(texture);
}

std::ostream &operator<<(std::ostream &os, const Item &obj) {
    obj.print(os);
    return os;
}
