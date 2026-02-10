#ifndef OOP_RESOURCEMANAGER_H
#define OOP_RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>
#include <stdexcept> //de scos t2

class ResourceManager {
    std::map<std::string, sf::Texture> textures;

public:
    ResourceManager();

    void loadTexture(const std::string &name, const std::string &path);

    sf::Texture &getTexture(const std::string &name);

    ~ResourceManager();

    friend std::ostream &operator<<(std::ostream &os, const ResourceManager &rm);
};


#endif //OOP_RESOURCEMANAGER_H
