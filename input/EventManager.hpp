#ifndef EventManager_hpp
#define EventManager_hpp

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Binding.hpp"
#include "../utility/Utility.hpp"
#include "../state/StateType.hpp"

using Bindings = std::unordered_map<std::string, Binding*>;
// Having different states in a game will create situations where the same key or event
// will be needed by at least two of the states. Grouping the callbacks together by state
// and only invoking them if the current state is that of a callback.
// enum class StateType;
using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>;
using Callbacks = std::unordered_map<StateType, CallbackContainer>;

class EventManager {
public:
    EventManager();
    ~EventManager();

    bool addBinding(Binding* binding);
    bool removeBinding(std::string name);
    void setFocus(const bool& focus);

    template <class T>
    bool addCallback(StateType state, const std::string& name, void(T::*func)(EventDetails*), T* instance) {
        auto itr = m_callbacks.emplace(state, CallbackContainer()).first;
        auto temp = std::bind(func, instance, std::placeholders::_1);
        return itr->second.emplace(name, temp).second;
    }

    bool removeCallback(StateType state, const std::string& name);

    void handleEvent(sf::Event& event);
    void update();

    sf::Vector2i getMousePosition(sf::RenderWindow* window);

    void setCurrentState(StateType type);

private:
    void loadBindings();
    
    StateType m_currentState;
    Bindings m_bindings;
    Callbacks m_callbacks;
    bool m_hasFocus;
};

#endif