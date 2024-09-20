#include "EntityManager.hpp"

EntityManager::EntityManager(SystemManager* systemManager, TextureManager* textureManager) 
    : m_idCounter(0), m_systemManager(systemManager), m_textureManager(textureManager) {
        registerComponent<Transform>(Component::Transform);
        // register components
}

// EntityManager::EntityManager(SharedContext *context, unsigned int maxEntities)
// {
//     m_context = context;
//     m_maxEntities = maxEntities;
//     m_idCounter = 0;

//     loadEnemyTypes("EnemyList.list");
//     registerEntity<Player>(EntityType::Player);
//     registerEntity<Enemy>(EntityType::Enemy);
// }

EntityManager::~EntityManager() {
    purge();
}

int EntityManager::addEntity(const Bitmask& mask) {
    EntityId id = m_idCounter;
    EntityData data = std::make_pair(Bitmask(), ComponentContainer());

    // If the insertion is successful
    if (not m_entities.emplace(id, data).second) return -1;
    ++ m_idCounter;

    // Add enabled components
    for (unsigned int i = 0; i < N_COMPONENT_TYPES; ++i) {
        if (mask.getBit(i)) {
            addComponent(id, (Component)i);
        }
    }

    // Notifying the system manager of a modified entity
    // m_systemManager->entityModified(entity, mask);
    // m_systemManager->addEvent(entity, (EventID)EntityEvent::Spawned);
    return id;
}

int EntityManager::addEntity(const std::string& entityFile) {
    EntityId id = -1;

    std::ifstream file;
    file.open(Utility::workingDirectory() + "assets/entities/" + entityFile + ".entity");
    if (not file.is_open()) {
        std::cout << "! Failed to load entity: " << entityFile << std::endl;
        return -1;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line[0] == '|') continue;
        std::stringstream keystream(line);
        std::string type;
        if (type == "Name") {

        } else if (type == "Attributes") {
            if (id != -1) continue;
            Bitset bits = 0;
            Bitmask mask;
            keystream >> bits;
            mask.setMask(bits);
            id = addEntity(mask);
            if (id == -1) return;
        } else if (type == "Component") {
            if (id == -1) continue;
            unsigned int componentType = 0;
            keystream >> componentType;
            BaseComponent* component = getComponent<BaseComponent>(id, (Component)componentType);
            if (not component) continue;
            keystream >> *component;
            if (component->getType() == Component::SpriteSheet) {
                // SpriteSheet* sheet = (SpriteSheet*)component;
                // sheet->create(m_textureManager);
            } else if (component->getType() == Component::Transform) {
                // ...
            }
        }
    }
    file.close();
    return id;
}

bool EntityManager::removeEntity(const EntityId &id) {
    auto itr = m_entities.find(id);
    if (itr == m_entities.end()) return false;
    // Removing all components
    while (itr->second.second.begin() != itr->second.second.end()) {
        delete itr->second.second.back();
        itr->second.second.pop_back();
    }
    m_entities.erase(id);
    // m_systemManager->removeEntity(id);
    return true;

}

bool EntityManager::addComponent(const EntityId& id, const Component& componentType) {
    auto itr = m_entities.find(id);
    // If the entity is not found
    if (itr == m_entities.end()) return false;
    // If the entity already has the component
    if (itr->second.first.getBit( (unsigned int)componentType )) return false;
    // If the component type does not exist
    auto compItr = m_componentFactory.find(componentType);
    if (compItr == m_componentFactory.end()) return false;
    // Component type does exists, create a new instance of the component
    BaseComponent* component = compItr->second();
    itr->second.second.emplace_back(component);
    itr->second.first.turnOnBit( (unsigned int)componentType );
    // Notifying the system manager of a modified entity
    // m_systemManager->entityModified(id, itr->second.first);
    return true;
}

bool EntityManager::removeComponent(const EntityId &id, const Component &componentType) {
    auto itr = m_entities.find(id);
    // If the entity is not found
    if (itr == m_entities.end()) return false;
    // If the entity does not have the component
    if (not itr->second.first.getBit( (unsigned int)componentType )) return false;
    // Component type does exist, remove it
    auto& container = itr->second.second;
    auto component = std::find(container.begin(), container.end(), [&componentType](BaseComponent* c) {
        return c->getType() == componentType;
    });
    if (component == container.end()) return false;
    delete *component;
    container.erase(component);
    itr->second.first.clearBit( (unsigned int)componentType );

    // Notifying the system manager of a modified entity
    // m_systemManager->entityModified(id, itr->second.first);
    return true;
}

bool EntityManager::hasComponent(const EntityId& id, const Component& componentType) {
    auto itr = m_entities.find(id);
    // If the entity is not found
    if (itr == m_entities.end()) return false;
    return itr->second.first.getBit( (unsigned int)componentType );
}

