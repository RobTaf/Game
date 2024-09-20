#ifndef Character_hpp
#define Character_hpp

#include "../BaseEntity.hpp"
#include "../../../animation/spritesheet/SpriteSheet.hpp"

class Character: public BaseEntity {
    friend class EntityManager;
public:
    Character(EntityManager* entityManager);
    virtual ~Character();

    void move(const Direction& direction);
    void jump();
    void attack();
    void getHurt(const int& damage);
    void load(const std::string& fileName);

    void draw(sf::RenderWindow* window);
    virtual void update(float deltaTime);
    virtual void onEntityCollision(BaseEntity* collider, bool attack) = 0;

protected:
    void updateAttackAABB();
    void animate();
    SpriteSheet m_spriteSheet;
    float m_jumpVelocity;
    int m_hitpoints;
    sf::FloatRect m_attackAABB;
    sf::Vector2f m_attackAABBoffset;
};

#endif