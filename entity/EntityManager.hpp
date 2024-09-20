#ifndef EntityManager_hpp
#define EntityManager_hpp

#include "entities/characters/Player.hpp"
#include "entities/characters/Enemy.hpp"

#include "../component/bitmask/Bitmask.hpp"
#include "../component/Transform.hpp"
// ...

using ComponentContainer = std::vector<BaseComponent*>;

using EntityID = unsigned int;
using EntityData = std::pair<Bitmask, ComponentContainer>;
using EntityContainer = std::unordered_map<EntityID, EntityData>;


using EntityFactory = std::unordered_map<EntityType, std::function<BaseEntity*(void)>>;
using ComponentFactory = std::unordered_map<Component, std::function<BaseComponent*(void)>>;

// using EnemyTypes = std::unordered_map<std::string, std::string>;

class SystemManager;
class EntityManager {
public:
    EntityManager(SystemManager* systemManager, TextureManager* TextureManager);
    // EntityManager(SharedContext* context, unsigned int maxEntities);
    ~EntityManager();

    int addEntity(const Bitmask& mask);
    int addEntity(const std::string& entityFile);
    bool removeEntity(const EntityID& id);

    bool addComponent(const EntityID& id, const Component& component);
    bool removeComponent(const EntityID& id, const Component& component);
    bool hasComponent(const EntityID& id, const Component& component);

    template <class T>
    T* getComponent(const EntityID& id, const Component& component) {
        auto itr = m_entities.find(id);
        if (itr == m_entities.end()) return nullptr;
        // Found the entity
        if (itr->second.first.getBit(( unsigned int )component) == 0) return nullptr;
        // Found the component
        auto& container = itr->second.second;
        BaseComponent* toReturn = std::find(container.begin(), container.end(), [&component](BaseComponent* c) {
            return c->getType() == component;
        });
        return (toReturn != container.end() ? dynamic_cast<T*>(*toReturn) : nullptr);
    }

    void purge();

    // int add(const EntityType& type, const std::string& name = "");
    // BaseEntity* find(const std::string& name);
    // BaseEntity* find(unsigned int id);
    // void remove(unsigned int id);
    // void purge();

    void update(float deltaTime);
    void draw();

private:
    template <class T>
    void registerEntity(const EntityType& type) {
        m_entityFactory[type] = [this]() -> BaseEntity* {
            return new T(this);
        };
    }

    template <class T>
    void registerComponent(const Component& type) {
        m_componentFactory[type] = []() -> BaseComponent* {
            return new T();
        };
     }

    // void processRemovals();
    // void loadEnemyTypes(const std::string& name);
    // void entityCollisionCheck();

    ComponentFactory m_componentFactory;
    EntityFactory m_entityFactory;
    EntityContainer m_entities;
    // EnemyTypes m_enemyTypes;
    // SharedContext* m_context;
    unsigned int m_idCounter;
    unsigned int m_maxEntities;

    // std::vector<EntityID> m_entitiesToRemove;

    SystemManager* m_systemManager;
    TextureManager* m_textureManager;
};

#endif