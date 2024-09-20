#include "CollisionElement.hpp"

CollisionElement::CollisionElement(float area, TileInfo* info, const sf::FloatRect &bounds) {
    m_area = area;
    m_tile = info;
    m_tileBounds = bounds;
}