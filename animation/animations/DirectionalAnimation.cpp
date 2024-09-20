#include "DirectionalAnimation.hpp"
#include "../spritesheet/SpriteSheet.hpp"

void DirectionalAnimation::frameStep() {
    // Still animating?
    if (m_startFrame < m_endFrame) { ++m_currentFrame; } else { --m_currentFrame; }
    // Reached the end?
    if ((m_startFrame < m_endFrame and m_currentFrame > m_endFrame) or (m_startFrame > m_endFrame and m_currentFrame < m_endFrame)) {
        if (m_loop) {
            m_currentFrame = m_startFrame;
            return;
        } else {
            m_currentFrame = m_endFrame;
            pause();
        }
    }
}

void DirectionalAnimation::cropSprite() {
    // Crop sprite by passing position and size
    sf::IntRect rect(
        m_spriteSheet->getSpriteSize().x * m_currentFrame, 
        m_spriteSheet->getSpriteSize().y * (m_rowFrame + (short)m_spriteSheet->getSpriteDirection()),
        m_spriteSheet->getSpriteSize().x,
        m_spriteSheet->getSpriteSize().y
    );
    m_spriteSheet->cropSprite(rect);
}

void DirectionalAnimation::readIn(std::stringstream &stream) {
    stream >> m_startFrame >> m_endFrame >> m_rowFrame >> m_timeFrame >> m_actionStartFrame >> m_actionEndFrame;
}
