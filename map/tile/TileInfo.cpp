#include "TileInfo.hpp"
#include "../../utility/SharedContext.hpp"

TileInfo::TileInfo(SharedContext* context, const std::string& texture, TileID id = 0) :
    m_context(context), m_id(0), m_texture(texture), m_deadly(false) {

    TextureManager* textureManager = m_context->m_textureManager;
    if (m_texture == "") {
        m_id = id;
        std::cout << "no texture";
        return;
    }

    if (not textureManager->requireResource(texture)) {
        std::cerr << "! Failed loading tile textures: " << texture << std::endl;
        return;
    }

    m_texture = texture;
    m_id = id;
    m_sprite.setTexture(*textureManager->getResource(texture));
    sf::IntRect tileBoundaries(
        m_id % (Sheet::SheetWidth / Sheet::TileSize) * Sheet::TileSize,
        m_id / (Sheet::SheetHeight / Sheet::TileSize) * Sheet::TileSize,
        Sheet::TileSize,
        Sheet::TileSize
    );
    std::cout << "Tile boundaries: " << tileBoundaries.getPosition().x << " " << tileBoundaries.getPosition().y << std::endl;
    m_sprite.setTextureRect(tileBoundaries);
}

TileInfo::~TileInfo() {
    if (m_texture == "") return;
    m_context->m_textureManager->releaseResource(m_texture);
}
