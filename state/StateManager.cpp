#include "StateManager.hpp"

StateManager::StateManager(SharedContext* shared) {
    m_shared = shared;
    registerState<IntroState>(StateType::Intro);
    registerState<MainMenuState>(StateType::MainMenu);
    registerState<GameState>(StateType::Game);
}

StateManager::~StateManager() {
    for (auto& itr : m_states) {
        itr.second->onDestroy();
        delete itr.second;
    }
}

void StateManager::update(const sf::Time &time) {
    if (m_states.empty()) return;
    if (m_states.back().second->isTranscendent() and m_states.size() > 1) {
        auto itr = m_states.end();
        while (itr != m_states.begin()) {
            if (itr != m_states.end()) {
                if (itr->second->isTranscendent())
                    break;
            }
            --itr;
        }
        for (; itr != m_states.end(); ++itr) {
            itr->second->update(time);
        }
    } else {
        m_states.back().second->update(time);
    }
}

void StateManager::draw() {
    if (m_states.empty()) return;
    if (m_states.back().second->isTransparent() and m_states.size() > 1) {
        auto itr = m_states.end();
        while (itr != m_states.begin()) {
            if (itr != m_states.end()) {
                if (itr->second->isTransparent())
                    break;
            }
            --itr;
        }
        for (; itr != m_states.end(); ++itr) {
            m_shared->m_window->getRenderWindow()->setView(itr->second->getView());
            itr->second->draw();
        }
    } else {
        m_states.back().second->draw();
    }
}

void StateManager::processRequests() {
    while (m_toRemove.begin() != m_toRemove.end()) {
        removeState(*m_toRemove.begin());
        m_toRemove.erase(m_toRemove.begin());
    }
}

bool StateManager::hasState(const StateType &type) {
    for (auto itr = m_states.begin(); itr != m_states.end(); ++itr) {
        if (itr->first == type) {
            auto removed = std::find(m_toRemove.begin(), m_toRemove.end(), type);
            if (removed == m_toRemove.end())
                return true;
            return false;
        }
    }
    return false;
}

void StateManager::switchTo(const StateType& type) {
    m_shared->m_eventManager->setCurrentState(type);
    for (auto itr = m_states.begin(); itr != m_states.end(); ++itr) {
        if (itr->first == type) {
            m_states.back().second->deactivate();
            StateType tempType = itr->first;
            BaseState* tempState = itr->second;
            m_states.erase(itr);
            m_states.emplace_back(tempType, tempState);
            m_shared->m_window->getRenderWindow()->setView(tempState->getView());
            return;
        }
    }

    // State with 'type' was not found
    if (not m_states.empty())
        m_states.back().second->deactivate();
    createState(type);
    m_states.back().second->activate();
    m_shared->m_window->getRenderWindow()->setView(m_states.back().second->getView());
}

void StateManager::remove(const StateType &type) {
    m_toRemove.push_back(type);
}

void StateManager::createState(const StateType &type) {
    auto newState = m_stateFactory.find(type);
    if (newState == m_stateFactory.end()) return;
    BaseState* state = newState->second();
    state->m_view = m_shared->m_window->getRenderWindow()->getDefaultView();
    m_states.emplace_back(type, state);
    state->onCreate();
}

void StateManager::removeState(const StateType &type) {
    for (auto itr = m_states.begin(); itr != m_states.end(); ++itr) {
        if (itr->first == type) {
            itr->second->onDestroy();
            delete itr->second;
            m_states.erase(itr);
            return;
        }
    }
}

SharedContext* StateManager::getContext() {
    return m_shared;
}