#include "Enemy.hpp"

Enemy::Enemy(EntityManager* entityManager): Character(entityManager) {
}

Enemy::~Enemy()
{
}

void Enemy::onEntityCollision(BaseEntity *collider, bool attack)
{
}