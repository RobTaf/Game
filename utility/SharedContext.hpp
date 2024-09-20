#ifndef SharedContext_hpp
#define SharedContext_hpp

#include "../map/Map.hpp"
#include "../Window.hpp"
#include "../input/EventManager.hpp"
#include "../resource/TextureManager.hpp"
#include "../entity/EntityManager.hpp"

// class Map;
// class Window;
// class EventManager;
// class TextureManager;
class EntityManager;

struct SharedContext {
    SharedContext() :
        m_window(nullptr), 
        m_eventManager(nullptr), 
        m_textureManager(nullptr),
        m_gameMap(nullptr),
        m_entityManager(nullptr) {}

    Window* m_window;
    EventManager* m_eventManager;
    TextureManager* m_textureManager;
    EntityManager* m_entityManager;
    Map* m_gameMap;
};

#endif