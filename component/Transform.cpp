#include "Transform.hpp"

Transform::Transform(): BaseComponent(Component::Transform) {
    // Just for now
    setRotation(0, 0);
}

Transform::~Transform()
{
}

void Transform::readIn(std::stringstream &stream) {
    stream >> m_position.x >> m_position.y >> m_scale.x >> m_scale.y;
}

const sf::Vector2f &Transform::getPosition() {
    return m_position;
}

const sf::Vector2f &Transform::getOldPosition() {
    return m_positionOld;
}

void Transform::setPosition(const sf::Vector2f &position) {
    m_positionOld = m_position;
    m_position = position;
}

void Transform::setPosition(float x, float y) {
    m_positionOld = m_position;
    m_position = sf::Vector2f(x, y);
}

const sf::Vector2f &Transform::getRotation() {
    return m_rotation;
}

void Transform::setRotation(const sf::Vector2f &rotation) {
    m_rotation = rotation;
}

void Transform::setRotation(float x, float y) {
    m_rotation = sf::Vector2f(x, y);
}

const sf::Vector2f& Transform::getScale() {
    return m_scale;
}

void Transform::setScale(const sf::Vector2f &scale) {
    m_scale = scale;
}

void Transform::setScale(float x, float y) {
    m_scale = sf::Vector2f(x, y);
}

void Transform::moveBy(float x, float y) {
    m_positionOld = m_position;
    m_position += sf::Vector2f(x, y);
}

void Transform::moveBy(const sf::Vector2f &vector) {
    m_positionOld = m_position;
    m_position += vector;
}

void Transform::rotateBy(float x, float y)
{
}

void Transform::rotateBy(const sf::Vector2f &vector)
{
}

void Transform::scaleBy(float x, float y) {
    m_scale.x *= x;
    m_scale.y *= y;
}

void Transform::scaleBy(const sf::Vector2f &vector) {
    m_scale.x *= vector.x;
    m_scale.y *= vector.y;
}
