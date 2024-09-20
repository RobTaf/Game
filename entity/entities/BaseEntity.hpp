#ifndef Entity_hpp
#define Entity_hpp

#include <SFML/Graphics.hpp>

#include "../collision/CollisionElement.hpp"
#include "../../map/tile/TileInfo.hpp"

enum class EntityType {
    Base = 0, Enemy, Player, Weapon, Effect,
};

enum class EntityState {
    Idle = 0, Walking, Jumping, Attacking, Hurt, Dying
};

class EntityManager;

class BaseEntity {
    friend class EntityManager;
public:
    BaseEntity(EntityManager* entityManager);
    virtual ~BaseEntity();

    void move(float x, float y);
    void addVelocity(float x, float y);
    void accelerate(float x, float y);
    void setAcceleration(float x, float y);
    void applyFriction(float x, float y);

    virtual void update(float deltaTime);
    virtual void draw(sf::RenderWindow* window) = 0;

    EntityType getType();
    void setType(const EntityType& type);

    EntityState getState();
    void setState(const EntityState& state);

    unsigned int getId();

    sf::Vector2f& getPosition();
    void setPosition(const sf::Vector2f& position);
    void setPosition(const float& x, const float& y);

    sf::Vector2f& getSize();
    void setSize(const sf::Vector2f& size);
    void setSize(const float& x, const float& y);

protected:
    void updateAABB();
    void checkCollisions();
    void resolveCollisions();
    virtual void onEntityCollision(BaseEntity* collider, bool attack) = 0;

    unsigned int m_id;
    std::string m_name;
    EntityType m_type;
    EntityState m_state;

    sf::Vector2f m_position;
    sf::Vector2f m_positionOld;
    sf::Vector2f m_velocity;
    sf::Vector2f m_velocityMax;
    sf::Vector2f m_acceleration;
    sf::Vector2f m_speed;
    sf::Vector2f m_friction;

    TileInfo* m_tileUnderneath;
    sf::Vector2f m_boxSize;
    sf::FloatRect m_AABB;
    bool m_isCollidingOnX;
    bool m_isCollidingOnY;
    Collisions m_collisions;

    // Collisions m_collisions;
    EntityManager* m_entityManager;
};

bool sortCollisions(const CollisionElement& first, const CollisionElement& second);

#endif