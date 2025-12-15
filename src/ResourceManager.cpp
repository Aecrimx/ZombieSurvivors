//
// Created by Mircea on 13.12.2025.
//

#include "ResourceManager.h"

ResourceManager::ResourceManager() = default;

void ResourceManager::loadTexture(const std::string &name, const std::string &path) {
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        (void)texture.loadFromFile("assets/missing_texture.png");//ca sa taca ca nu dau nimaniu return value
        throw std::runtime_error("Failed to load texture:" + path);
    }
    textures[name] = std::move(texture);
}

sf::Texture &ResourceManager::getTexture(const std::string &name) {
    const auto it = textures.find(name);
    if (it == textures.end()) {
        auto missing = textures.find("missing_texture");
        if (missing != textures.end()) {
            return missing->second;
        }
        throw std::runtime_error("Texture not found in assets:" + name); //mutat in T2
    }
    return it->second;
}

ResourceManager::~ResourceManager() {
    textures.clear();
}

std::ostream &operator<<(std::ostream &os, const ResourceManager &rm) {
    os << "[ResourceManager] texturi incarcate:" << rm.textures.size() << "\n";
    for (const auto &pair : rm.textures) {
        const auto &tex = pair.second;
        auto size = tex.getSize();
        os << "->Nume: " << pair.first << ", size : [" << size.x << "x" << size.y << "]\n";
    }
    return os;
}