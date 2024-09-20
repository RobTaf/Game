#include "Player.hpp"
#include "../../EntityManager.hpp"

Player::Player(EntityManager* entityManager): Character(entityManager) {
    load("Player");
    m_type = EntityType::Player;
    m_state = EntityState::Idle;

    EventManager* eventManager = m_entityManager->getContext()->m_eventManager;
    eventManager->addCallback<Player>(StateType::Game, "Player_MoveLeft", &Player::react, this);
    eventManager->addCallback<Player>(StateType::Game, "Player_MoveRigth", &Player::react, this);
    eventManager->addCallback<Player>(StateType::Game, "Player_Jump", &Player::react, this);
    eventManager->addCallback<Player>(StateType::Game, "Player_Attack", &Player::react, this);

    m_spriteSheet.setAnimation("Idle", true, true);
    m_spriteSheet.getCurrentAnimation()->setLoop(true);
}

Player::~Player() {
    EventManager* eventManager = m_entityManager->getContext()->m_eventManager;
    eventManager->removeCallback(StateType::Game, "Player_MoveLeft");
    eventManager->removeCallback(StateType::Game, "Player_MoveRight");
    eventManager->removeCallback(StateType::Game, "Player_Jump");
    eventManager->removeCallback(StateType::Game, "Player_Attack");
}

void Player::onEntityCollision(BaseEntity* collider, bool attack) {
    if (m_state == EntityState::Dying) { return; }
    if (attack) {
        if (m_state != EntityState::Attacking) { return; }
        if (not m_spriteSheet.getCurrentAnimation()->isInAction()) { return; }
        if (collider->getType() != EntityType::Enemy and collider->getType() != EntityType::Player) { return; }
        Character* opponent = (Character*)collider;
        opponent->getHurt(1);
        if (m_position.x > opponent->getPosition().x) {
            opponent->addVelocity(-32, 0);
        } else {
            opponent->addVelocity(32, 0);
        }
    } else {
        // Other behaviour
    }
}

void Player::react(EventDetails* details) {
    if (details->m_keyCode == 71) {
        move(Direction::Left);
    } else if (details->m_keyCode == 72) {
        move(Direction::Right);
    } else if (details->m_keyCode == 57) {
        jump();
    } else if (details->m_keyCode == 17) {
        attack();
    }
}
