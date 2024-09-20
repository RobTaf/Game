#include "EventManager.hpp"

EventManager::EventManager() {
    m_hasFocus = true;
    loadBindings();
}

EventManager::~EventManager() {
    for (auto& itr : m_bindings) {
        delete itr.second;
        itr.second = nullptr;
    }
}

bool EventManager::addBinding(Binding* binding) {
    if (m_bindings.find(binding->m_name) != m_bindings.end())
        return false;
    return m_bindings.emplace(binding->m_name, binding).second;
}

bool EventManager::removeBinding(std::string name) {
    auto itr = m_bindings.find(name);
    if (itr == m_bindings.end())
        return false;
    delete itr->second;
    m_bindings.erase(itr);
    return true;
}

void EventManager::setFocus(const bool &focus) {
    m_hasFocus = focus;
}

bool EventManager::removeCallback(StateType state, const std::string &name) {
    auto itr = m_callbacks.find(state);
    if (itr == m_callbacks.end()) return false;
    auto itr2 = itr->second.find(name);
    if (itr2 == itr->second.end()) return false;
    itr->second.erase(name);
    return true;
}

void EventManager::handleEvent(sf::Event &event) {
    for (auto& b_itr : m_bindings) {
        Binding* currentBinding = b_itr.second;
        for (auto& e_itr : currentBinding->m_events) {
            EventType sfmlEvent = (EventType)event.type;
            if (e_itr.first != sfmlEvent) { continue; }
            if (sfmlEvent == EventType::KeyDown or sfmlEvent == EventType::KeyUp) {
                if (e_itr.second.m_code == event.key.code) {
                    // Matching event/keystroke, increase count
                    if (currentBinding->m_details.m_keyCode != -1) {
                        currentBinding->m_details.m_keyCode = e_itr.second.m_code;
                    }
                    std::cout << "Key pressed" << std::endl;
                    ++(currentBinding->m_count);
                    break;
                }
            } else if (sfmlEvent == EventType::MButtonDown or sfmlEvent == EventType::MButtonUp) {
                if (e_itr.second.m_code == event.mouseButton.button) {
                    // Matching event/keystroke, increase count
                    currentBinding->m_details.m_mouse.x = event.mouseButton.x;
                    currentBinding->m_details.m_mouse.y = event.mouseButton.y;
                    if (currentBinding->m_details.m_keyCode != -1) {
                        currentBinding->m_details.m_keyCode = e_itr.second.m_code;
                    }
                    std::cout << "Button pressed" << std::endl;
                    ++(currentBinding->m_count);
                    break;
                }
            } else {
                if (sfmlEvent == EventType::MouseWheel) {
                    currentBinding->m_details.m_mouseWheelDelta = event.mouseWheel.delta;
                    std::cout << "Mouse wheel rotated" << std::endl;
                    std::cout << currentBinding->m_details.m_mouseWheelDelta << std::endl;
                } else if (sfmlEvent == EventType::WindowResized) {
                    currentBinding->m_details.m_size.x = event.size.width;
                    currentBinding->m_details.m_size.y = event.size.height;
                } else if (sfmlEvent == EventType::TextEntered) {
                    currentBinding->m_details.m_textEntered = event.text.unicode;
                }
                ++(currentBinding->m_count);
            }
        }
    }
}

void EventManager::update() {
    if (not m_hasFocus) return;
    for (auto& b_itr : m_bindings) {
        Binding* currentBinding = b_itr.second;
        for (auto& e_itr : currentBinding->m_events) {
            switch (e_itr.first) {
            case (EventType::Keyboard):
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(e_itr.second.m_code))) {
                    if (currentBinding->m_details.m_keyCode != -1) {
                        currentBinding->m_details.m_keyCode = e_itr.second.m_code;
                    }
                    ++(currentBinding->m_count);
                    std::cout << "Key pressed" << std::endl;
                }
                break;
            case(EventType::Mouse):
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button(e_itr.second.m_code))) {
                    if (currentBinding->m_details.m_keyCode != -1) {
                        currentBinding->m_details.m_keyCode = e_itr.second.m_code;
                    }
                    ++(currentBinding->m_count);
                    std::cout << "Mouse Button pressed" << std::endl;
                }
                break;
            case(EventType::Joystick):
            // Up for expansion.
                break;
            }
        }

        if (currentBinding->m_events.size() == currentBinding->m_count) {
            // Callbacks of the current state
            auto stateCallbacks = m_callbacks.find(m_currentState);
            // Global callbacks. Anything with the state type 0 will be invoked
            // regardless of which state we are in.
            auto otherCallbacks = m_callbacks.find(StateType(0));
            if (stateCallbacks != m_callbacks.end()) {
                auto callItr = stateCallbacks->second.find(currentBinding->m_name);
                if (callItr != stateCallbacks->second.end()) {
                    // Pass in information about the events.
                    callItr->second(&currentBinding->m_details);
                }
            }
            if (otherCallbacks != m_callbacks.end()) {
                auto callItr = otherCallbacks->second.find(currentBinding->m_name);
                if (callItr != otherCallbacks->second.end()) {
                    // Pass in information about events.
                    callItr->second(&currentBinding->m_details);
                }
            }
        }
        currentBinding->m_count = 0;
        currentBinding->m_details.clear();
    }
}

sf::Vector2i EventManager::getMousePosition(sf::RenderWindow* window = nullptr) {
    return (window ? sf::Mouse::getPosition(*window) : sf::Mouse::getPosition());
}

void EventManager::loadBindings() {
    std::string delimiter = ":";
    std::ifstream bindings;
    bindings.open(Utility::workingDirectory() + "assets/config/keys.cfg");
    if (not bindings.is_open()) {
        std::cout << "! Failed loading keys.cfg." << std::endl;
        return;
    }
    std::string line;
    while (std::getline(bindings, line)) {
        std::stringstream keystream(line);
        std::string callbackName;
        keystream >> callbackName;
        Binding* bind = new Binding(callbackName);
        while (!keystream.eof()) {
            std::string keyval;
            keystream >> keyval;
            int start = 0;
            int end = keyval.find(delimiter);
            if (end == std::string::npos) {
                delete bind;
                bind = nullptr;
                break;
            }
            EventType type = EventType(stoi(keyval.substr(start, end - start)));
            int code = stoi(keyval.substr(end + delimiter.length(),
            keyval.find(delimiter, end + delimiter.length())));
            EventInfo eventInfo;
            eventInfo.m_code = code;
            bind->bindEvent(type, eventInfo);
        }
        if (!addBinding(bind)) { delete bind; }
        bind = nullptr;
    }
    bindings.close();
}

void EventManager::setCurrentState(StateType type) {
    m_currentState = type;
}
