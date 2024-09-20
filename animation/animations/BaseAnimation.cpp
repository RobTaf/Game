#include "BaseAnimation.hpp"
#include "../spritesheet/SpriteSheet.hpp"

BaseAnimation::BaseAnimation() {
    m_currentFrame = 0;
    m_startFrame = 0;
    m_endFrame = 0;
    m_rowFrame = 0;
    m_actionStartFrame = -1;
    m_actionEndFrame = -1;
    m_loop = false;
    m_playing = false;
}

BaseAnimation::~BaseAnimation()
{
}

void BaseAnimation::play() {
    m_playing = true;
}

void BaseAnimation::pause() {
    m_playing = false;
}

void BaseAnimation::stop() {
    m_playing = false;
    reset();
}

void BaseAnimation::reset() {
    m_currentFrame = m_startFrame;
    m_elapsedTime = 0;
    cropSprite();
}

bool BaseAnimation::isInAction() {
    if (m_actionStartFrame == -1 or m_actionEndFrame == -1)
        return true;
    return (m_currentFrame >= m_actionStartFrame and m_currentFrame <= m_actionEndFrame);
}

void BaseAnimation::update(const float& deltaTime) {
    if (not m_playing)
        return;
    m_elapsedTime += deltaTime;
    if (m_elapsedTime < m_timeFrame)
        return;
    if (m_elapsedTime >= m_timeFrame) {
        frameStep();
        cropSprite();
        m_elapsedTime = 0;
    }
}

void BaseAnimation::setSpriteSheet(SpriteSheet* sheet) {
    m_spriteSheet = sheet;
}

SpriteSheet *BaseAnimation::getSpriteSheet() {
    return m_spriteSheet;
}

void BaseAnimation::setName(const std::string& name) {
    m_name = name;
}

std::string BaseAnimation::getName() {
    return m_name;
}

void BaseAnimation::setFrame(const Frame& frame) {
    if ((frame >= m_startFrame and frame <= m_endFrame) or (frame >= m_endFrame and frame <= m_startFrame)) {
        m_currentFrame = frame;
    }
}

Frame BaseAnimation::getFrame() {
    return m_currentFrame;
}

void BaseAnimation::setStartFrame(const Frame& frame) {
    m_startFrame = frame;
}

Frame BaseAnimation::getStartFrame() {
    return m_startFrame;
}

void BaseAnimation::setEndFrame(const Frame& frame) {
    m_endFrame = frame;
}

Frame BaseAnimation::getEndFrame() {
    return m_endFrame;
}

void BaseAnimation::setRowFrame(const Frame& frame) {
    m_rowFrame = frame;
}

Frame BaseAnimation::getRowFrame() {
    return m_rowFrame;
}

void BaseAnimation::setStartActionFrame(int& frame) {
    m_actionStartFrame = frame;
}

int BaseAnimation::getStartActionFrame() {
    return m_actionStartFrame;
}

void BaseAnimation::setEndActionFrame(int& frame) {
    m_actionEndFrame = frame;
}

int BaseAnimation::getEndActionFrame() {
    return m_actionEndFrame;
}

void BaseAnimation::setTimeFrame(float& timeFrame) {
    m_timeFrame = timeFrame;
}

float BaseAnimation::getTimeFrame() {
    return m_timeFrame;
}

void BaseAnimation::setElapsedTime(float& elapsedTime) {
    m_elapsedTime = elapsedTime;
}

float BaseAnimation::getElapsedTime() {
    return m_elapsedTime;
}

void BaseAnimation::setLoop(const bool& loop) {
    m_loop = loop;
}

bool BaseAnimation::isLooping() {
    return m_loop;
}

void BaseAnimation::setPlaying(const bool& playing) {
    m_playing = playing;
}

bool BaseAnimation::isPlaying() {
    return m_playing;
}
