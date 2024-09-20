#ifndef TextureManager_hpp
#define TextureManager_hpp

#include <SFML/Graphics.hpp>

#include "ResourceManager.hpp"

class TextureManager: public ResourceManager<TextureManager, sf::Texture> {
public:
    TextureManager();
    sf::Texture* load(const std::string& path);
};

#endif