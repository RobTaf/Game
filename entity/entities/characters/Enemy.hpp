#ifndef Enemy_hpp
#define Enemy_hpp

#include "Character.hpp"

class Enemy: public Character {
public:
    Enemy(EntityManager* entityManager);
    ~Enemy();
    void onEntityCollision(BaseEntity* collider, bool attack) override;

};

#endif