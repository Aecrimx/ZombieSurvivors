//
// Created by Mircea on 13.12.2025.
//

#include "ResourceManager.h"
#include "ResourceLoadException.h"
#include <iostream>

ResourceManager::ResourceManager() = default;

void ResourceManager::ensureMissingTexture() {
    if (!hasTexture("missing_texture")) {
        sf::Texture missingTex;
        if (missingTex.loadFromFile("assets/missing_texture.png")) {
            textures["missing_texture"] = std::move(missingTex);
        }
    }
}

void ResourceManager::loadTexture(const std::string &name,
                                  const std::string &path) {
    ensureMissingTexture();

    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        if (hasTexture("missing_texture")) {
            textures[name] = textures["missing_texture"];
        }
        throw ResourceLoadException(path);
    }
    textures[name] = std::move(texture);
}

sf::Texture &ResourceManager::getTexture(const std::string &name) {
    ensureMissingTexture();

    const auto it = textures.find(name);
    if (it == textures.end()) {
        auto missing = textures.find("missing_texture");
        if (missing != textures.end()) {
            return missing->second;
        }
        throw ResourceLoadException("Texture not found: " + name);
    }
    return it->second;
}

bool ResourceManager::hasTexture(const std::string &name) const {
    return textures.find(name) != textures.end();
}

ResourceManager::~ResourceManager() { textures.clear(); }

std::ostream &operator<<(std::ostream &os, const ResourceManager &rm) {
    os << "[ResourceManager] texturi incarcate:" << rm.textures.size() << "\n";
    for (const auto &pair: rm.textures) {
        const auto &tex = pair.second;
        auto size = tex.getSize();
        os << "->Nume: " << pair.first << ", size : [" << size.x << "x" << size.y
                << "]\n";
    }
    return os;
}
