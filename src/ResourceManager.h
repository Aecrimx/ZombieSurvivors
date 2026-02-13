#ifndef OOP_RESOURCEMANAGER_H
#define OOP_RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ResourceManager {
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Font> fonts; // e overkill map pt un singur font dar poate doresc sa mai adaug un font
    void ensureMissingTexture();
    void loadTextureInternal(const std::string &name, const std::string &path);

public:
    ResourceManager();

    void loadTexture(const std::string &name, const std::string &path);

    sf::Texture &getTexture(const std::string &name);

    void loadFont(const std::string &name, const std::string &path);
    sf::Font &getFont(const std::string &name);

    bool hasTexture(const std::string &name) const;



    ~ResourceManager();

    friend std::ostream &operator<<(std::ostream &os, const ResourceManager &rm);
};

#endif // OOP_RESOURCEMANAGER_H
