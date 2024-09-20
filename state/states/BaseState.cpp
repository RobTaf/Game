#include "BaseState.hpp"

BaseState::BaseState(StateManager* stateManager) {
    m_stateManager = stateManager;
    m_transparent = false;
    m_transcendent = false;
}

BaseState::~BaseState()
{
}

void BaseState::setTransparent(const bool &transparent) {
    m_transparent = transparent;
}

bool BaseState::isTransparent() {
    return m_transparent;
}

void BaseState::setTranscendent(const bool& transcendent) {
    m_transcendent = transcendent;
}

bool BaseState::isTranscendent() {
    return m_transcendent;
}

StateManager* BaseState::getStateManager() {
    return m_stateManager;
}

sf::View &BaseState::getView() {
    return m_view;
}
