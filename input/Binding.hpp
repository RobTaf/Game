#ifndef Binding_hpp
#define Binding_hpp

#include <SFML/Graphics.hpp>

enum class EventType {
    KeyDown = sf::Event::KeyPressed,
    KeyUp = sf::Event::KeyReleased,
    MButtonDown = sf::Event::MouseButtonPressed,
    MButtonUp = sf::Event::MouseButtonReleased,
    MouseWheel = sf::Event::MouseWheelMoved,
    WindowResized = sf::Event::Resized,
    GainedFocus = sf::Event::GainedFocus,
    LostFocus = sf::Event::LostFocus,
    MouseEntered = sf::Event::MouseEntered,
    MouseLeft = sf::Event::MouseLeft,
    Closed = sf::Event::Closed,
    TextEntered = sf::Event::TextEntered,
    Keyboard = sf::Event::Count + 1, Mouse, Joystick
};

struct EventInfo {
    EventInfo() { m_code = 0; }
    EventInfo(int event) { m_code = event; }
    union {
        int m_code;
    };
};

struct EventDetails {
    EventDetails(const std::string& bindName) { 
        m_name = bindName;
        clear(); 
    }

    void clear() {
        m_size = sf::Vector2i(0, 0);
        m_textEntered = 0;
        m_mouse = sf::Vector2i(0, 0);
        m_mouseWheelDelta = 0;
        m_keyCode = -1;
    }

    std::string m_name;
    sf::Vector2i m_size;
    sf::Uint32 m_textEntered;
    sf::Vector2i m_mouse;
    int m_mouseWheelDelta;
    int m_keyCode;
};

using Events = std::vector<std::pair<EventType, EventInfo>>;

struct Binding {
    Binding(const std::string& name) 
        : m_name(name), m_details(name), m_count(0) {}

    void bindEvent(EventType type, EventInfo info = EventInfo()) {
        m_events.emplace_back(type, info);
    }

    Events m_events;
    EventDetails m_details;
    std::string m_name;
    int m_count;  
};

#endif