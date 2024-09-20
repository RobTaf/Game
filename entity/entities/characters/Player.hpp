#ifndef Player_hpp
#define Player_hpp

#include "Character.hpp"
#include "../../../input/EventManager.hpp"

class Player: public Character {
public:
    Player(EntityManager* entityManager);
    ~Player();

    void onEntityCollision(BaseEntity* collider, bool attack) override;
    void react(EventDetails* details);
};

#endif