void EntityManager::purge() {
    // m_systemManager->purgeEntities();
    for (auto& entity : m_entities) {
        for (auto& component : entity.second.second)
            delete component;
        entity.second.second.clear();
        entity.second.first.clear();
    }
    m_entities.clear();
    m_idCounter = 0;
}

// SharedContext *EntityManager::getContext() {
//     return m_context;
// }

// std::size_t EntityManager::size() {
//     return m_entities.size();
// }

// int EntityManager::add(const EntityType &type, const std::string &name) {
//     auto itr = m_entityFactory.find(type);
//     if (itr == m_entityFactory.end()) return -1;
//     BaseEntity* entity = itr->second();
//     entity->m_id = m_idCounter;
//     if (name != "") { entity->m_name = name; }
//     m_entities.emplace(m_idCounter, entity);
//     if (type == EntityType::Enemy) {
//         auto itr = m_enemyTypes.find(name);
//         if (itr != m_enemyTypes.end()) {
//             Enemy* enemy = (Enemy*)entity;
//             enemy->load(itr->second);
//         }
//     }
//     ++m_idCounter;
//     return m_idCounter - 1;
// }

// BaseEntity *EntityManager::find(const std::string& name) {
//     for (auto itr : m_entities) {
//         if (itr.second->m_name == name) {
//             return itr.second;
//         }
//     }
//     return nullptr;
// }

// BaseEntity *EntityManager::find(unsigned int id) {
//     auto itr = m_entities.find(id);
//     return itr != m_entities.end() ? itr->second : nullptr; 
// }

// void EntityManager::remove(unsigned int id) {
//     m_entitiesToRemove.emplace_back(id);
// }

// void EntityManager::purge() {
//     for (auto& itr : m_entities) {
//         delete itr.second;
//     }
//     m_entities.clear();
//     m_idCounter = 0;
// }

// void EntityManager::update(float deltaTime) {
//     for (auto& itr : m_entities) {
//         itr.second->update(deltaTime);
//         entityCollisionCheck();
//         processRemovals();
//     }
// }

// void EntityManager::draw() {
//     sf::RenderWindow* window = m_context->m_window->getRenderWindow();
//     sf::FloatRect viewSpace = m_context->m_window->viewSpace();

//     for (auto& itr : m_entities) {
//         if (not viewSpace.intersects(itr.second->m_AABB))
//             continue;
//         itr.second->draw(window);
//     }
// }

// void EntityManager::processRemovals() {
//     while (m_entitiesToRemove.begin() != m_entitiesToRemove.end()) {
//         unsigned int id = m_entitiesToRemove.back();
//         auto itr = m_entities.find(id);
//         if (itr != m_entities.end()) {
//             std::cout << "Discarding entity " << itr->second->getId() << std::endl;
//             delete itr->second;
//             m_entities.erase(itr);
//         }
//         m_entitiesToRemove.pop_back();
//     }
// }

// void EntityManager::loadEnemyTypes(const std::string &name) {
//     std::ifstream file;
//     file.open(Utility::workingDirectory() + "assets/lists/" + name);
//     if (not file.is_open()) {
//         std::cout << "! Failed loading enemy list file: " << name << std::endl;
//         return;
//     }
//     std::string line;
//     while (std::getline(file, line)) {
//         if (line[0] == '|') continue;
//         std::stringstream keystream(line);
//         std::string name;
//         std::string charFile;
//         keystream >> name >> charFile;
//         m_enemyTypes.emplace(name, charFile);
//     }
//     file.close();
// }

// void EntityManager::entityCollisionCheck() {
//     if (m_entities.empty()) return;
//     for (auto curr = m_entities.begin(); curr != m_entities.end(); ++curr) {
//         for (auto succ = std::next(curr); succ != m_entities.end(); ++succ) {
//             if (curr->first == succ->first) continue;

//             // Regular AABB bounding box collision
//             if (curr->second->m_AABB.intersects(succ->second->m_AABB)) {
//                 curr->second->onEntityCollision(curr->second, false);
//                 succ->second->onEntityCollision(succ->second, false);
//             }
            
//             EntityType type1 = curr->second->getType();
//             EntityType type2 = succ->second->getType();

//             if (type1 == EntityType::Player or type1 == EntityType::Enemy) {
//                 Character* char1 = (Character*)curr->second;
//                 if (char1->m_attackAABB.intersects(succ->second->m_AABB)) {
//                     char1->onEntityCollision(succ->second, true);
//                 }
//             }

//             if (type2 == EntityType::Player or type2 == EntityType::Enemy) {
//                 Character* char2 = (Character*)succ->second;
//                 if (char2->m_attackAABB.intersects(curr->second->m_AABB)) {
//                     char2->onEntityCollision(curr->second, true);
//                 }
//             }
//         }
//     }
// }
