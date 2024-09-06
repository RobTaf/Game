#ifndef SharedContext_hpp
#define SharedContext_hpp

#include "../input/EventManager.hpp"
#include "../Window.hpp"

struct SharedContext {
    SharedContext() :
        m_window(nullptr), m_eventManager(nullptr) {}

    Window* m_window;
    EventManager* m_eventManager;
};

#endif