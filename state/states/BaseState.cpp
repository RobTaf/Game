#include "BaseState.hpp"

BaseState::~BaseState()
{
}

void BaseState::setTransparent(const bool &transparent)
{
    m_transparent = transparent;
}

bool BaseState::isTransparent() {
    return m_transparent;
}

void BaseState::setTranscendent(const bool& transcendent) {
    m_transcendent = transcendent;
}

bool BaseState::isTranscendent()
{
    return m_transcendent;
}