#include "BaseEntity.hpp"
#include "../EntityManager.hpp"

BaseEntity::BaseEntity(EntityManager* entityManager) {
    m_entityManager = entityManager;
    m_id = 0;
    m_name = "Base Entity";
    m_type = EntityType::Base;
    m_state = EntityState::Idle;
    m_tileUnderneath = nullptr;
    m_isCollidingOnX = false;
    m_isCollidingOnY = false;
}

BaseEntity::~BaseEntity()
{
}

void BaseEntity::move(float x, float y) {
    m_positionOld = m_position;
    m_position += sf::Vector2f(x, y);

    sf::Vector2u mapSize = m_entityManager->getContext()->m_gameMap->getMapSize();
    if (m_position.x < 0) {
        m_position.x = 0;
    } else if (m_position.x > (mapSize.x + 1) * Sheet::TileSize) {
        m_position.x = (mapSize.x + 1) * Sheet::TileSize;
    }

    if (m_position.y < 0) {
        m_position.y = 0;
    } else if (m_position.y > (mapSize.y + 1) * Sheet::TileSize) {
        m_position.y = (mapSize.y + 1) * Sheet::TileSize;
        setState(EntityState::Dying);
    }
    updateAABB();
}

void BaseEntity::addVelocity(float x, float y) {
    m_velocity += sf::Vector2f(x, y);
    if (abs(m_velocity.x) > m_velocityMax.x) {
        if (m_velocity.x < 0) {
            m_velocity.x = -m_velocityMax.x;
        } else {
            m_velocity.x = m_velocityMax.x;
        }
    }

    if (abs(m_velocity.y) > m_velocityMax.y) {
        if (m_velocity.y < 0) {
            m_velocity.y = -m_velocityMax.y;
        } else {
            m_velocity.y = m_velocityMax.y;
        }
    }
}

void BaseEntity::accelerate(float x, float y) {
    m_acceleration += sf::Vector2f(x, y);
}

void BaseEntity::applyFriction(float x, float y) {
    if (m_velocity.x != 0) {
        if (abs(m_velocity.x) - abs(x) < 0) {
            m_velocity.x = 0;
        } else {
            if (m_velocity.x < 0) {
                m_velocity.x += x;
            } else {
                m_velocity.x -= x;
            }
        }
    }

    if (m_velocity.y != 0) {
        if (abs(m_velocity.y) - abs(y) < 0) {
            m_velocity.y = 0;
        } else {
            if (m_velocity.y < 0) {
                m_velocity.y += y;
            } else {
                m_velocity.y -= y;
            }
        }
    }
}

void BaseEntity::update(float deltaTime) {
    Map* map = m_entityManager->getContext()->m_gameMap;
    float gravity = map->getGravity();
    accelerate(0, gravity);
    addVelocity(m_acceleration.x * deltaTime, m_acceleration.y * deltaTime);
    setAcceleration(0, 0);

    sf::Vector2f friction;
    if (m_tileUnderneath) {
        friction = m_tileUnderneath->m_friction;
        if (m_tileUnderneath->m_deadly) {
            setState(EntityState::Dying);
        }
    } else if (map->getDefaultTile()) {
        friction = map->getDefaultTile()->m_friction;
    } else {
        friction = m_friction;
    }
    float frictionX = (m_speed.x * friction.x) * deltaTime;
    float frictionY = (m_speed.y * friction.y) * deltaTime;
    applyFriction(frictionX, frictionY);
    sf::Vector2f deltaPos = m_velocity * deltaTime;
    move(deltaPos.x, deltaPos.y);
    m_isCollidingOnX = false;
    m_isCollidingOnY = false;
    checkCollisions();
    resolveCollisions();
}

void BaseEntity::updateAABB() {
    m_AABB = sf::FloatRect(m_position.x - (m_boxSize.x / 2), m_position.y - m_boxSize.y, m_boxSize.x, m_boxSize.y);
}

