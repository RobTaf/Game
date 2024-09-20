#ifndef CollisionElement_hpp
#define CollisionElement_hpp

#include <SFML/Graphics.hpp>
#include "../../map/tile/TileInfo.hpp"

struct CollisionElement {
    CollisionElement(float area, TileInfo* info, const sf::FloatRect& bounds);
    float m_area;
    TileInfo* m_tile;
    sf::FloatRect m_tileBounds;
};

using Collisions = std::vector<CollisionElement>;

#endif