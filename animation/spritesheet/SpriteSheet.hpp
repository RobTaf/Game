#ifndef SpriteSheet_hpp
#define SpriteSheet_hpp

#include "../../resource/TextureManager.hpp"
#include "../animations/BaseAnimation.hpp"
#include "../animations/DirectionalAnimation.hpp"
#include "Direction.hpp"

using Animations = std::unordered_map<std::string, BaseAnimation*>;

/// @brief This class serves as a container of sprites making resource
/// management more efficient. Each sprite is a 32 by 32 px in size.
class SpriteSheet {
public:
    SpriteSheet(TextureManager* textureManager);
    ~SpriteSheet();

    bool loadSheet(const std::string& file);
    void releaseSheet();

    void setSpriteSize(const sf::Vector2i& size);
    sf::Vector2i getSpriteSize();

    void setSpritePosition(const sf::Vector2f& position);

    void setSpriteDirection(const Direction& direction);
    Direction getSpriteDirection();

    BaseAnimation* getCurrentAnimation();
    bool setAnimation(const std::string& name, const bool& play, const bool& loop);

    void cropSprite(const sf::IntRect& rect);
    void update(const float& deltaTime);
    void draw(sf::RenderWindow* window);

private:
    std::string m_texture;
    sf::Sprite m_sprite;
    sf::Vector2i m_spriteSize;
    sf::Vector2f m_spriteScale;
    Direction m_direction;
    std::string m_animationType;
    Animations m_animations;
    BaseAnimation* m_currentAnimation;
    TextureManager* m_textureManager;
};

#endif