#ifndef TileInfo_hpp
#define TileInfo_hpp

#include <string>
#include <SFML/Graphics.hpp>

#include "Sheet.hpp"

using TileID = unsigned int;

class SharedContext;

struct TileInfo {
    TileInfo(SharedContext* context, const std::string& texture, TileID id);
    ~TileInfo();

    std::string m_name;
    std::string m_texture;
    sf::Sprite m_sprite;
    sf::Vector2f m_friction;
    TileID m_id;
    bool m_deadly;

    SharedContext* m_context;
};

#endif