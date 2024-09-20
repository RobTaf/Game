#include "TextureManager.hpp"

TextureManager::TextureManager() : ResourceManager("assets/config/textures.cfg") {}

sf::Texture* TextureManager::load(const std::string& path) {
    sf::Texture* texture = new sf::Texture();
    if (not texture->loadFromFile(Utility::workingDirectory() + path)) {
        delete texture;
        texture = nullptr;
        std::cerr << "! Failed to load textures: " << path << std::endl;
    }
    return texture;
}