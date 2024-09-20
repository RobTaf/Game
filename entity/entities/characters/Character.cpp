#include "Character.hpp"
#include "../../EntityManager.hpp"

Character::Character(EntityManager *entityManager): 
    BaseEntity(entityManager),
    m_spriteSheet(m_entityManager->getContext()->m_textureManager),
    m_jumpVelocity(250),
    m_hitpoints(5) {
        m_name = "Character";
}

Character::~Character()
{
}

void Character::move(const Direction &direction) {
    if (getState() == EntityState::Dying) return;
    m_spriteSheet.setSpriteDirection(direction);
    if (direction == Direction::Left) {
        accelerate(-m_speed.x, 0);
    } else {
        accelerate(m_speed.x, 0);
    }
}

void Character::jump() {
    if (getState() == EntityState::Dying or 
        getState() == EntityState::Jumping or 
        getState() == EntityState::Hurt) {
        return;
    }
    setState(EntityState::Jumping);
    // The velocity has to be high enough in order to
    // break the gravitational force of the level.
    addVelocity(0, -m_jumpVelocity);
}

void Character::attack() {
    if (getState() == EntityState::Dying or 
        getState() == EntityState::Jumping or 
        getState() == EntityState::Hurt or
        getState() == EntityState::Attacking) {
        return;
    }
    setState(EntityState::Attacking);
}

void Character::getHurt(const int& damage) {
    if (getState() == EntityState::Dying or 
        getState() == EntityState::Hurt) {
        return;
    }
    m_hitpoints = (m_hitpoints - damage > 0) ? m_hitpoints - damage : 0;
    if (m_hitpoints > 0) {
        setState(EntityState::Hurt);
    } else {
        setState(EntityState::Dying);
    }
}

void Character::load(const std::string& fileName) {
    std::ifstream file;
    file.open(Utility::workingDirectory() + "assets/characters/" + fileName + ".char");
    if (not file.is_open()) {
        std::cout << "! Failed loading character file: " << fileName << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream keystream(line);
        std::string type;
        keystream >> type;
        if (type == "Name") {
            keystream >> m_name;
        } else if (type == "Spritesheet") {
            std::string path;
            keystream >> path;
            m_spriteSheet.loadSheet(fileName);
        } else if (type == "Hitpoints") {
            keystream >> m_hitpoints;
        } else if (type == "BoundingBox") {
            sf::Vector2f boundingSize;
            keystream >> boundingSize.x >> boundingSize.y;
            setSize(boundingSize.x, boundingSize.y);
        } else if (type == "DamageBox") {
            keystream >> m_attackAABBoffset.x >> m_attackAABBoffset.y >> m_attackAABB.width >> m_attackAABB.height;
        } else if (type == "Speed") {
            keystream >> m_speed.x >> m_speed.y;
        } else if (type == "JumpVelocity") {
            keystream >> m_jumpVelocity;
        } else if (type == "MaxVelocity") {
            keystream >> m_velocityMax.x >> m_velocityMax.y;
        } else {
            std::cout << "Unknown type in character file: " << fileName << std::endl;
        }
    }
    file.close();
}

void Character::draw(sf::RenderWindow *window) {
    m_spriteSheet.draw(window);
}

void Character::update(float deltaTime) {
    BaseEntity::update(deltaTime);
    if (m_attackAABB.width != 0 and m_attackAABB.height != 0) {
        updateAttackAABB();
    }
    if (getState() != EntityState::Dying and getState() != EntityState::Attacking and getState() != EntityState::Hurt) {
        if (abs(m_velocity.y) > 0.001f) {
            setState(EntityState::Jumping);
        } else if (abs(m_velocity.x) >= 0.1f) {
            setState(EntityState::Walking);
        } else {
            setState(EntityState::Idle);
        }
    } else if (getState() == EntityState::Attacking or getState() == EntityState::Hurt) {
        if (not m_spriteSheet.getCurrentAnimation()->isPlaying()) {
            setState(EntityState::Idle);
        }
    } else if (getState() == EntityState::Dying) {
        if (not m_spriteSheet.getCurrentAnimation()->isPlaying()) {
            m_entityManager->remove(m_id);
        }
    }
    animate();
    m_spriteSheet.update(deltaTime);
    m_spriteSheet.setSpritePosition(m_position);
}

void Character::updateAttackAABB() {
    m_attackAABB.left = (m_spriteSheet.getSpriteDirection() == Direction::Left ? 
        (m_AABB.left - m_attackAABB.width) - m_attackAABBoffset.x : (m_AABB.left + m_AABB.width) + m_attackAABBoffset.x);
    m_attackAABB.top = m_AABB.top + m_attackAABBoffset.y;
}

void Character::animate() {
    EntityState state = getState();
    if (state == EntityState::Walking and m_spriteSheet.getCurrentAnimation()->getName() != "Walk") {
        m_spriteSheet.setAnimation("Walk", true, true);
    } else if (state == EntityState::Jumping and m_spriteSheet.getCurrentAnimation()->getName() != "Jump") {
        m_spriteSheet.setAnimation("Jump", true, false);
    } else if(state == EntityState::Attacking && m_spriteSheet.getCurrentAnimation()->getName() != "Attack") {
        m_spriteSheet.setAnimation("Attack",true,false);
    } else if(state == EntityState::Hurt && m_spriteSheet.getCurrentAnimation()->getName() != "Hurt") {
        m_spriteSheet.setAnimation("Hurt",true,false);
    } else if(state == EntityState::Dying && m_spriteSheet.getCurrentAnimation()->getName() != "Death") {
        m_spriteSheet.setAnimation("Death",true,false);
    } else if(state == EntityState::Idle && m_spriteSheet.getCurrentAnimation()->getName() != "Idle") {
        m_spriteSheet.setAnimation("Idle",true,true);
    }
}