void BaseEntity::checkCollisions() {
    Map* gameMap = m_entityManager->getContext()->m_gameMap;
    unsigned int tileSize = gameMap->getTileSize();
    int fromX = floor(m_AABB.left / tileSize);
    int toX = floor(m_AABB.left + m_AABB.width / tileSize);
    int fromY = floor(m_AABB.top / tileSize);
    int toY = floor(m_AABB.top + m_AABB.height / tileSize);

    for (int x = fromX; x <= toX; x++) {
        for (int y = fromY; y <= toY; y++) {
            Tile* tile = gameMap->getTile(x, y);
            if (not tile) continue;
            sf::FloatRect tileBounds(x * tileSize, y * tileSize, tileSize, tileSize);
            sf::FloatRect intersection;
            m_AABB.intersects(tileBounds, intersection);
            float area = intersection.width * intersection.height;

            CollisionElement element(area, tile->m_properties, tileBounds);
            m_collisions.emplace_back(element);
            if (tile->m_warp and m_type == EntityType::Player) {
                gameMap->loadNext();
            }
        }
    }
}

void BaseEntity::resolveCollisions() {
    if (not m_collisions.empty()) {
        std::sort(m_collisions.begin(), m_collisions.end(), sortCollisions);
        Map* gameMap= m_entityManager->getContext()->m_gameMap;
        unsigned int tileSize = gameMap->getTileSize();
        for (auto& itr : m_collisions) {
            if (not m_AABB.intersects(itr.m_tileBounds))
                continue;
            // Calculate the differences between the center points of the entity's AABB 
            // and the tile's bounding box for both the X and Y axes. These differences
            // are used to determine how far the entity has overlapped with the tile.
            float xDiff = (m_AABB.left + (m_AABB.width / 2)) - (itr.m_tileBounds.left + (itr.m_tileBounds.width / 2));
            float yDiff = (m_AABB.top + (m_AABB.height / 2)) - (itr.m_tileBounds.top + (itr.m_tileBounds.height / 2));
            float resolve = 0;
            // If the entity has overlapped more in the X direction, resolve X-axis 
            // collision. Otherwise, resolve Y-axis collision.
            if (abs(xDiff) > abs(yDiff)) {
                // If the entity is on the right/left side of the tile, calculate
                // how much to move the entity to the right to prevent overlapping
                if (xDiff > 0) { 
                    resolve = (itr.m_tileBounds.left + tileSize) - m_AABB.left;
                } else {
                    resolve = -((m_AABB.left + m_AABB.width) - itr.m_tileBounds.left);
                }
                move(resolve, 0);
                m_velocity.x = 0;
                m_isCollidingOnX = true;
                std::cout << "Colliding on X" << std::endl;
            } else {
                if (yDiff > 0) {
                    resolve = (itr.m_tileBounds.top + tileSize) - m_AABB.top;
                } else {
                    resolve = (m_AABB.top + m_AABB.height) - (itr.m_tileBounds.top);
                }
                move(0, resolve);
                m_velocity.y = 0;
                if (m_isCollidingOnY)
                    continue;
                m_tileUnderneath = itr.m_tile;
                m_isCollidingOnY = true;
                std::cout << "Colliding on Y" << std::endl;
            }
        }
        m_collisions.clear();
    }
    if (not m_isCollidingOnY)
        m_tileUnderneath = nullptr;
}

unsigned int BaseEntity::getId() {
    return m_id;
}

EntityType BaseEntity::getType() {
    return m_type;
}

void BaseEntity::setType(const EntityType &type) {
    m_type = type;
}

sf::Vector2f &BaseEntity::getPosition()
{
    return m_position;
}

void BaseEntity::setPosition(const sf::Vector2f& position) {
    m_position = position;
    updateAABB();
}

void BaseEntity::setPosition(const float &x, const float &y) {
    m_position = sf::Vector2f(x, y);
    updateAABB();
}

void BaseEntity::setAcceleration(float x, float y) {
    m_acceleration = sf::Vector2f(x, y);
    updateAABB();
}

sf::Vector2f& BaseEntity::getSize() {
    return m_boxSize;
}

void BaseEntity::setSize(const sf::Vector2f& size) {
    m_boxSize = size;
    updateAABB();
}

void BaseEntity::setSize(const float &x, const float &y) {
    m_boxSize = sf::Vector2f(x, y);
    updateAABB();
}

EntityState BaseEntity::getState() {
    return m_state;
}

void BaseEntity::setState(const EntityState& state) {
    if (m_state == EntityState::Dying) return;
    m_state = state;
}

bool sortCollisions(const CollisionElement &first, const CollisionElement &second) {
    return first.m_area > second.m_area;
}
