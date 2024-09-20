#include "SpriteSheet.hpp"

SpriteSheet::SpriteSheet(TextureManager* textureManager) {
    m_textureManager = textureManager;
    m_currentAnimation = nullptr;
    m_spriteScale = {1, 1};
    m_direction = Direction::Right;
}

SpriteSheet::~SpriteSheet() {
    releaseSheet();
}

bool SpriteSheet::loadSheet(const std::string& file) {
    std::ifstream sheet;
    sheet.open(Utility::workingDirectory() + "assets/sheets/" + file + ".sheet");
    if (sheet.is_open()) {
        releaseSheet();
        std::string line;
        while (std::getline(sheet, line)) {
            if (line[0] == '|')
                continue;
            std::stringstream keystream(line);
            std::string type;
            keystream >> type;
            // ...
            if (type == "Texture") {
                if (m_texture != "") {
                    std::cerr << "! Duplicate texture entries in: " << file << std::endl;
                    continue;
                } else {
                    std::string texture;
                    keystream >> texture;
                    if (not m_textureManager->requireResource(texture)) {
                        std::cerr << "! Could not setup the texture: " << texture << std::endl;
                        continue; 
                    } else {
                        m_texture = texture;
                        m_sprite.setTexture(*m_textureManager->getResource(m_texture));
                    }
                }
            } else if (type == "Size") {
                keystream >> m_spriteSize.x >> m_spriteSize.y;
                setSpriteSize(m_spriteSize);
            } else if (type == "Scale") {
                keystream >> m_spriteScale.x >> m_spriteScale.y;
                m_sprite.setScale(m_spriteScale);
            } else if (type == "AnimationType") {
                keystream >> m_animationType;
            } else if (type == "Animation") {
                std::string name;
                keystream >> name;
                if (m_animations.find(name) != m_animations.end()) {
                    std::cerr << "! Duplicate animation( " << name << ") in: " << file << std::endl;
                    continue;
                }
                BaseAnimation* animation = nullptr;
                if (m_animationType == "Directional") {
                    animation = new DirectionalAnimation();
                } else {
                    std::cerr << "! Unknown animation type: " << m_animationType << std::endl;
                    continue;
                }
                keystream >> *animation;
                animation->setSpriteSheet(this);
                animation->setName(name);
                animation->reset();
                m_animations.emplace(name, animation);

                if (m_currentAnimation) { continue; }
                m_currentAnimation = animation;
                m_currentAnimation->play();
            }
        }
        sheet.close();
        return true;
    } else {
        std::cerr << "! Failed loading spritesheet: " << file << std::endl;
        return false;
    }
}

void SpriteSheet::releaseSheet() {
    m_textureManager->releaseResource(m_texture);
    m_currentAnimation = nullptr;
    while (m_animations.begin() != m_animations.end()) {
        delete m_animations.begin()->second;
        m_animations.erase(m_animations.begin());
    }
}

void SpriteSheet::setSpriteSize(const sf::Vector2i& size) {
    m_spriteSize = size;
    m_sprite.setOrigin(m_spriteSize.x / 2, m_spriteSize.y / 2);
}

sf::Vector2i SpriteSheet::getSpriteSize() {
    return m_spriteSize;
}

void SpriteSheet::setSpritePosition(const sf::Vector2f& position) {
    m_sprite.setPosition(position);
}

void SpriteSheet::setSpriteDirection(const Direction& direction) {
    if (direction == m_direction) 
        return;
    m_direction = direction;
    m_currentAnimation->cropSprite();
}

Direction SpriteSheet::getSpriteDirection() {
    return m_direction;
}

void SpriteSheet::cropSprite(const sf::IntRect& rect) {
    // Select sprite by passing the top-left corner
    m_sprite.setTextureRect(rect);
}

BaseAnimation* SpriteSheet::getCurrentAnimation() {
    return m_currentAnimation;
}

bool SpriteSheet::setAnimation(const std::string& name, const bool& play, const bool& loop) {
    auto itr = m_animations.find(name);
    if (itr == m_animations.end())
        return false;
    if (itr->second == m_currentAnimation)
        return false;
    if (m_currentAnimation)
        m_currentAnimation->stop();
    m_currentAnimation = itr->second;
    m_currentAnimation->setLoop(loop);
    if (play)
        m_currentAnimation->play();
    m_currentAnimation->cropSprite();
    return true;
}

void SpriteSheet::update(const float& deltaTime) {
    m_currentAnimation->update(deltaTime);
}

void SpriteSheet::draw(sf::RenderWindow* window) {
    window->draw(m_sprite);
}